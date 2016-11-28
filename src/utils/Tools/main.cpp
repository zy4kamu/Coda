#include "Tools.h"

#include <iostream>
#include <vector>
#include <cstring>
#include <algorithm>
#include <thread>

using std::wcout;
using std::wcin;
using std::locale;
using std::string;

class SimpleServer : public Tools::ProcessInetServer<Tools::FloatSizeConnecter<> >
{
public:
    SimpleServer(int port) : Tools::ProcessInetServer<Tools::FloatSizeConnecter<> >(port){}
    string GetResponce(const string& request) const
    {
        return "Recieved " + request;
    }
};

void RunServer()
{
    SimpleServer server(5001);
    server.Start();
}

void RunClient(const string& message)
{
    try {
        Tools::ProcessInetClient<Tools::FloatSizeConnecter<> > client("127.0.0.1",5001);
        for (size_t i = 0; i < 10; ++i)
        {
            client.Send(message);
            std::cout << client.Get() << std::endl;
        }
        client.Close();
    } catch (...) {
        std::cout << "Client error;" << std::endl;
        return;
    }
}

int main()
{
    Tools::PrepareConsole();
//    SimpleServer serv(5001);
//    serv.Start();
    string message("sport motobike soccer football play tennis score result live");
    try {
        Tools::ProcessInetClient<Tools::FloatSizeConnecter<> > client("127.0.0.1",1234);
        for (size_t i = 0; i < 100; ++i)
        {
            client.Send(message);
            std::cout << client.Get() << std::endl;
        }
        client.Close();
    } catch (...) {
        std::cout << "Client error;" << std::endl;
        return 0;
    }

//    std::thread server(RunServer);
//    std::thread client1(RunClient, "client1");
//    std::thread client2(RunClient, "client2");
//    client1.join();
//    client2.join();
//    server.join();


//    vector<string> input = { "a", "ab", "bc", "bca", "c", "caa" };
//    Tools::AhoCorasickParser<string> compiled(input);
//    string toMatch = "abcaa";
//    vector<pair<size_t, size_t> > matched = compiled.Match(toMatch);
//    std::sort(matched.begin(), matched.end());

//    Tools::TestAhoCorasick<string>();

//    vector<int> range = Tools::Range(2, 10, 2);

//    Tools::DocumentExtendedCreator a;
//    std::shared_ptr<Tools::DocumentExtended> d =
//            std::static_pointer_cast<Tools::DocumentExtended>(
//                a.loadDocument("/media/smaug/Independent/articles/2015-01-02_if-were-stuck-with-political-wags-at-least-let-them-be-interesting-in-their-own-right-9954876")
//                );
//    std::wcout<<d->url()<<std::endl;

//    /////////////////////
//    size_t numberOfFiles = Tools::GetNumberOfFiles("/media/smaug/russian_wikipedia/syntax-matrix/");
//    std::wcout << numberOfFiles << std::endl;

//    vector<string> files = Tools::GetFiles("/media/smaug/russian_wikipedia/syntax-matrix/");
//    std::wcout << files.size() << std::endl;

//    string echo = Tools::ExecuteShellScript("echo \u041f\u0440\u0438\u0432\u0435\u0442!!");//here was cyrrilic symbols: Привет
//    wstring wecho = Tools::ConvertUTF8StringToWstring(echo);
//    std::wcout << wecho << std::endl;

//    std::wstring line = L"Hello, how are you?";
//    std::vector<wstring> splitted1 = Tools::Split(line, L" ", false);
//    wcout << line << std::endl;
//    for (size_t i = 0; i < splitted1.size(); ++i)
//    {
//        wcout << i << L" " << splitted1[i] << std::endl;
//    }

//    std::string line1 = "Hello, how are you?";
//    std::vector<string> splitted2 = Tools::Split(line1, ' ', false);
//    std::cout << line1 << std::endl;
//    for (size_t i = 0; i < splitted2.size(); ++i)
//    {
//        std::cout << i << L" " << splitted2[i] << std::endl;
//    }

//    line = L"Hello, how are      you?";
//    std::vector<wstring> splitted3 = Tools::Split(line, L' ', true);
//    wcout << line << std::endl;
//    for (size_t i = 0; i < splitted3.size(); ++i)
//    {
//        wcout << i << L" " << splitted3[i] << std::endl;
//    }

//    line = L"Hello, how     are you?  z";
//    std::vector<wstring> splitted4 = Tools::Split(line, L"  ");
//    wcout << line << std::endl;
//    for (size_t i = 0; i < splitted4.size(); ++i)
//    {
//        wcout << i << L" " << splitted4[i] << std::endl;
//    }

//    line = L"Hello, how are you?";
//    vector<wstring> splittings;
//    splittings.push_back(L"ll");
//    splittings.push_back(L" ");
//    std::vector<wstring> splitted5 = Tools::Split(line, splittings);
//    std::vector<wstring> splitted6 = Tools::Split(line, L" ", L" ");
//    std::vector<wstring> splitted7 = Tools::Split(line, L' ', L' ');

//    wstring toRemove = L"Hello, how are you?";
//    wstring removed = Tools::Remove(toRemove, L' ');
//    wcout << removed << std::endl;

//    vector<wstring> toConcat(3, L"Hello");
//    wstring zz = L"zz";
//    wstring concatenated = Tools::Concatenate(toConcat, wstring(L" "));
//    wcout << concatenated << std::endl;

//    wstring toContains = L"Hello";
//    bool contains = Tools::ContainsSubstring(toContains, toContains);
//    wcout << contains << std::endl;

//    wstring str = L"MOTHER = \u041c\u0410\u041c\u0410";//here was cyrrilic symbols: МАМА
//    wcout << Tools::ToLower(str) << std::endl;

//    map<wstring, int> data;
//    data[L"mother"] = 1;
//    vector<wstring> keys;

//    vector<int> vec = {1, 2, 3, 4, 5};
//    Tools::EraseLess(&vec, 4);

//    unordered_map<string, int> map1 = { {"a", 1}, {"b", 2}, {"c", 3} };
//    Tools::EraseMapValueLess(&map1, 3);

//    unordered_map<string, int> map2 = { {"a", 1}, {"b", 2}, {"c", 3} };

//    auto keys1 = Tools::Keys(map2);
//    auto values1 = Tools::Values(map2);

//    unordered_map<string, int> map3 = { {"a", 5}, {"d", 6}, {"e", 7} };
//    bool x1 = Tools::Contains(map3, {"a", 5});
//    x1 = Tools::Contains(map3, {"a", 6});
//    x1 = Tools::ContainsKey(map3, "a");
//    x1 = Tools::ContainsKey(map3, "x");
//    x1 = Tools::ContainsValue(map3, 5);
//    x1 = Tools::ContainsValue(map3, 8);
//    if (x1) {
//    }

//    vector<string> vec1 = { "b", "z"};

//    Tools::Difference(&map2, map3);
//    Tools::Difference(&map2, vec1);

//    vector<string> v1 = {"a", "b", "c" };
//    vector<string> v2 = {"a", "b", "d" };
//    vector<string> v3 = Tools::Difference(v1, v2);
//    vector<string> v4 = Tools::Intersection(v1, v2);
//    Tools::Difference(&v1, v2);

//    v1 = {"a", "b", "c" };
//    map1 = { {"a", 1} };
//    Tools::Difference(&v1, map1);

//    vector<int> sorted1 { 3, 2 , 1 };
//    vector<char> sorted2 { 'a', 'b', 'c' };
//    Tools::Sort(sorted1, sorted2);
//    for (size_t i = 0; i < sorted1.size(); ++i)
//        std::wcout << sorted1[i] << L" " << sorted2[i] << std::endl;

//    std::map<int, int> mmap = { {1,1}, {2, 2}, {3, 3} };


//    vector<pair<int, int> > mapvec = Tools::ToVector(mmap);
//    for (auto p : mapvec) {
//        std::wcout << p.first << L" " << p.second << std::endl;
//    }
//    std::map<const int, int> mmmap = Tools::ToMap(mmap);

//    vector<int> toUnique = { 1, 2, 1, 2, 1, 2 };
//    Tools::Distinct(&toUnique);

//    Tools::TableCalculator<> calculator([](double x) { return x * x; }, 2, 10, 1);
//    for (double i = 0; i < 20; i += 1.5)
//    {
//        std::wcout << i << L" " << calculator.Get(i) << std::endl;
//    }


//    Tools::TrieDictionary<vector<int>, bool> trieDict(false);

//    trieDict[vector<int>{1, 2, 3}] = true;
//    trieDict[vector<int>{1, 2, 4}] = true;
//    trieDict[vector<int>{1, 5}] = true;

//    auto iter = trieDict.begin();

//    for(auto iter = trieDict.begin(); iter != trieDict.end(); iter++)
//    {
//        auto a = iter->first;

//        for(auto& it : a)
//        {
//            std::cout << it << " ";
//        }
//        std::cout << std::endl;
//    }

//    wcout << "Trie size: " << trieDict.size() << std::endl;

//    wcout << L"That's all" << std::endl;
}
