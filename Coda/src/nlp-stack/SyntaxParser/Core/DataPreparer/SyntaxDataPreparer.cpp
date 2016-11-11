#include "SyntaxDataPreparer.h"
#include "SyntaxTree.h"

namespace SyntaxParser
{

SyntaxDataPreparer::SyntaxDataPreparer(
    const string& treesFile
    , const string& trainFile
    , const string& testFile
    , shared_ptr<DataFunctor> trainFunctor
    , shared_ptr<DataFunctor> testFunctor)
    : treesFile(treesFile)
    , trainFile(trainFile)
    , testFile(testFile)
    , trainFunctor(trainFunctor)
    , testFunctor(testFunctor)
{
}

void SyntaxDataPreparer::Prepare()
{
    wifstream in(treesFile);
    wofstream outTrain(trainFile);
    wofstream outTest(testFile);

    int enumerator = 0;
    wstring line;
    while (getline(in, line))
    {
        try
        {
            if (Tools::Trim(line).size() == 0) {
                ++enumerator;
                wcout << L"Empty line: " << enumerator << endl;
                continue;
            } else {
                SyntaxTree tree(line);
                if ((*trainFunctor)(enumerator)) {
                    outTrain << line << std::endl;
                } else if ((*testFunctor)(enumerator)) {
                    outTest << line << std::endl;
                }
                ++enumerator;
            }
        }
        catch(...)
        {
            ++enumerator;
            wcout << L"Exceptional line: " << enumerator << L": " << line << endl;
        }
    }
    in.close();
    outTrain.close();
    outTest.close();
}

}
