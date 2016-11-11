#include "FeatureCalculatorEn.h"
#include "FileReader.h"
#include "Tools.h"
#include <cwctype>

using std::wstring;

namespace Disambiguation
{
    FeatureCalculatorEn::FeatureCalculatorEn(
        shared_ptr<MorphologicalDictionary> dictionary
        , const string& possibleTagsFile)
        : dictionary(dictionary)
    {
        FileManager::Read(possibleTagsFile, &possibleTags);
    }

    vector<PredisambiguatedData> FeatureCalculatorEn::CalculateFeatures(
            const vector<Tokenization::Token>& sentence) const
    {
        vector<PredisambiguatedData> result;

        for( size_t i = 0; i < sentence.size(); i++)
        {
            PredisambiguatedData tokenResult(sentence[i]);

            wstring capFeature = capitalizedFeature(sentence[i].content);
            if(!capFeature.empty()) {
                tokenResult.features.emplace_back(std::move(capFeature));
            }

            wstring word = Tools::ToLower(sentence[i].content);

            // WORD
            tokenResult.features.push_back(word);

            // DICTIONARY features
            vector<wstring> dictFeatures = dictionaryFeatures(sentence[i].content, &tokenResult);

            // UNION Features
            std::sort(dictFeatures.begin(), dictFeatures.end());
            wstring unionFeature = L"UNION_";
            for (wstring& str : dictFeatures)
            {
                unionFeature += str + L"_";
            }
            tokenResult.features.emplace_back(std::move(unionFeature));

            // ADDING DICTIONARY FEATURES
            tokenResult.features.insert(tokenResult.features.end(),
                std::make_move_iterator(dictFeatures.begin()),
                std::make_move_iterator(dictFeatures.end()));

            // SUFFIX
            for (size_t suffixLength = 1; suffixLength < std::min(word.size(), (size_t)4); ++suffixLength)
            {
                wstring suffix = suffixFeature(word, suffixLength);
                if(suffix != L"") {
                    tokenResult.features.emplace_back(std::move(suffix));
                }
            }

            // PREFIX
            for (size_t prefixLength = 1; prefixLength < std::min(word.size(), (size_t)4); ++prefixLength)
            {
                wstring prefix = prefixFeature(word, prefixLength);
                if(prefix != L"") {
                    tokenResult.features.emplace_back(std::move(prefix));
                }
            }

            // DOT
            if (word.size() > 1 && Tools::ContainsSubstring(word, L"."))
            {
                tokenResult.features.emplace_back(L"CONTAINS_DOT");
            }

            // HYFFEN
            if (word.size() > 1 && Tools::ContainsSubstring(word, L"-"))
            {
                tokenResult.features.emplace_back(L"CONTAINS_HYFFEN");
            }

            // UPPER
            if (word.size() > 2 && Tools::ToUpper(word) == word)
            {
                tokenResult.features.emplace_back(L"IS_UPPER");
            }

            result.emplace_back(std::move(tokenResult));
        }
        return result;
    }

    wstring FeatureCalculatorEn::capitalizedFeature(const wstring& word) const
    {
        const wstring capFeatureString = L"CAP";

        if(word.empty()) {
            return L"";
        }
        if(std::iswupper(word[0])) {
            return capFeatureString;
        }
        return L"";
    }

    vector<wstring> FeatureCalculatorEn::dictionaryFeatures(
        const wstring& inputWord
        , PredisambiguatedData* predisambiguated) const
    {
        if (inputWord.compare(L"NUMBER") == 0 ) {
            return { L"CD", L"JJ", L"LS", L"NN", L"NNP", L"NNS", L"VBN", L"COMMA" };
        } else if (inputWord.compare(L"COMMA") == 0 || inputWord.compare(L",") == 0) {
            return { L"COMMA" };
        }

        vector<wstring> features;

        shared_ptr<vector<shared_ptr<Morphology> > > morphology =
            std::make_shared<vector<shared_ptr<Morphology> > >();
        dictionary->getMorphology(inputWord, morphology);

        wstring word = Tools::ToLower(inputWord);
        if (morphology->size() > 0) {
            for (size_t morphIndex = 0; morphIndex < morphology->size(); ++morphIndex)
            {
                vector<shared_ptr<wstring> >& currentMorphology = (*morphology)[morphIndex]->features;
                if (currentMorphology.size() == 0)
                {
                    continue;
                }
                const wstring& label = *(currentMorphology[0]);
                features.push_back(label);
                if (predisambiguated->gramInfo.find(label)
                    == predisambiguated->gramInfo.end())
                {
                    predisambiguated->gramInfo[label]
                         = vector<shared_ptr<Morphology> >(1, (*morphology)[morphIndex]);
                } else {
                    predisambiguated->gramInfo[label].push_back(
                        (*morphology)[morphIndex]);
                }
            }
        } else {
            features.emplace_back(L"UNKN");
            features.insert(features.end(), possibleTags.begin(), possibleTags.end());
        }
        return features;
    }

    wstring FeatureCalculatorEn::suffixFeature(const wstring& word, size_t suffixLength) const
    {
        if(word == L"comma" || word == L"number" ) {
            return L"";
        }
        wstring result;
        size_t index = std::max( 0, int(word.length()) - int(suffixLength));
        result = L"s";
        result += Tools::ConvertIntToWstring(suffixLength);
        result += L"=";
        result += word.substr(index);
        return result;
    }

    wstring FeatureCalculatorEn::prefixFeature(const wstring& word, size_t size) const
    {
        if(word == L"comma" || word == L"number" ) {
            return L"";
        }
        wstring result = L"p";
        result += Tools::ConvertIntToWstring(size);
        result += L"=";
        result += word.substr(0, size);
        return result;
    }
}


