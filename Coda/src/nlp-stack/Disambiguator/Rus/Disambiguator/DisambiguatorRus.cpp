#include "DisambiguatorRus.h"

namespace Disambiguation
{
    DisambiguatorRus::DisambiguatorRus(
        const string& crfFile
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , bool isBinary
        , bool useCyrillic
        , bool useTransitivity)
        : Disambiguator(
            crfFile
            , dictionary
            , featureCalculator
            , chainTransformer
            , possibleStateFinder
            , Tools::Language::RU
            , isBinary)
        , useCyrillic(useCyrillic)
        , useTransitivity(useTransitivity)
    {
    }

    DisambiguatorRus::DisambiguatorRus(
        shared_ptr<CRF> model
        , shared_ptr<MorphologicalDictionary> dictionary
        , shared_ptr<IFeatureCalculator> featureCalculator
        , shared_ptr<IChainTransformer> chainTransformer
        , shared_ptr<IPossibleStateFinder> possibleStateFinder
        , bool useCyrillic
        , bool useTransitivity)
        : Disambiguator(
            model
            , dictionary
            , featureCalculator
            , chainTransformer
            , possibleStateFinder
            , Tools::Language::RU)
        , useCyrillic(useCyrillic)
        , useTransitivity(useTransitivity)
    {
    }

    Tools::Language DisambiguatorRus::GetLanguage() const
    {
        return Tools::Language::RU;
    }

    bool isVerb(const GrammInfo& gramInfo)
    {
        const vector<wstring>& features = gramInfo.features;
        if (features.size() == 0)
        {
            return false;
        }
        return Tools::StartsWith(features[0], L"\u0433\u043b\u0430\u0433\u043e\u043b")//here was cyrrilic symbols: глагол
            || Tools::StartsWith(features[0], L"\u043f\u0440\u0438\u0447\u0430\u0441\u0442\u0438\u0435");//here was cyrrilic symbols: причастие
    }

    bool isTransitive(const GrammInfo& gramInfo)
    {
        const vector<wstring>& features = gramInfo.features;
        for (size_t iter = 0; iter < features.size(); ++iter)
        {
            if (features[iter] == L"\u043f\u0435\u0440\u0435\u0445\u043e\u0434\u043d\u044b\u0439")//here was cyrrilic symbols: переходный
            {
                return true;
            }
        }
        return false;
    }

    void DisambiguatorRus::applyPostprocessRules(
        wstring* label
        , const GrammInfo& gramInfo)
    {
        if (useTransitivity && isVerb(gramInfo))
        {
            if (useCyrillic) {
                if (isTransitive(gramInfo)) {
                    *label += L"@\u041f\u0415\u0420\u0415\u0425";//here was cyrrilic symbols: ПЕРЕХ
                } else {
                    *label += L"@\u041d\u0415\u041f\u0415\u0420\u0415\u0425";//here was cyrrilic symbols: НЕПЕРЕХ
                }
            } else {
                if (isTransitive(gramInfo)) {
                    *label += L"@trans";
                } else {
                    *label += L" - nottrans";
                }
            }
        }
    }
}

