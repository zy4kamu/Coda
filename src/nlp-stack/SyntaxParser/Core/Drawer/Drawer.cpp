#include "Drawer.h"

namespace SyntaxParser
{

Drawer::Drawer(const string& outputFile)
    : outputFile(outputFile.size() > 0 ? outputFile : SYNTAX_OUTPUT_TREE_PATH)
{
}

void Drawer::Draw(const SyntaxTree& real, const SyntaxTree& predicted, bool openPDF)
{
    draw(getDualDotText(real, predicted), openPDF);
}

void Drawer::Draw(const SyntaxTree& tree, bool openPDF)
{
    draw(tree.ToDotString(), openPDF);
}

void Drawer::draw(const wstring& dotGraph, bool openPDF)
{
    wofstream out(outputFile);
    out << dotGraph;
    out.close();

    if (openPDF)
    {
        string drawTreeScript = SYNTAX_DRAW_TREE_SCRIPT;
        drawTreeScript += " ";
        drawTreeScript += GRAPHVIZ_PATH;
        drawTreeScript += " ";
        drawTreeScript += outputFile;
        system(drawTreeScript.c_str());
    }
}

wstring Drawer::getDualDotText(
    const SyntaxTree& real, const SyntaxTree& predicted)
{
    const vector<SyntaxNode>& realNodes = real.GetNodes();
    const vector<SyntaxNode>& predictedNodes = predicted.GetNodes();
    wstring dot2texTree = L"digraph {\n";
    for (size_t it = 0; it < realNodes.size(); ++it)
    {
        wstring itString = Tools::ConvertIntToWstring(it);
        dot2texTree += L"\tnode_" + itString + L"[label=\"" + itString +
            L": " + realNodes[it].content + L"\n" + realNodes[it].label + L"\"];\n";
    }
    for (size_t it = 0; it < realNodes.size(); ++it)
    {
        for (size_t jt = 0; jt < realNodes[it].neighbours.size(); ++jt)
        {
            dot2texTree +=
                L"\tnode_" + Tools::ConvertIntToWstring(it) + L" -> " +
                L"node_" + Tools::ConvertIntToWstring(
                    realNodes[it].neighbours[jt]) + L";\n";
        }
    }
    for (size_t it = 0; it < predictedNodes.size(); ++it)
    {
        for (size_t jt = 0; jt < predictedNodes[it].neighbours.size(); ++jt)
        {
            dot2texTree +=
                L"\tnode_" + Tools::ConvertIntToWstring(it) + L" -> " +
                L"node_" + Tools::ConvertIntToWstring(
                predictedNodes[it].neighbours[jt]) + L" [color=red, weight=0.1];\n";
        }
    }
    dot2texTree += L"\tlabelloc=\"t\"";
    dot2texTree += L"\tlabel=\"" + real.GetSentence() + L"\"";
    dot2texTree += L"}";
    return dot2texTree;
}


}

