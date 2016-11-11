#include "BinaryReader.hpp"
#include "BinaryWriter.hpp"
#include "FileReader.h"
#include "FileWriter.h"

using namespace FileManager;

int main()
{
    Tools::PrepareConsole();

    shared_ptr<std::stringstream> stream =
        std::make_shared<std::stringstream>();
    BinaryReader<std::stringstream> reader(stream, std::locale("ru_RU.UTF-8"));
    BinaryWriter<std::stringstream> writer(stream, std::locale("ru_RU.UTF-8"));

    int first = 1;
    size_t second = 2;
    double third = 3.14;
    string forth = "fourth";
    wstring fifth = L"пятый";
    writer << first << second << third << forth << fifth;

    int _first;
    size_t _second;
    double _third;
    string _forth;
    wstring _fifth;
    reader >> _first >> _second >> _third >> _forth >> _fifth;

    set<wstring> a1;
    a1.insert(L"3");
    a1.insert(L"4");
    a1.insert(L"5");
    Write(string(CSV_MANAGER_CONFIG_PATH) + "1.txt", a1);
    a1.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "1.txt", &a1);

    set<vector<string> > a2;
    a2.insert(vector<string>(1, "3"));
    a2.insert(vector<string>(2, "4"));
    a2.insert(vector<string>(3, "5"));
    Write(string(CSV_MANAGER_CONFIG_PATH) + "2.txt", a2);
    a2.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "2.txt", &a2);

    set<map<string, int> > a3;
    map<string, int> za3;
    za3["A"] = 1;
    za3["B"] = 2;
    za3["C"] = 3;
    a3.insert(za3);
    a3.insert(za3);
    a3.insert(map<string, int>());
    Write(string(CSV_MANAGER_CONFIG_PATH) + "3.txt", a3);
    a3.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "3.txt", &a3);

    map<int, int> a4;
    a4[3] = 1;
    a4[2] = 2;
    a4[1] = 3;
    Write(string(CSV_MANAGER_CONFIG_PATH) + "4.txt", a4);
    a4.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "4.txt", &a4);

    unordered_map<int, vector<wstring> > a5;
    a5[1] = vector<wstring>(3, L"a");
    a5[2] = vector<wstring>(2, L"b");
    a5[3] = vector<wstring>(1, L"c");
    Write(string(CSV_MANAGER_CONFIG_PATH) + "5.txt", a5);
    a5.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "5.txt", &a5);

    unordered_map<int, map<wstring, wstring> > a6;
    map<wstring, wstring> za6;
    za6[L"A"] = L"A";
    za6[L"B"] = L"B";
    za6[L"C"] = L"C";
    a6[1] = za6;
    a6[2] = za6;
    a6[3] = za6;
    Write(string(CSV_MANAGER_CONFIG_PATH) + "6.txt", a6);
    a6.clear();
    Read(string(CSV_MANAGER_CONFIG_PATH) + "6.txt", &a6);

    map<wstring, map<wstring, int> > a7;
    map<wstring, int> za7;
    za7[L"A"] = 1;
    za7[L"B"] = 2;
    za7[L"C"] = 3;
    a7[L"AA"] = za7;
    map<wstring, int> zza7;
    zza7[L"A"] = 3;
    zza7[L"B"] = 2;
    zza7[L"C"] = 1;
    a7[L"BB"] = zza7;
    FileManager::WriteSortedTable(string(CSV_MANAGER_CONFIG_PATH) + "7.txt", a7, L',');
}
