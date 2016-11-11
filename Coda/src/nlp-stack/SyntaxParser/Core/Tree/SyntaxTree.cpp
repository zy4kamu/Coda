#include "SyntaxTree.h"
#include "Tools.h"

#include "Drawer.h"
#include "Math/MatrixTools.h"

using namespace SyntaxParser;

SyntaxTree::SyntaxTree()
{
}

SyntaxTree::SyntaxTree(const SyntaxTree& tree) 
    : rootIndex(tree.rootIndex)
    , nodes(tree.nodes)
    , sentence(tree.sentence)
{
}

SyntaxTree::SyntaxTree(SyntaxTree&& tree)
    : rootIndex(tree.rootIndex)
    , nodes(std::move(tree.nodes))
    , sentence(std::move(tree.sentence))
{
}

SyntaxTree::SyntaxTree(int rootIndex
    , vector<SyntaxNode>&& nodes
    , wstring&& sentence)
    : rootIndex(rootIndex)
    , nodes(std::move(nodes))
    , sentence(std::move(sentence))
{
}

SyntaxTree& SyntaxTree::operator = (const SyntaxTree& tree)
{
    rootIndex = tree.rootIndex;
    nodes = tree.nodes;
    sentence = tree.sentence;
    return *this;
}

SyntaxTree& SyntaxTree::operator = (SyntaxTree&& tree)
{
    rootIndex = tree.rootIndex;
    nodes = std::move(tree.nodes);
    sentence = std::move(tree.sentence);
    return *this;
}

SyntaxTree::SyntaxTree(const wstring& line)
{
    readTree(line);
}

void SyntaxTree::readRecursiveTree(const wstring& line)
{
    // Initialize empty nodes
    vector<wstring> splitted = Tools::Split(line, L"#");
    int size = stoi(splitted[0]);
    nodes.resize(size);
    for (int nodeIndex = 0; nodeIndex < size
        ; ++nodeIndex)
    {
        nodes[nodeIndex].index = nodeIndex;
    }
    // Fill each node
    for (int splittedIndex = 1; splittedIndex <= size; ++splittedIndex)
    {
        vector<wstring> nodeSplitted = Tools::Split(splitted[splittedIndex], L"^");
        // Set index
        int index = stoi(nodeSplitted[0]);
        // Evaluate root
        if (splittedIndex == 1)
        {
            rootIndex = index;
            nodes[index].parentIndex = -1;
        }
        // Set common data
        vector<wstring> tokenAndPunctuation = Tools::Split(nodeSplitted[1], L"|");
        nodes[index].content = tokenAndPunctuation[0];
        nodes[index].lemma = nodeSplitted[2];
        nodes[index].label = nodeSplitted[3];
        nodes[index].splittedLabel = Tools::Split(nodeSplitted[3], L"@");
        // Set punctuation
        if (tokenAndPunctuation.size() > 1)
        {
            nodes[index].punctuation.reserve(tokenAndPunctuation[1].size());
            for (size_t punctuationIndex = 0; punctuationIndex < tokenAndPunctuation[1].size()
                ; ++punctuationIndex)
            {
                nodes[index].punctuation.emplace_back(wstring(
                    tokenAndPunctuation[1].begin() + punctuationIndex
                    , tokenAndPunctuation[1].begin() + punctuationIndex + 1));
            }
        }
        // Set graph dependencies
        for (size_t neighbourEnumerator = 4; neighbourEnumerator < nodeSplitted.size()
            ; ++neighbourEnumerator)
        {
            int neighbourIndex = stoi(nodeSplitted[neighbourEnumerator]);
            nodes[index].neighbours.push_back(nodes[neighbourIndex].index);
            nodes[neighbourIndex].parentIndex = index;
        }
    }
    // Create sentence
    if (nodes.size() > 0)
    {
        sentence = nodes[0].content;
        for (size_t punctuationIndex = 0
            ; punctuationIndex < nodes[0].punctuation.size()
            ; ++punctuationIndex)
        {
            sentence += nodes[0].punctuation[punctuationIndex];
        }
        for (size_t nodeIndex = 1; nodeIndex < nodes.size(); ++nodeIndex)
        {
            sentence += L" " + nodes[nodeIndex].content;
            for (size_t punctuationIndex = 0
                ; punctuationIndex < nodes[nodeIndex].punctuation.size()
                ; ++punctuationIndex)
            {
                sentence += nodes[nodeIndex].punctuation[punctuationIndex];
            }
        }
    }
}

void SyntaxTree::readTree(const wstring& line)
{
    vector<wstring> nodesString = Tools::Split(line, L"~");
    size_t size = nodesString.size();
    this->nodes.resize(size);
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        vector<wstring> splitted = Tools::Split(nodesString[nodeIndex], L"\t");
        SyntaxNode& node = nodes[nodeIndex];
        node.index = nodeIndex;
        Tools::ConvertToInt(splitted[1], node.parentIndex);
        node.content = std::move(splitted[2]);
        node.lemma = std::move(splitted[3]);
        node.label = splitted[4];
        node.splittedLabel = Tools::Split(splitted[4], L"@");
        if (splitted.size() > 5)
        {
            node.punctuation = Tools::Split(splitted[5], L"|");
        }

        if (node.parentIndex != -1) {
            nodes[node.parentIndex].neighbours.push_back(nodeIndex);
        } else {
            this->rootIndex = nodeIndex;
        }

        this->sentence += node.content;
        if (splitted.size() > 5)
        {
            this->sentence += splitted[5];
        }
        if (nodeIndex + 1 != size)
        {
            this->sentence += L" ";
        }
    }
}

SyntaxTree::SyntaxTree(const vector<Disambiguation::DisambiguatedData>& syntaxNodes)
{
    nodes.resize(syntaxNodes.size());
    for (size_t nodeIndex = 0; nodeIndex < syntaxNodes.size()
        ; ++nodeIndex)
    {
        nodes[nodeIndex] = SyntaxNode(syntaxNodes[nodeIndex]);
        nodes[nodeIndex].index = nodeIndex;
    }
}

SyntaxTree::SyntaxTree(int size)
{
    nodes.resize(size);
    for (int nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        nodes[nodeIndex].index = nodeIndex;
    }
}

SyntaxTree::~SyntaxTree(void)
{
}

wstring SyntaxTree::ToDotString(bool createCap, const wstring& nodesPrefix) const
{
    wstring dot2texTree = createCap ? L"digraph {\n" : L"";
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        wstring itString = Tools::ToWstring(nodeIndex);
        vector<wstring> splitted;
        dot2texTree += L"\t" + nodesPrefix + L"node_" + itString 
            + L"[label=\"" + itString + L": " + nodes[nodeIndex].content + L"\\n" + nodes[nodeIndex].label
            + L"\"];\n";
    }
    for (size_t it = 0; it < nodes.size(); ++it)
    {
        for (size_t jt = 0; jt < nodes[it].neighbours.size(); ++jt)
        {
            dot2texTree += 
                L"\t" + nodesPrefix + L"node_" + Tools::ToWstring(it) + L" -> " +
                nodesPrefix + L"node_" + Tools::ToWstring(nodes[it].neighbours[jt]) + L";\n";
        }
    }
    if (nodesPrefix != L"")
    {
        dot2texTree += 
            L"\t" + nodesPrefix + L" -> " + nodesPrefix + L"node_" + 
            Tools::ToWstring(rootIndex) + L";\n";
    }
    if (createCap) 
    {
        dot2texTree += L"}";
    }
    return dot2texTree;
}

wstring SyntaxTree::ToReadableString(bool compressToOneString) const
{
    const wchar_t separator = compressToOneString ? L'~' : L'\n';
    wstring str;
    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
    {
        str += Tools::ConvertIntToWstring(nodeIndex);
        str += L"\t";
        str += Tools::ConvertIntToWstring(nodes[nodeIndex].parentIndex);
        str += L"\t";
        str += nodes[nodeIndex].content;
        str += L"\t";
        str += nodes[nodeIndex].lemma;
        str += L"\t";
        str += nodes[nodeIndex].label;
        str += L"\t";
        if (nodes[nodeIndex].punctuation.size() > 0)
        {
            str += nodes[nodeIndex].punctuation[0];
            for (size_t index = 1; index < nodes[nodeIndex].punctuation.size(); ++index)
            {
                str += L"|";
                str += nodes[nodeIndex].punctuation[index];
            }
        }
        if (nodeIndex + 1 < nodes.size())
        {
            str += separator;
        }
    }
    return str;
}

wstring SyntaxTree::ToRecursiveString() const
{
    return Tools::ConvertIntToWstring(nodes.size()) + L"#" +
        convertToRecursiveString(nodes[rootIndex]);
}

vector<SyntaxNode>& SyntaxTree::GetNodes()
{
    return nodes;
}

const vector<SyntaxNode>& SyntaxTree::GetNodes() const
{
    return nodes;
}

int SyntaxTree::GetRootIndex() const
{
    return rootIndex;
}

int SyntaxTree::GetSize() const
{
    return nodes.size();
}

void SyntaxTree::SetEdges(const vector<vector<int> >& dependencies)
{
    for (size_t it = 0; it < nodes.size(); ++it)
    {
        nodes[it].neighbours = dependencies[it];
        for (size_t neighbourIndex = 0; neighbourIndex < dependencies[it].size()
             ; ++neighbourIndex)
        {
            nodes[dependencies[it][neighbourIndex]].parentIndex
                = it;
        }
    }
}

void SyntaxTree::SetRoot(int _rootIndex)
{
    rootIndex = _rootIndex;
    nodes[rootIndex].parentIndex = -1;
}

void SyntaxTree::GetSubTree(int index, set<int>* subTree) const
{
    for (size_t it = 0; it < nodes[index].neighbours.size(); ++it)
    {
        int toAddIndex = nodes[index].neighbours[it];
        subTree->insert(toAddIndex);
        GetSubTree(toAddIndex, subTree);
    }
}

const wstring& SyntaxTree::GetSentence() const
{
    return sentence;
}

int SyntaxTree::GetParentIndex(int nodeIndex) const // !!! rewrite it
{
    if (nodeIndex == rootIndex)
    {
        return -1;
    }
    for (size_t parentIndex = 0; parentIndex < nodes.size(); ++parentIndex)
    {
        for (size_t sonIndex = 0; sonIndex < nodes[parentIndex].neighbours.size(); ++sonIndex)
        {
            if (nodes[parentIndex].neighbours[sonIndex] == nodeIndex)
            {
                return parentIndex;
            }
        }
    }
    return -1;
}

bool SyntaxTree::IsValid() const
{
    return true;
}

wstring SyntaxTree::convertToRecursiveString(const SyntaxNode& node) const
{
    wstring specialRepresentation = Tools::ConvertIntToWstring(node.index);
    specialRepresentation += L"^";
    specialRepresentation += node.content;
    if (node.punctuation.size() > 0)
    {
        specialRepresentation += L"|";
        for (size_t punctIndex = 0; punctIndex < node.punctuation.size()
             ; ++punctIndex)
        {
            specialRepresentation += node.punctuation[punctIndex];
        }
    }
    specialRepresentation += L"^";
    specialRepresentation += node.lemma;
    specialRepresentation += L"^";
    specialRepresentation += node.label;
    for (size_t childIndex = 0; childIndex < node.neighbours.size()
        ; ++childIndex)
    {
        const SyntaxNode& child = nodes[node.neighbours[childIndex]];
        specialRepresentation += L"^" + Tools::ConvertIntToWstring(child.index);
    }
    for (size_t childIndex = 0; childIndex < node.neighbours.size()
        ; ++childIndex)
    {
        const SyntaxNode& child = nodes[node.neighbours[childIndex]];
        specialRepresentation += L"#" + convertToRecursiveString(child);
    }
    return specialRepresentation;
}

matrix<double> calculatePartitionFunctionMatrix(
    const vector<vector<double> >& edgeWeights
    , const vector<double>& rootWeights)
{
    int size = rootWeights.size();
    matrix<double> partititionFunctionMatrix(size, size);
    // Set non-diagonal elements
    for (int headerIndex = 0; headerIndex < size; ++headerIndex)
    {
        for (int modifierIndex = 0; modifierIndex < size; ++modifierIndex)
        {
            if (headerIndex != modifierIndex)
            {
                partititionFunctionMatrix(headerIndex, modifierIndex)
                    = -edgeWeights[headerIndex][modifierIndex];
            }
        }
    }
    // Set diagonal elements
    for (int modifierIndex = 1; modifierIndex < size; ++modifierIndex)
    {
        double& currentDiagonalElement
            = partititionFunctionMatrix(modifierIndex, modifierIndex);
        currentDiagonalElement = 0;
        for (int headIndex = 0; headIndex < size; ++headIndex)
        {
            if (headIndex != modifierIndex)
            {
                currentDiagonalElement -= partititionFunctionMatrix(
                    headIndex, modifierIndex);
            }
        }
    }
    // Replace first raw
    for (int modifierIndex = 0; modifierIndex < size; ++modifierIndex)
    {
        partititionFunctionMatrix(0, modifierIndex) =
            rootWeights[modifierIndex];
    }
    return partititionFunctionMatrix;
}

void SyntaxTree::Correct()
{
//    for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
//    {
//        SyntaxNode& currentNode = nodes[nodeIndex];
//        wstring& currentTag = currentNode.label;
//        for (size_t sonIndex = 0; sonIndex < currentNode.neighbours.size(); ++sonIndex)
//        {
//            vector<int>& neighbours = currentNode.neighbours;
//            wstring& sonTag = nodes[neighbours[sonIndex]].label;
//            if (Tools::StartsWith(currentTag, L"V") && Tools::ContainsSubstring(sonTag, L"\u0420\u041e\u0414"))//here was cyrrilic symbols: РОД
//            {
//                for (size_t i = 0; i < nodes.size(); ++i)
//                    std::wcout << nodes[i].content << L" ";
//                std::wcout << nodeIndex << L" " << neighbours[sonIndex] << std::endl;
//                sonTag = Tools::Replace(sonTag, L"\u0420\u041e\u0414", L"\u0412\u0418\u041d");//here was cyrrilic symbols: РОД,ВИН
//            }
//        }
//    }
}
