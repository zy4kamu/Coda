#include "FileReader.h"
#include "Tools.h"

namespace FileManager
{

void ReadWithThreshold(const string& file
    , unordered_map<wstring, unordered_map<wstring, int> >& data
    , int countThreshold)
{
    data.clear();
    wifstream in(file);
	Tools::SetLocale(in);
    wstring line;
    set<wchar_t> splitChars;
    splitChars.insert(L'(');
    splitChars.insert(L')');
    while (getline(in, line))
    {
        vector<wstring> splitted = Tools::Split(line, L"\t");
        int count = std::stoi(splitted[1]);
        if (count < countThreshold) {
            continue;
        }
        data[splitted[0]] = unordered_map<wstring, int>();
        unordered_map<wstring, int>& current = data[splitted[0]];
        splitted = Tools::Split(splitted[2], splitChars);
        for (size_t splitIndex = 1; splitIndex < splitted.size(); splitIndex += 2)
        {
            vector<wstring> valueNum;
            valueNum = Tools::Split(splitted[splitIndex], L";");
            current[valueNum[0]] = std::stoi(valueNum[1]);
        }
    }
    in.close();
}

};
