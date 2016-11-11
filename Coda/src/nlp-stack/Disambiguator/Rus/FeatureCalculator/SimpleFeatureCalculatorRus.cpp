#include "SimpleFeatureCalculatorRus.h"
#include "ConfigPreparerRus.h"

#include "FileReader.h"

namespace Disambiguation
{
    // const int FREQUENT_TOKENS_THRESHOLD = 300; // It is defined in ConfigPreparerRus
    const int UNTRANSLATED_TOKENS_THRESHOLD = 0;

    MapMapInt cut(MapMapInt& uncutted, int threshold)
    {
        MapMapInt cutted;
        for (MapMapInt::iterator iter1 = uncutted.begin(); iter1 != uncutted.end(); ++iter1)
        {
            wstring first = iter1->first;
            unordered_map<wstring, int>& second = iter1->second;
            int sum = 0;
            for (auto iter2 = second.begin(); iter2 != second.end(); ++iter2)
            {
                sum += iter2->second;
            }
            if (sum >= threshold)
            {
                cutted[first] = std::move(second);
            }
        }
        return cutted;
    }

    SimpleFeatureCalculatorRus::SimpleFeatureCalculatorRus(
        const string& tokenLabelFrequencyDistributionFile
        , const string& translationsForUntranslatedTokensFile
        , const string& abbreviatureFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<OpenCorporaToSynagrusConverter> converter)
        : dictionary(dictionary), converter(converter)
    {
        MapMapInt buffer0;
        FileManager::Read(tokenLabelFrequencyDistributionFile
            , &buffer0, L'\t', L',', L'~');
        tokenLabelFrequencyDistribution = cut(buffer0, FREQUENT_TOKENS_THRESHOLD);

        MapMapInt buffer1;
        FileManager::Read(translationsForUntranslatedTokensFile
            , &buffer1, L'\t', L',', L'~');
        translationsForUntranslatedTokens = cut(buffer1, UNTRANSLATED_TOKENS_THRESHOLD);

        FileManager::Read(abbreviatureFile, &abbreviatureTags, L'\t', L',');
    }

    vector<PredisambiguatedData> SimpleFeatureCalculatorRus::CalculateFeatures(
        const vector<Tokenization::Token>& sentence) const
    {
        vector<PredisambiguatedData> data;
        for (size_t tokenIndex = 0; tokenIndex < sentence.size(); ++tokenIndex)
        {
            data.push_back(calculateFeatures(sentence[tokenIndex]
                , tokenIndex + 1 == sentence.size()));
        }
        nameEntityFeatureCalculatorRus.Evaluate(&data);
        return data;
    }

    PredisambiguatedData SimpleFeatureCalculatorRus::calculateFeatures(
        const Tokenization::Token& token, bool isLastInSentence) const
    {
        PredisambiguatedData predisambigData(token);
        vector<wstring>& labels = predisambigData.features;
        // Evaluate abbreviatures
        bool evaluated = false;
        bool isAbbreviature = false;
        MapVec::const_iterator tagsForAbbreviature;
        wstring loweredToken = Tools::ToLower(token.content);
        if (token.punctuation.size() > 0)
        {
            wstring abbreviature = loweredToken + token.punctuation[0];
            tagsForAbbreviature = abbreviatureTags.find(abbreviature);
            isAbbreviature = tagsForAbbreviature != abbreviatureTags.end();
        }
        if (isAbbreviature) {
            evaluated = !isLastInSentence;
            labels.insert(labels.end()
                , tagsForAbbreviature->second.begin()
                , tagsForAbbreviature->second.end());
        // Evaluate frequent tokens
        } else if (tokenLabelFrequencyDistribution.find(loweredToken)
            != tokenLabelFrequencyDistribution.end()) {
                evaluated = true;
                labels.push_back(L"__" + loweredToken + L"__");
        // Evaluate untranslated tokens
        } else if (translationsForUntranslatedTokens.find(loweredToken)
            != translationsForUntranslatedTokens.end()) {
            evaluated = true;
            const unordered_map<wstring, int>& currentLabels
                = translationsForUntranslatedTokens.at(loweredToken);
            for (auto iter = currentLabels.begin(); iter != currentLabels.end(); ++iter) 
            {
                labels.push_back(iter->first);
            }
        // Evaluate numbers
        } else if (Tools::IsNumber(loweredToken)) {
            evaluated = true;
            labels.push_back(L"NUM");
        // Evaluate MISSED tokens
        } else if (loweredToken == L"missed") {
            evaluated = true;
            labels.push_back(L"UNKNOWN");
        // Evaluate foreign tokens
        } else if (Tools::ContainsLatin(loweredToken)) {
            evaluated = true;
            labels.push_back(L"NID");
        }
        // Evaluate normal tokens and set gram info
        shared_ptr<vector<shared_ptr<Morphology> > > morphology =
            std::make_shared<vector<shared_ptr<Morphology> > >();
        dictionary->getMorphology(loweredToken, morphology);

        for (size_t morphIndex = 0; morphIndex < morphology->size(); ++morphIndex)
        {
            vector<shared_ptr<wstring> >& opFeatures = (*morphology)[morphIndex]->descriptions;
            if (opFeatures.size() == 0)
            {
                continue;
            }
            wstring opLabel = *(opFeatures[0]);
            for (size_t iter = 1; iter < opFeatures.size(); ++iter)
            {
                opLabel += L" - " + *(opFeatures[iter]);
            }
            vector<wstring> syntagLabel = converter->PartialConvert(opLabel);
            if (!evaluated)
            {
                labels.insert(labels.end(), syntagLabel.begin(), syntagLabel.end());
            }
            for (size_t labelIndex = 0; labelIndex < syntagLabel.size(); ++labelIndex)
            {
                wstring label = translateInputToOutputProperty(
                    syntagLabel[labelIndex]);
                if (predisambigData.gramInfo.find(label)
                    == predisambigData.gramInfo.end())
                {
                    predisambigData.gramInfo[label]
                         = vector<shared_ptr<Morphology> >(1, (*morphology)[morphIndex]);
                } else {
                    predisambigData.gramInfo[label].push_back(
                        (*morphology)[morphIndex]);
                }
            }
        }
//        if (labels.size() == 0)
//        {
//            labels.insert(labels.end(), allNounTags.begin(), allNounTags.end()); // Transfered into name entity feature calculator
//        }
        return predisambigData;
    }

    wstring SimpleFeatureCalculatorRus::translateInputToOutputProperty(const wstring& prop) const
    {
        return Tools::Replace(Tools::Replace(prop, L"@PEREH", L""), L"@NEPEREH", L"");
    }
};
