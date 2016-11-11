#ifndef CONFIGURATION_FILES_PREPARER
#define CONFIGURATION_FILES_PREPARER

#include <vector>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <iterator>

#include "Chain.h"
#include "MorphologicalDictionary.h"
#include "DisambiguatorSettingsRus.h"
#include "FileReader.h"
#include "FileWriter.h"
#include "IDisambiguatorSettings.h"
#include "OpenCorporaToSynagrusConverter.h"
#include "SchemaFeatureCalculatorRus.h"
#include "Token.h"
#include "Tools.h"

using std::vector;
using std::wstring;
using std::map;
using std::set;
using std::string;

namespace Disambiguation
{

const int FREQUENT_TOKENS_THRESHOLD = 300;

struct SentenceItem : public Tokenization::Token
{
    wstring label;
    vector<wstring> possibleStates;
};


class ConfigPreparerRus
{
typedef vector<SentenceItem> Sentence;
typedef unordered_map<wstring, unordered_map<wstring, int> > MapMapInt;
typedef TagsetConverting::OpenCorporaToSynagrusConverter ConverterType;

public:
    ConfigPreparerRus(
        shared_ptr<IDisambiguatorSettings> settings);
    void PrepareConfigurationFiles();
private:
    MapMapInt tokenLabelFrequencyDistribution;
    map<wstring, int> unitedLabels;
    map<wstring, int> unitedPOSs;
    MapMapInt translationsForUntranslatedTokens;
    vector<Sentence> sentences;
    shared_ptr<MorphologicalDictionary> dictionary;
    shared_ptr<ConverterType> converter;
    shared_ptr<DataFunctor> trainingSetFunctor;

    void readSentences();
    void findPossibleStates();
    void getTokenFrequency();
    void getUnitedLabelsFrequency();
    void getUnitedPOSFrequency();
    void getTranslationsForUntranslatedTokens();
    void prepareFileWithTranslationsForFeatureCalculatorRus();

    wstring translateInputToOutputProperty(
        const wstring& prop) const;
};

ConfigPreparerRus::ConfigPreparerRus(
    shared_ptr<IDisambiguatorSettings> settings)
    : dictionary(settings->GetDictionary())
    , converter(dynamic_pointer_cast<DisambiguatorSettingsRus>(settings)->GetOpenCorporaToSynTagRusConverter())
    , trainingSetFunctor(settings->GetTrainingFunctor())
{
}

void ConfigPreparerRus::PrepareConfigurationFiles()
{
    wcout << L"Reading sentences..." << std::endl;
    readSentences();
    wcout << L"\nFinding possible states using dictionary..." << std::endl;
    findPossibleStates();
    wcout << L"\nFinding token frequency..." << std::endl;
    getTokenFrequency();
    wcout << L"\nGet united labels frequency..." << std::endl;
    getUnitedLabelsFrequency();
    wcout << L"\nGet united POS frequency..." << std::endl;
    getUnitedPOSFrequency();
    wcout << L"\nGet translations for untranslated sentences..." << std::endl;
    getTranslationsForUntranslatedTokens();
    wcout << L"\nPrepare file for feature calculator..." << std::endl;
    prepareFileWithTranslationsForFeatureCalculatorRus();
}

void ConfigPreparerRus::readSentences()
{
    sentences.clear();
    wifstream in(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/Token_Label.txt");
	Tools::SetLocale(in);
    wstring line;
    vector<wstring> splitted;
    int enumerator = 0;
    int sentenceEnumerator = 0;
    sentences.emplace_back(Sentence());
    while (getline(in, line))
    {
        ++enumerator;
        if (enumerator % 100 == 0) 
        {
            wcout << L"\r" << enumerator;
        }
        splitted = Tools::Split(line, L"\t");
        if (splitted.size() != 2)
        {
            ++sentenceEnumerator;
            if (!(*trainingSetFunctor)(sentenceEnumerator))
            {
                continue;
            } else {
                sentences.emplace_back(Sentence());
                continue;
            }
        } else if (!(*trainingSetFunctor)(sentenceEnumerator)) {
            continue;
        }
        SentenceItem token;
        token.label = splitted[1];
        splitted = Tools::Split(wstring(splitted[0]), L"|");
        token.content = splitted[0];
        token.punctuation.push_back(splitted.size() > 1 ? splitted[1] : L"");
        sentences.back().push_back(token);
    }
    in.close();
}

void ConfigPreparerRus::findPossibleStates()
{
    size_t enumerator = 0;
    // Iterate over sentences
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        Sentence& sentence = sentences[sentenceIndex];
        // Iterate over tokens in sentence
        for (size_t tokenIndex = 0; tokenIndex < sentence.size(); ++tokenIndex)
        {
            ++enumerator;
            if (enumerator % 100 == 0)
            {
                wcout << L"\r" << enumerator;
            }
            SentenceItem& sentenceItem = sentence[tokenIndex];
            wstring& token = sentenceItem.content;
            vector<wstring>& labels = sentenceItem.possibleStates;
            // Create labels
            if (Tools::IsNumber(token))
            {
                labels.push_back(L"NUM");
            } else if (Tools::ContainsLatin(token)) {
                labels.push_back(L"NID");
            } else {
                shared_ptr<vector<shared_ptr<Morphology> > > morphology =
                    std::make_shared<vector<shared_ptr<Morphology> > >();
                dictionary->getMorphology(token, morphology);
                for (size_t morphIndex = 0; morphIndex < morphology->size(); ++morphIndex)
                {
                    vector<shared_ptr<wstring> >& opFeatures = (*morphology)[morphIndex]->features;
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
                    labels.insert(labels.end(), syntagLabel.begin(), syntagLabel.end());
                }
            }
            if (labels.size() == 0) {
                labels.push_back(L"UNKNOWN");
            }
            // Unique labels
            vector<wstring> uniqueLabels = labels;
            std::sort(uniqueLabels.begin(), uniqueLabels.end());
            labels.clear();
            std::unique_copy(uniqueLabels.begin(), uniqueLabels.end(), std::back_inserter(labels));
        }
    }
}

void ConfigPreparerRus::getTokenFrequency()
{
    size_t enumerator = 0;
    // Iterate over sentences
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        Sentence& sentence = sentences[sentenceIndex];
        // Iterate over tokens in sentence
        for (size_t tokenIndex = 0; tokenIndex < sentence.size(); ++tokenIndex)
        {
            ++enumerator;
            if (enumerator % 100 == 0)
            {
                wcout << L"\r" << enumerator;
            }
            const SentenceItem& sentenceItem = sentence[tokenIndex];
            const wstring& token = sentenceItem.content;
            const wstring& label = sentenceItem.label;
            if (tokenLabelFrequencyDistribution.find(token) 
                == tokenLabelFrequencyDistribution.end())
            {
                tokenLabelFrequencyDistribution[token] = unordered_map<wstring, int>();
            }
            unordered_map<wstring, int>& currentDistribution 
                = tokenLabelFrequencyDistribution[token];
            if (currentDistribution.find(label) != currentDistribution.end()) {
                ++currentDistribution[label];
            } else {
                currentDistribution[label] = 1;
            }
        }
    }
    FileManager::Write(string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + "RU/Token_LabelFrequencyDistribution.txt"
        , tokenLabelFrequencyDistribution, L'\t', L',', L'~');
}

void ConfigPreparerRus::getUnitedLabelsFrequency()
{
    size_t enumerator = 0;
    // Iterate over sentences
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        Sentence& sentence = sentences[sentenceIndex];
        // Iterate over tokens in sentence
        for (size_t tokenIndex = 0; tokenIndex < sentence.size(); ++tokenIndex)
        {
            ++enumerator;
            if (enumerator % 100 == 0)
            {
                wcout << L"\r" << enumerator;
            }
            const SentenceItem& sentenceItem = sentence[tokenIndex];
            const vector<wstring>& possibleLabels = sentenceItem.possibleStates;
            const wstring& label = sentenceItem.label;
            wstring unitedLabel = GetUnitedLabel(possibleLabels);
            if (unitedLabel.size() == 0)
            {
                unitedLabel = label;
            }
            if (unitedLabels.find(unitedLabel) != unitedLabels.end())
            {
                ++unitedLabels[unitedLabel];
            } else {
                unitedLabels[unitedLabel] = 1;
            }
        }
    }
    FileManager::Write(
        string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + "RU/UnitedLabels.csv", unitedLabels, L',');
}

void ConfigPreparerRus::getUnitedPOSFrequency()
{
    size_t enumerator = 0;
    // Iterate over sentences
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        Sentence& sentence = sentences[sentenceIndex];
        // Iterate over tokens in sentence
        for (size_t tokenIndex = 0; tokenIndex < sentence.size(); ++tokenIndex)
        {
            ++enumerator;
            if (enumerator % 100 == 0)
            {
                wcout << L"\r" << enumerator;
            }
            const SentenceItem& sentenceItem = sentence[tokenIndex];
            const vector<wstring>& possibleLabels = sentenceItem.possibleStates;
            wstring unitedLabel = GetUnitedPOS(possibleLabels);
            if (unitedPOSs.find(unitedLabel) != unitedPOSs.end())
            {
                ++unitedPOSs[unitedLabel];
            } else {
                unitedPOSs[unitedLabel] = 1;
            }
        }
    }
    FileManager::Write(
        string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + "RU/UnitedPOS.csv", unitedPOSs, L',');
}

void ConfigPreparerRus::getTranslationsForUntranslatedTokens()
{
    size_t enumerator = 0;
    // Construct untranslated tokens
    unordered_set<wstring> untranslatedTokens;
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        for (size_t tokenIndex = 0; tokenIndex < sentences[sentenceIndex].size()
            ; ++tokenIndex)
        {
            ++enumerator;
            if (enumerator % 100 == 0)
            {
                wcout << L"\r" << enumerator;
            }
            const SentenceItem& item = sentences[sentenceIndex][tokenIndex];
            const wstring& token = item.content;
            const wstring& label = item.label;
            const vector<wstring>& possibleStates = item.possibleStates;
            bool found = false;
            for (size_t stateIndex = 0; stateIndex < possibleStates.size(); ++stateIndex)
            {
                if (translateInputToOutputProperty(possibleStates[stateIndex]) == label)
                {
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                untranslatedTokens.insert(token);
            }
        }
    }
    // Fill untranslated tokens
    for (size_t sentenceIndex = 0; sentenceIndex < sentences.size()
        ; ++sentenceIndex)
    {
        for (size_t tokenIndex = 0; tokenIndex < sentences[sentenceIndex].size()
            ; ++tokenIndex)
        {
            const SentenceItem& item = sentences[sentenceIndex][tokenIndex];
            const wstring& token = item.content;
            const wstring& label = item.label;
            if (untranslatedTokens.find(token) == untranslatedTokens.end())
            {
                continue;
            }
            if (translationsForUntranslatedTokens.find(token) 
                == translationsForUntranslatedTokens.end())
            {
                translationsForUntranslatedTokens[token] = unordered_map<wstring, int>();
            }
            unordered_map<wstring, int>& currentDistribution 
                = translationsForUntranslatedTokens[token];
            if (currentDistribution.find(label) != currentDistribution.end()) {
                ++currentDistribution[label];
            } else {
                currentDistribution[label] = 1;
            }
        }
    }
    FileManager::Write(
        string(DISAMBIGUATOR_CONFIG_FOLDER_STRING) + "RU/UntranslatedTokens.txt"
        , translationsForUntranslatedTokens, L'\t', L',', L'~');
}

void ConfigPreparerRus::prepareFileWithTranslationsForFeatureCalculatorRus()
{
    wofstream out(
        string(DISAMBIGUATOR_CONFIG_FOLDER_STRING)
        + "RU/FileTranslationsForFeatureCalculatorRus.txt");
	Tools::SetLocale(out);
    for (auto iter = tokenLabelFrequencyDistribution.begin()
         ; iter != tokenLabelFrequencyDistribution.end(); ++iter)
    {
        const wstring& token = iter->first;
        const unordered_map<wstring, int>& current = iter->second;
        int frequency = 0;
        for (auto iter1 = current.begin(); iter1 != current.end(); ++iter1)
        {
            frequency += iter1->second;
        }
        if (frequency >= FREQUENT_TOKENS_THRESHOLD)
        {
            out << token << L"\t";
            auto translationIterator = current.begin();
            while (translationIterator != current.end())
            {
                if (static_cast<double>(translationIterator->second)
                    / static_cast<double>(frequency) > 0.001)
                {
                    out << translationIterator->first;
                    ++translationIterator;
                    if (translationIterator != current.end())
                    {
                        out << L",";
                    }
                } else {
                    ++translationIterator;
                }
            }
            out << std::endl;
        }
    }
    out.close();
}

wstring ConfigPreparerRus::translateInputToOutputProperty(
    const wstring& prop) const
{
    return Tools::Replace(Tools::Replace(prop, L"@PEREH", L""), L"@NEPEREH", L"");
}

}

#endif // CONFIGURATION_FILES_PREPARER
