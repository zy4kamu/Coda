#include "BinaryIO.h"

#include <ios>

namespace Tools
{
void WriteStringArray_Binary(const string& fname, const vector<string>& data)
{
    std::ofstream outfile(fname.c_str(), std::ios::binary);
    int length = data.size();
    outfile.write((char*)&length, sizeof(int));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int size = data[i].size();
        outfile.write((char*)&size, sizeof(int));
        outfile.write(data[i].c_str(), sizeof(char) * data[i].length());
    }
}

void WriteStringArray_Binary(ofstream& out, const vector<string>& data)
{
    int length = data.size();
    out.write((char*)&length, sizeof(int));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int size = data[i].size();
        out.write((char*)&size, sizeof(int));
        out.write(data[i].c_str(), sizeof(char) * data[i].length());
    }
}

void WriteWStringArray_Binary(const string &fname, const vector<wstring>& data)
{
    std::ofstream outfile(fname.c_str(), std::ios::binary);
    int length = data.size();
    outfile.write((char*)&length, sizeof(int));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int size = data[i].size();
        outfile.write((char*)&size, sizeof(int));
        outfile.write((char*)data[i].c_str(), sizeof(wchar_t) * data[i].length());
    }
}

void WriteWStringArray_Binary(ofstream& out, const vector<wstring>& data)
{
    int length = data.size();
    out.write((char*)&length, sizeof(int));
    for (size_t i = 0; i < data.size(); ++i)
    {
        int size = data[i].size();
        out.write((char*)&size, sizeof(int));
        out.write((char*)data[i].c_str(), sizeof(wchar_t) * data[i].length());
    }
}

void WriteString_Binary(ofstream& i_out, const string& i_string)
{
    int size = (int)i_string.size();
    i_out.write((char*)&size, 4);
    i_out.write(i_string.c_str(), 1 * i_string.length());
}

void WriteWString_Binary(ofstream& i_out, const wstring& i_wstring, locale loc)
{
    string str;
    Tools::ConvertWstringToUTF8(i_wstring,str,loc);
    WriteString_Binary(i_out,str);
}

void ReadStringArray(const string &fname, vector<string>& data)
{
    data.clear();
    ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
    int length;
    in.read((char*)&length, sizeof(int));
    data.resize(length);
    for (vector<string>::iterator it = data.begin(); it != data.end(); ++it)
    {
        int size;
        in.read((char*)&size, sizeof(int));
        char* buffer = new char[size];
        in.read(buffer, sizeof(char) * size);
        Convert(buffer, size, *it);
        delete [] buffer;
    }
}

void ReadStringArray(ifstream& in, vector<string>& data)
{
    data.clear();
    int length;
    in.read((char*)&length, sizeof(int));
    data.resize(length);
    for (vector<string>::iterator it = data.begin(); it != data.end(); ++it)
    {
        int size;
        in.read((char*)&size, sizeof(int));
        char* buffer = new char[size];
        in.read(buffer, sizeof(char) * size);
        Convert(buffer, size, *it);
        delete [] buffer;
    }
}

void ReadWStringArray(const string &fname, vector<wstring>& data)
{
    data.clear();
    ifstream in(fname.c_str(), std::ios::in | std::ios::binary);
    int length;
    in.read((char*)&length, sizeof(int));
    data.resize(length);
    for (vector<wstring>::iterator it = data.begin(); it != data.end(); ++it)
    {
        int size;
        in.read((char*)&size, sizeof(int));
        wchar_t* buffer = new wchar_t[size*2];
        in.read((char*)buffer, sizeof(wchar_t)*2 * size);
        Convert(buffer, size, *it);
        delete [] buffer;
    }
}

void ReadWStringArray(ifstream& in, vector<wstring>& data)
{
    data.clear();
    int length;
    in.read((char*)&length, sizeof(int));
    data.resize(length);
    for (vector<wstring>::iterator it = data.begin(); it != data.end(); ++it)
    {
        int size;
        in.read((char*)&size, sizeof(int));
#ifdef MSVC
		wchar_t* buffer = new wchar_t[size*2];
        in.read((char*)buffer, sizeof(wchar_t)*2 * size);
#else
		wchar_t* buffer = new wchar_t[size];
		in.read((char*)buffer, sizeof(wchar_t) * size);
#endif
        Convert(buffer, size, *it);
        delete [] buffer;
    }
}

void
ReadWString(ifstream& i_in, wstring& o_wstring,locale loc )
{
    string str;
    ReadString(i_in,str);
    o_wstring = Tools::ConvertUTF8StringToWstring(str,loc);
}

void
ReadString(ifstream& i_in, string& o_string)
{
    o_string.clear();
    int size;
    i_in.read((char*)&size, 4);
    char* buffer = new char[size];
    i_in.read((char*)buffer, 1 * size);
    Convert(buffer, size, o_string);
    delete [] buffer;
}

}
