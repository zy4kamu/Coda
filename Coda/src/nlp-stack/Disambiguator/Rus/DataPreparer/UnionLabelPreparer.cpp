#include "UnionLabelPreparerRus.h"

#include <algorithm>
#include <iterator>

#include "FileManager.h"
#include "Tools.h"

namespace Disambiguation
{

UnionLabelPreparerRus::UnionLabelPreparerRus()
    : dictionary(MorphologicalDictionary::CreateDictionary(BIN_DIC_DATA_PATH))
    , converter(string(DISAMBIGUATOR_CONFIG) 
    + "Translation of OpenCorpora properties to Synatgrus.txt")
{
}

void UnionLabelPreparerRus::PrepareUnitedPossibleStates()
{
    wifstream in(string(DISAMBIGUATOR_CONFIG) + "Token_Label.txt");
    SetLocale(in);
    wstring line;
    vector<wstring> splitted;
    int enumerator = 0;
    while (getline(in, line))
    {
        ++enumerator;
        wcout << enumerator << std::endl;
        // Get token and label
        wstring token;
        wstring label;
        Tools::Split(line, L"\t", splitted);
        if (splitted.size() != 2)
        {
            continue;
        }
        label = splitted[1];
        Tools::Split(wstring(splitted[0]), L"|", splitted);
        token = splitted[0];
        // Get gram info
        vector<GrammInfo> data = dictionary->getGrammInfo(token, 2);
        // Get labels
        vector<wstring> labels;
        for (size_t tokenIndex = 0; tokenIndex < data.size(); ++tokenIndex)
        {
            vector<wstring> opFeatures = data[tokenIndex].features;
            if (opFeatures.size() == 0)
            {
                continue;
            }
            wstring opLabel = opFeatures[0];
            for (size_t iter = 1; iter < opFeatures.size(); ++iter)
            {
                opLabel += L" - " + opFeatures[iter];
            }
            vector<wstring> syntagLabel = converter.PartialConvert(opLabel);
            labels.insert(labels.end(), syntagLabel.begin(), syntagLabel.end());
        }
        // Add united label
        wstring unitedLabel = getUnitedLabel(labels);
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
        Print(string(DISAMBIGUATOR_CONFIG) + "UnitedLabels.txt", unitedLabels);
    }
    in.close();
}

wstring UnionLabelPreparerRus::getUnitedLabel(
    vector<wstring>& labels)
{
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
}

};
