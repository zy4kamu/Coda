#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <boost/regex.hpp>
#include "ssplitter-feature-extractor.h"
#include "logistic-regression.h"
#include "sentence-splitter.h"
#include "ssplitter-trainer.h"
#include "Tools.h"
#include "ISentenceSplitter.h"

using namespace std;
using _sentence_splitter::ISentenceSplitter;

/* Show message when parameters are not parsed
 */
void paramMessage()
{
    cout << "Usage:" << endl;
    cout << "\tno params\t- split text form console" << endl;
    cout << "\t<input file>\t- split text from file" << endl;
    cout << "\t[-m] <input file>\t- model path (if absent, standard config is used)" << endl;
    cout << "\t[-v] <input file>\t- validation file path for train mode(if absent, tested on learning data)" << endl;
    cout << "\t[-test] <input file>\t- test data from file. Data is in format 'sentence per line'" << endl;
    cout << "\t[-train] <input file>\t- train using data from file. Data is in format 'sentence per line'" << endl;
}

/* Split text from console
 */
void testFromConsole(shared_ptr<ISentenceSplitter> spltr)
{
    wstring input;
    while(true)
    {
        wcout << L"Input text:" << endl;

        std::getline(wcin,input);

        if(input.empty())
        {
           break;
        }
        std::vector<size_t> borders = spltr->split(input);

        std::wcout << std::endl << L"*************************" << std::endl;

        int init = 0;

        for(size_t i = 0; i < borders.size(); i++)
        {
            bool trim = false;
            for(size_t j = init; j <= borders[i]; j++)
            {
                if(trim)
                {
                    std::wcout << input[j];
                }
                else if(std::iswgraph(input[j]))
                {
                    std::wcout << input[j];
                    trim = true;
                }
            }
            std::wcout << std::endl;
            init = borders[i] + 1;
        }
    }
}

/* Split text from file
 */
int splitFile(string& model, string& fileName)
{
    _sentence_splitter::SentenceSplitter spltr(model);
    std::wifstream in(fileName);
	Tools::SetLocale(in);
    wstring input;
    if(!in)
    {
        std::cout << "Unable to open file " << fileName << std::endl;
        return -1;
    }


    while(std::getline(in, input))
    {
        if(input.empty())
        {
           continue;
        }
        std::vector<size_t> borders = spltr.split(input);


        int init = 0;

        for(size_t i = 0; i < borders.size(); i++)
        {
            bool trim = false;
            for(size_t j = init; j <= borders[i]; j++)
            {
                if(trim)
                {
                    std::wcout << input[j];
                }
                else if(std::iswgraph(input[j]))
                {
                    std::wcout << input[j];
                    trim = true;
                }
            }
            std::wcout << std::endl;
            init = borders[i] + 1;
        }
    }
    return 0;
}

/*
 *Load train set in format 'sentence per line' and train model
 */
void train(string& model, string& trainFileName, string& validFileName)
{
    _sentence_splitter::Trainer trainer(3,3);
// for Russian, space importance param was 0.5. For English (PennTreeBank corpus) 0.6 seems to be better
    trainer.train( trainFileName, validFileName, model, 0.6);
}

/*
 * Load test set in format 'sentence per line' and test model
 * Output: precision, recall, accuracy
 */
void test(string& model, string& fileName)
{
    _sentence_splitter::Trainer trainer(3,3);

    trainer.test(fileName, model, true);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    std::wcout << L"Input language: ru, en: ";
    wstring lang;
    getline(wcin, lang);

    std::unordered_map<wstring, Tools::Language> langIds = {
        {L"ru", Tools::Language::RU},
        {L"en", Tools::Language::EN}
    };
    if (langIds.find(lang) == langIds.end()) {
        std::wcout << L"Wrong language input..." << std::endl;
        exit(0);
    }

    shared_ptr<ISentenceSplitter> spltr = ISentenceSplitter::GetSentenceSplitter(langIds[lang]);

    if(argc < 2)
    {
        testFromConsole(spltr);
    }
    else
    {
        std::string model = SSPLITTER_CONFIG_ENGLISH;
     //   std::string model = "/home/irina/SentenceSplitterData/modelEM.txt";
        std::string testFile;
        std::string trainFile;
        std::string validFile;
        std::string input;
        for(size_t i = 1; i < size_t(argc); i++)
        {
           if(strcmp(argv[i], "-train") == 0)
           {
               if(i == size_t(argc) - 1)
               {
                   paramMessage();
                   return -1;
               }
               i++;
               trainFile = argv[i];
               continue;
           }
           if(strcmp(argv[i], "-test") == 0)
           {
               if(i == size_t(argc) - 1)
               {
                   paramMessage();
                   return -1;
               }
               i++;
               testFile = argv[i];
               continue;
           }
           if(strcmp(argv[i], "-m") == 0)
           {
               if(i == size_t(argc) - 1)
               {
                   paramMessage();
                   return -1;
               }
               i++;
               model = argv[i];
               continue;
           }
           if(strcmp(argv[i], "-v") == 0)
           {
               if(i == size_t(argc) - 1)
               {
                   paramMessage();
                   return -1;
               }
               i++;
               validFile = argv[i];
               continue;
           }
           if(i > 1)
           {
               paramMessage();
               return -1;
           }
           input = argv[i];
        }
        if(input != "") {
            return splitFile(model, input);
        }
        if(trainFile != "")
        {
           train(model, trainFile, validFile);
        }
        if(testFile != "") {
           test(model, testFile);
        }

    }

return 0;
}

