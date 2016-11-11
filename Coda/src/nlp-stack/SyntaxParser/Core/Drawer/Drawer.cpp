#include "Drawer.h"

namespace SyntaxParser
{

void Drawer::Draw(const SyntaxTree& real, const SyntaxTree& predicted)
{
    draw(getDualDotText(real, predicted));
}

void Drawer::Draw(const SyntaxTree& tree)
{
    draw(tree.ToDotString());
}

void Drawer::draw(const wstring& dotGraph)
{
    wofstream out(SYNTAX_OUTPUT_TREE_PATH);
    out << dotGraph;
    out.close();

    string drawTreeScript = SYNTAX_DRAW_TREE_SCRIPT;
    drawTreeScript += " ";
    drawTreeScript += GRAPHVIZ_PATH;
    drawTreeScript += " ";
    drawTreeScript += SYNTAX_OUTPUT_TREE_PATH;
    system(drawTreeScript.c_str());
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

