#include "SyntaxModel.h"

#include <iostream>
#include <fstream>

#include "Tools.h"

using std::wifstream;
using std::wofstream;

namespace SyntaxParser
{
    SyntaxModel::SyntaxModel()
        : alphabet(new Alphabet())
    {
    }

    void SyntaxModel::ReadFromText(const string& file)
    {
        wifstream in(file);
        wstring str;
        getline(in, str);
        int size = stoi(str);
        parameters.resize(size);

        vector<wstring> splitted;
        for (int it = 0; it < size; ++it)
        {
            getline(in, str);
            splitted = Tools::Split(str, L"\t");
            alphabet->LookUpIndex_Adding(splitted[0]);
            parameters[it] = stod(splitted[1]);
        }
        in.close();
    }

    void SyntaxModel::WriteToText(const string& file)
    {
        wofstream out(file);
        out << parameters.size() << std::endl;
        for (size_t index = 0; index < parameters.size(); ++index)
        {
            out << alphabet->LookUpObject(index)
                << L"\t" << parameters[index] << std::endl;
        }
        out.close();
    }

    void SyntaxModel::Cut(double threshold)
    {
        shared_ptr<Alphabet> newAlphabet(new Alphabet());
        vector<double> newParameters;

        for (size_t index = 0; index < parameters.size(); ++index)
        {
            if (std::abs(parameters[index]) > threshold)
            {
                newAlphabet->LookUpIndex_Adding(
                    alphabet->LookUpObject(index));
                newParameters.push_back(parameters[index]);
            }
        }
        alphabet = newAlphabet;
        parameters = std::move(newParameters);
    }

    void SyntaxModel::ReadFromBinary(const string& file)
    {
        std::wcout << L"UNIMPLEMENTED YET";
    }

    void SyntaxModel::WriteToBinary(const string& file)
    {
        std::wcout << L"UNIMPLEMENTED YET";
    }
}
