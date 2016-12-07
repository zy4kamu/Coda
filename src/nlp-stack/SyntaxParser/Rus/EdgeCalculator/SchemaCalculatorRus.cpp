#include "SchemaCalculatorRus.h"
#include "FileReader.h"

using namespace SyntaxParser;

SchemaCalculatorRus::SchemaCalculatorRus(const vector<SyntaxNode>* nodes)
    : nodes(nodes)
{
    FileManager::Read(string(SYNTAX_CONFIG_FOLDER) + "RU/FrequentTokens.txt"
        , &tokenFrequency, L',');
    initializePunctuation();
    initializeUnits();
}

void SchemaCalculatorRus::initializePunctuation()
{
    punctuation.resize(nodes->size());
    for (size_t nodeIndex = 0; nodeIndex < nodes->size(); ++nodeIndex)
    {
        for (size_t punctuationIndex = 0
            ; punctuationIndex < (*nodes)[nodeIndex].punctuation.size()
            ; ++punctuationIndex)
        {
            punctuation[nodeIndex] += (*nodes)[nodeIndex].punctuation[punctuationIndex];
        }
    }
}

void SchemaCalculatorRus::initializeUnits()
{
    size_t size = nodes->size();
    posUnits.resize(size);
    extendedPOSUnits.resize(size);
    fullPOSUnits.resize(size);
    tokenUnits.resize(size);
    punctuationUnits.resize(size);
    for (size_t nodeIndex = 0; nodeIndex < size; ++nodeIndex)
    {
        posUnits[nodeIndex] = calculatePOSUnit(nodeIndex);
        extendedPOSUnits[nodeIndex] = calculateExtendedPOSUnit(nodeIndex);
        fullPOSUnits[nodeIndex] = calculateFullPOSUnit(nodeIndex);
        tokenUnits[nodeIndex] = calculateTokenUnit(nodeIndex);
        punctuationUnits[nodeIndex] = calculatePunctuationUnit(nodeIndex);
    }
}

vector<wstring> SchemaCalculatorRus::CalculateSchema(int leftIndex, int rightIndex)
{
    vector<wstring> schemas;
    wstring prefix = rightIndex > leftIndex ? L"schema_L_" : L"schema_R_";
    leftIndex = std::min(leftIndex, rightIndex);
    rightIndex = std::max(leftIndex, rightIndex);
    int distance = rightIndex - leftIndex;
    if (distance < 4)
    {
        schemas.push_back(prefix + CalculateSchema(A_PRIORITY, leftIndex, rightIndex));
        schemas.push_back(prefix + CalculateSchema(B_PRIORITY, leftIndex, rightIndex));
        schemas.push_back(prefix + CalculateSchema(C_PRIORITY, leftIndex, rightIndex));
        schemas.push_back(prefix + CalculateSchema(D_PRIORITY, leftIndex, rightIndex));
    } else if (distance < 6) {
        schemas.push_back(prefix + CalculateSchema(A_PRIORITY, leftIndex, rightIndex));
        schemas.push_back(prefix + CalculateSchema(B_PRIORITY, leftIndex, rightIndex));
    } else {
        //schemas.push_back(prefix + CalculateSchema(0, leftIndex, rightIndex));
    }
    return schemas;
}

tuple<SchemaType, SchemaType, SchemaType, SchemaType> 
    SchemaCalculatorRus::calculateSchemaTypes(
    PriorityLevel distancelevel, int leftIndex, int rightIndex)
{
    SchemaType pointLevel, neighbourLevel, outLevel, insideLevel;
    switch(distancelevel)
    {
        case A_PRIORITY:
            outLevel = EMPTY;
            neighbourLevel = PUNCTUATION;
            insideLevel = PUNCTUATION;
            pointLevel = FULL_POS_PLUS_PUNCTUATION;
            break;
        case B_PRIORITY:
            outLevel = EMPTY;
            neighbourLevel = POS_PLUS_PUNCTUATION;
            insideLevel = PUNCTUATION;
            pointLevel = FULL_POS_PLUS_PUNCTUATION;
            break;
        case C_PRIORITY:
            outLevel = EMPTY;
            neighbourLevel = EXTENDED_POS_PLUS_PUNCTUATION;
            insideLevel = POS_PLUS_PUNCTUATION;
            pointLevel = FULL_POS_PLUS_PUNCTUATION;
            break;
        case D_PRIORITY:
            outLevel = PUNCTUATION;
            neighbourLevel = EXTENDED_POS_PLUS_PUNCTUATION;
            insideLevel = POS_PLUS_PUNCTUATION;
            pointLevel = FULL_POS_PLUS_PUNCTUATION;
            break;
    }
    return tuple<SchemaType, SchemaType, SchemaType, SchemaType>(
        pointLevel, neighbourLevel
        , outLevel, insideLevel);
}

wstring SchemaCalculatorRus::CalculateSchema(
    PriorityLevel level, int leftIndex, int rightIndex)
{
    SchemaType pointLevel, neighbourLevel, outLevel, insideLevel;
    std::tie(pointLevel, neighbourLevel, outLevel, insideLevel)
        = calculateSchemaTypes(level, leftIndex, rightIndex);
    
    wstring schema = L"";
    for (int nodeIndex = 0; nodeIndex < int(nodes->size()); ++nodeIndex)
    {
        int distance = std::min(
            std::abs(nodeIndex - leftIndex)
            , std::abs(nodeIndex - rightIndex));
        if (distance == 0) {
            schema += getUnit(nodeIndex, pointLevel);
        } else if (distance < 2) {
            schema += getUnit(nodeIndex, neighbourLevel);
        } else if (nodeIndex < rightIndex && nodeIndex > leftIndex) {
            schema += getUnit(nodeIndex, insideLevel);
        } else {
            schema += getUnit(nodeIndex, outLevel);
        }
    }
    return schema;
}

wstring SchemaCalculatorRus::calculateTokenUnit(int index)
{
    if(tokenFrequency.find(nodes->at(index).content) != tokenFrequency.end()) {
        return L"_" + nodes->at(index).content
                + punctuation[index] + L"_";
    } else {
        return L"";
    }
}

 wstring SchemaCalculatorRus::calculatePOSUnit(int index)
 {
     return L"_" + nodes->at(index).splittedLabel[0]
        + punctuation[index] + L"_";
 }

wstring SchemaCalculatorRus::calculateExtendedPOSUnit(int index)
{
    return L"_" + nodes->at(index).splittedLabel[0]
        + (nodes->at(index).splittedLabel.size() > 1 ?
            nodes->at(index).splittedLabel[1] : L"")
        + punctuation[index] + L"_";
}

wstring SchemaCalculatorRus::calculateFullPOSUnit(int index)
{
    return L"_" + nodes->at(index).label + punctuation[index] + L"_"; 
}

wstring SchemaCalculatorRus::calculatePunctuationUnit(int index)
{
    return L"_" + punctuation[index] + L"_";
}

wstring SchemaCalculatorRus::getUnit(int index, SchemaType level)
{
    switch(level)
    {
        case EMPTY:
            return L"";
        case PUNCTUATION:
            return punctuation[index];
        case PUNCTUATION_WITH_PLACE_FOR_TOKEN:
            return punctuationUnits[index];
        case POS_PLUS_PUNCTUATION:
            return tokenUnits[index].size() > 0 ? tokenUnits[index] 
                : posUnits[index];
        case EXTENDED_POS_PLUS_PUNCTUATION:
            return tokenUnits[index].size() > 0 ? tokenUnits[index] 
                : extendedPOSUnits[index];
        case FULL_POS_PLUS_PUNCTUATION:
            return tokenUnits[index].size() > 0 ? tokenUnits[index] 
                : fullPOSUnits[index];
        default:
            return L"";
    }
}
