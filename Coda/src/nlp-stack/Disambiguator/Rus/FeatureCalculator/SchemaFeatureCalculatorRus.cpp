#include "SchemaFeatureCalculatorRus.h"

#include <algorithm>
#include <iterator>

#include "Tools.h"

namespace Disambiguation
{

wstring GetUnitedLabel(
    const vector<wstring>& _labels)
{
    vector<wstring> labels = _labels;
    std::sort(labels.begin(), labels.end());
    vector<wstring> unique;
    std::unique_copy(labels.begin(), labels.end(), std::back_inserter(unique));
    wstring unitedString;
    for (size_t labelIndex = 0; labelIndex < unique.size(); ++labelIndex)
    {
        unitedString += unique[labelIndex] + L"_";
    }
    if (unitedString.size() != 0) {
        unitedString = unitedString.substr(0, unitedString.size() - 1);
    } 
    return unitedString;
};

wstring GetUnitedPOS(
    const vector<wstring>& labels)
{
    vector<wstring> pos;
    for (size_t labelIndex = 0; labelIndex < labels.size(); ++labelIndex)
    {
        size_t foundSplitter = labels[labelIndex].find(L'@');
        if (foundSplitter == std::wstring::npos) {
            pos.emplace_back(labels[labelIndex]); 
        } else {
            pos.emplace_back(labels[labelIndex].substr(0, foundSplitter));
        }
    }
    std::sort(pos.begin(), pos.end());
    vector<wstring> unique;
    std::unique_copy(pos.begin(), pos.end(), std::back_inserter(unique));
    wstring unitedString;
    for (size_t labelIndex = 0; labelIndex < unique.size(); ++labelIndex)
    {
        unitedString += unique[labelIndex] + L"_";
    }
    if (unitedString.size() != 0) {
        unitedString = unitedString.substr(0, unitedString.size() - 1);
    } 
    return unitedString;
};

wstring ConvertToWideString(const vector<wstring>& punctuation)
{
    wstring punctuations;
    for (size_t punctIndex = 0; punctIndex < punctuation.size()
        ; ++punctIndex)
    {
        punctuations += Tools::PunctuationToString(
            punctuation[punctIndex]);
    }
    return punctuations;
}

Schema::Schema(const vector<int>& relativeDistances,
    const vector<SchemaType>& schemaTypes)
    : relativeDistances(relativeDistances)
    , schemaTypes(schemaTypes)
{
}

Schema::Schema(vector<int>&& relativeDistances,
    vector<SchemaType>&& schemaTypes)
    : relativeDistances(std::move(relativeDistances))
    , schemaTypes(std::move(schemaTypes))
{
}

SchemaFeatureCalculatorRus::SchemaFeatureCalculatorRus(
    shared_ptr<IFeatureCalculator> simpleFeatureCalculatorRus)
    : simpleFeatureCalculatorRus(simpleFeatureCalculatorRus)
{
    initializeSchemas();
}

void SchemaFeatureCalculatorRus::initializeSchemas()
{
    vector<int> distances0(1, 0);
    vector<SchemaType> schemaTypes0(1, UNITED_MORPHOLOGY);
    schemas.push_back(Schema(std::move(distances0), std::move(schemaTypes0)));

    vector<int> distances1;
    distances1.push_back(-1);
    distances1.push_back(0);
    distances1.push_back(1);
    vector<SchemaType> schemaTypes1(3, UNITED_MORPHOLOGY_PLUS_PUNCTUATION);
    schemas.push_back(Schema(std::move(distances1), std::move(schemaTypes1)));

    vector<int> distances2;
    distances2.push_back(-2);
    distances2.push_back(-1);
    distances2.push_back(0);
    distances2.push_back(1);
    distances2.push_back(2);
    vector<SchemaType> schemaTypes2;
    schemaTypes2.push_back(UNITED_POS_PLUS_PUNCTUATION);
    schemaTypes2.push_back(UNITED_POS_PLUS_PUNCTUATION);
    schemaTypes2.push_back(UNITED_MORPHOLOGY);
    schemaTypes2.push_back(UNITED_POS_PLUS_PUNCTUATION);
    schemaTypes2.push_back(UNITED_POS_PLUS_PUNCTUATION);
    schemas.push_back(Schema(std::move(distances2), std::move(schemaTypes2)));
}

vector<PredisambiguatedData> SchemaFeatureCalculatorRus::CalculateFeatures(
    const vector<Tokenization::Token>& sentence) const
{
    vector<PredisambiguatedData> simpleData 
        = simpleFeatureCalculatorRus->CalculateFeatures(sentence);
    map<SchemaType, vector<wstring> > schemaFeatures;
    calculateSchemaFeatures(simpleData, &schemaFeatures);
    for (size_t nodeIndex = 0; nodeIndex < simpleData.size(); ++nodeIndex)
    {
        for (size_t schemaIndex = 0; schemaIndex < schemas.size(); ++schemaIndex)
        {
            simpleData[nodeIndex].features.push_back(
                calculateSchema(nodeIndex
                    , schemaFeatures, schemas[schemaIndex]
                    , static_cast<int>(sentence.size())));
        }
    }
    return simpleData;
}

void SchemaFeatureCalculatorRus::calculateSchemaFeatures(
    const vector<PredisambiguatedData>& data
    , map<SchemaType, vector<wstring> >* schemaFeatures) const
{
    // Initializing
    for (int typeIndex = TOKEN_PLUS_PUNCUATION; typeIndex <= EMPTY; ++typeIndex)
    {
        SchemaType schemaType = static_cast<SchemaType>(typeIndex);
        (*schemaFeatures)[schemaType] = vector<wstring>(data.size());
    }
    // Run over tokens in sentence
    for (size_t nodeIndex = 0; nodeIndex < data.size(); ++nodeIndex)
    {
        // Run over schema type
        for (int typeIndex = TOKEN_PLUS_PUNCUATION; typeIndex <= EMPTY; ++typeIndex)
        {
            SchemaType schemaType = static_cast<SchemaType>(typeIndex);
            vector<wstring> featuresToAdd;
            switch(schemaType)
            {
            case TOKEN_PLUS_PUNCUATION:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = data[nodeIndex].content
                    + ConvertToWideString(data[nodeIndex].punctuation);
                break;
            case TOKEN:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = data[nodeIndex].content;
                break;
            case UNITED_MORPHOLOGY_PLUS_PUNCTUATION:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = GetUnitedLabel(data[nodeIndex].features)
                    + ConvertToWideString(data[nodeIndex].punctuation);
                break;
            case UNITED_MORPHOLOGY:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = GetUnitedLabel(data[nodeIndex].features);
                break;
            case UNITED_POS_PLUS_PUNCTUATION:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = GetUnitedPOS(data[nodeIndex].features)
                    + ConvertToWideString(data[nodeIndex].punctuation);
                break;
            case UNITED_POS:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = GetUnitedPOS(data[nodeIndex].features);
                break;
            case PUNCTUATION:
                (*schemaFeatures)[schemaType][nodeIndex] =
                    ConvertToWideString(data[nodeIndex].punctuation);
                break;
            case EMPTY:
                (*schemaFeatures)[schemaType][nodeIndex] 
                    = L"";
                break;
            }
        }
    }
}

wstring SchemaFeatureCalculatorRus::calculateSchema(int nodeIndex
    , const map<SchemaType, vector<wstring> >& schemaFeatures
    , const Schema& schema, int sentenceLength) const
{
    wstring schemaFeature = L"schema_";
    for (size_t schemaIndex = 0; schemaIndex < schema.relativeDistances.size()
        ; ++schemaIndex)
    {
        int relativeDistance = schema.relativeDistances[schemaIndex];
        SchemaType schemaType = schema.schemaTypes[schemaIndex];
        int realIndex = static_cast<int>(nodeIndex) + relativeDistance;
        if (realIndex >= 0 && realIndex < sentenceLength) {
            schemaFeature += schemaFeatures.at(schemaType)[realIndex] + L"~";
        } else {
            schemaFeature += L"*~";
        }
    }
    return schemaFeature;
}

};
