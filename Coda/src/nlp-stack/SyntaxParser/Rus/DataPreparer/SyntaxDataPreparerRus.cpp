#include "SyntaxDataPreparerRus.h"
#include "Disambiguator/Rus/Settings/DisambiguatorSettingsRus.h"

#include "IDisambiguator.h"
#include "FileWriter.h"
#include "SyntaxNode.h"
#include "ITokenizer.h"
#include "Tools.h"

using std::shared_ptr;

using Tokenization::ITokenizer;
using Disambiguation::IDisambiguator;
using Disambiguation::DisambiguatedData;

namespace SyntaxParser
{

SyntaxDataPreparerRus::SyntaxDataPreparerRus(
    const string& treesFile
    , const string& trainFile
    , const string& testFile
    , shared_ptr<DataFunctor> trainFunctor
    , shared_ptr<DataFunctor> testFunctor)
    : SyntaxDataPreparer(
        treesFile, trainFile
        , testFile, trainFunctor
        , testFunctor)
{
}

void SyntaxDataPreparerRus::Prepare()
{
    SyntaxDataPreparer::Prepare();
    prepareEdgeCalculatorConfig();
}

void SyntaxDataPreparerRus::addingPEREHtoSyntaxTrees()
{
    // Creating disambiguator
    shared_ptr<ITokenizer> tokenizer
        = ITokenizer::GetTokenizer(Tools::Language::RU);
    shared_ptr<IDisambiguator> disambiguator
        = IDisambiguator::GetDisambiguator(Tools::Language::RU);

    wifstream in(string(SYNTAX_CONFIG_FOLDER) + "RU/SyntaxTrees.txt");
    wofstream out(string(SYNTAX_CONFIG_FOLDER) + "RU/SyntaxTreesWithPEREH.txt");

    int enumerator = 0;
    wstring line;
    while (getline(in, line))
    {
        try
        {
            std::wcout << enumerator << L"\r";
            if (Tools::Trim(line).size() == 0)
            {
                ++enumerator;
                wcout << L"Empty line: " << enumerator << L": " << line << endl;
                out << std::endl;
                continue;
            }
            SyntaxTree tree(line);
            vector<SyntaxNode>& nodes = tree.GetNodes();
            vector<Token> tokens;
            for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
            {
                tokens.push_back(Token(static_cast<Token>(nodes[nodeIndex])));
            }
            vector<DisambiguatedData> disambiguated = disambiguator->Disambiguate(tokens);
            for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
            {
                if (Tools::ContainsSubstring(disambiguated[nodeIndex].label, L"@\u041f\u0415\u0420\u0415\u0425"))//here was cyrrilic symbols: ПЕРЕХ
                {
                    nodes[nodeIndex].label += L"@\u041f\u0415\u0420\u0415\u0425";//here was cyrrilic symbols: ПЕРЕХ
                } else if (Tools::ContainsSubstring(disambiguated[nodeIndex].label, L"@\u041d\u0415\u041f\u0415\u0420\u0415\u0425")) {//here was cyrrilic symbols: НЕПЕРЕХ
                    nodes[nodeIndex].label += L"@\u041d\u0415\u041f\u0415\u0420\u0415\u0425";//here was cyrrilic symbols: НЕПЕРЕХ
                }
            }
            out << tree.ToRecursiveString() << std::endl;
            ++enumerator;
        }
        catch(...)
        {
            ++enumerator;
            wcout << L"Exception in line: " << enumerator << L": " << line << endl;
            out << std::endl;
        }
    }
    in.close();
    out.close();

}

void SyntaxDataPreparerRus::prepareEdgeCalculatorConfig()
{
    wcout << L"Calculating possible connections..." << std::endl;
    vector<wstring> lines;
    FileManager::Read(string(SYNTAX_CONFIG_FOLDER) + "RU/TrainingSet.txt", &lines);
    unordered_map<wstring, unordered_map<wstring, int> > connections;
    for (size_t lineIndex = 0; lineIndex < lines.size(); ++lineIndex)
    {
        SyntaxTree tree(lines[lineIndex]);
        const vector<SyntaxNode>& nodes = tree.GetNodes();
        for (size_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
        {
            const wstring& parent = nodes[nodeIndex].label;
            const vector<int>& children = nodes[nodeIndex].neighbours;
            for (size_t childIndex = 0; childIndex < children.size(); ++childIndex)
            {
                const wstring& child = nodes[children[childIndex]].label;
                Tools::AddToMapMap(&connections, parent, child, 1);
            }
        }
    }
    size_t numberOfConnections = 0;
    for (auto iter = connections.begin(); iter != connections.end()
         ; ++iter)
    {
        numberOfConnections += iter->second.size();
    }
    FileManager::Write(
        string(SYNTAX_CONFIG_FOLDER) + "RU/EdgeCalculatorConfig.txt"
        , connections);
    wcout << L"There are " << numberOfConnections << " of parent -> son transition types..."
          << std::endl;
}

}
