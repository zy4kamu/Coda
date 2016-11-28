#include "NameEntityFeatureCalculatorRus.h"
#include "Tools.h"

namespace Disambiguation
{

vector<wstring> makeAllNounTags(bool isNER)
{
    vector<wstring> allNounTags;

    vector<wstring> plurarity;
    plurarity.push_back(L"\u0415\u0414");//here was cyrrilic symbols: ЕД
    plurarity.push_back(L"\u041c\u041d");//here was cyrrilic symbols: МН

    vector<wstring> cases;
    cases.push_back(L"\u0418\u041c");//here was cyrrilic symbols: ИМ
    cases.push_back(L"\u0420\u041e\u0414");//here was cyrrilic symbols: РОД
    cases.push_back(L"\u0414\u0410\u0422");//here was cyrrilic symbols: ДАТ
    cases.push_back(L"\u0412\u0418\u041d");//here was cyrrilic symbols: ВИН
    cases.push_back(L"\u0422\u0412\u041e\u0420");//here was cyrrilic symbols: ТВОР
    cases.push_back(L"\u041f\u0420");//here was cyrrilic symbols: ПР

    vector<wstring> genders;
    genders.push_back(L"\u041c\u0423\u0416");//here was cyrrilic symbols: МУЖ
    genders.push_back(L"\u0416\u0415\u041d");//here was cyrrilic symbols: ЖЕН
    if (!isNER)
    {
        genders.push_back(L"\u0421\u0420\u0415\u0414");//here was cyrrilic symbols: СРЕД
    }

    vector<wstring> animacy;
    animacy.push_back(L"\u041e\u0414");//here was cyrrilic symbols: ОД
    if (!isNER)
    {
        animacy.push_back(L"\u041d\u0415\u041e\u0414");//here was cyrrilic symbols: НЕОД
    }

    for (size_t plurarityIndex = 0; plurarityIndex < plurarity.size(); ++plurarityIndex)
    {
        for (size_t caseIndex = 0; caseIndex < cases.size(); ++caseIndex)
        {
            for (size_t genderIndex = 0; genderIndex < genders.size(); ++genderIndex)
            {
                for (size_t animacyIndex = 0; animacyIndex < animacy.size(); ++animacyIndex)
                {
                    wstring toAdd = L"S@";
                    toAdd += plurarity[plurarityIndex];
                    toAdd += L"@";
                    toAdd += genders[genderIndex];
                    toAdd += L"@";
                    toAdd += cases[caseIndex];
                    toAdd += L"@";
                    toAdd += animacy[animacyIndex];
                    allNounTags.emplace_back(std::move(toAdd));
                }
            }
        }
    }

    return allNounTags;
}

const vector<wstring> NameEntityFeatureCalculatorRus::allNounTags = makeAllNounTags(false);
const vector<wstring> NameEntityFeatureCalculatorRus::allNERtags = makeAllNounTags(true);

bool isFullName(const PredisambiguatedData& data)
{
    return Tools::IsUpper(data.content[0]);
}

bool isAbbreviaturedName(const PredisambiguatedData& data)
{
    return data.content.size() == 1 &&
        Tools::IsUpper(data.content[0]) &&
        data.punctuation.size() > 0 &&
        data.punctuation[0] == L".";
}


void NameEntityFeatureCalculatorRus::Evaluate(
    vector<PredisambiguatedData>* predisambiguated) const
{
    size_t size = predisambiguated->size();
    vector<bool> isAbbreviaturedNames(size);
    vector<bool> isFullNames(size);
    for (size_t tokenIndex =  0; tokenIndex < size; ++tokenIndex)
    {
        PredisambiguatedData& current = (*predisambiguated)[tokenIndex];
        isAbbreviaturedNames[tokenIndex] = isAbbreviaturedName(current);
        isFullNames[tokenIndex] = isFullName(current);
    }
    for (size_t tokenIndex =  0; tokenIndex < size; ++tokenIndex)
    {
        PredisambiguatedData& current = (*predisambiguated)[tokenIndex];
        if (current.features.size() == 0)
        {
            if (isAbbreviaturedNames[tokenIndex]) {
                current.features = allNERtags;
            } else if (tokenIndex > 0 &&
               isFullNames[tokenIndex] &&
               isFullNames[tokenIndex - 1]) {
                current.features = allNERtags;
            } else if (tokenIndex + 1 < size &&
                isFullNames[tokenIndex] &&
                isFullNames[tokenIndex + 1]) {
                current.features = allNERtags;
            } else {
                current.features = allNounTags;
            }
        } else if (isAbbreviaturedNames[tokenIndex]) {
            if (tokenIndex > 0 && isFullNames[tokenIndex - 1]) {
                current.features = allNERtags;
            } else if (tokenIndex + 1 < size &&
                isFullNames[tokenIndex + 1]) {
                current.features = allNERtags;
            }
        }
    }
}

}
