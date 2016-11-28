#include "lemmatizer-interface.h"
#include "lemmatizer.h"
#include "ISentenceSplitter.h"
#include "Tools.h"


int MAX_STRING = 100;

int argPos(char *str, int argc, char **argv)
{
    int a;

    for (a=1; a<argc; a++) if (!strcmp(str, argv[a])) return a;

    return -1;
}


int main(int argc, char **argv)
{
    setlocale(LC_ALL,"");
    if (argc==1) {
        printf("\t-in <file>\n");
        printf("\t\t<file> with unlemmatized text\n");

        printf("\t-out <file>\n");
        printf("\t\t<file> to write test\n");

        printf("\t-count <int>\n");
        printf("\t\t 0 if you do not want to see number of sentences done (default = 1)\n");

        printf("\t-lang <lang>\n");
        printf("\t\t Current valid languages: ru, en (default = ru)\n");
    }
    int TerminalArgc;
    int SentenceCounter = 1;
    char in_file[MAX_STRING];
    char out_file[MAX_STRING];
    Tools::Language lang = Tools::Language::RU;

    TerminalArgc=argPos((char *)"-in", argc, argv);
    if (TerminalArgc>0) {
        if (TerminalArgc+1==argc) {
            printf("ERROR: input case data file not specified!\n");
            return 0;
        }
        strcpy(in_file, argv[TerminalArgc+1]);
    }

    TerminalArgc = argPos((char *)"-out", argc, argv);
    if (TerminalArgc>0) {
        if (TerminalArgc+1==argc) {
            printf("ERROR: output case data file not specified!\n");
            return 0;
        }
        strcpy(out_file, argv[TerminalArgc+1]);
    }

    TerminalArgc = argPos((char *)"-count", argc, argv);
    if (TerminalArgc > 0 ) {
        if (TerminalArgc + 1 == argc) {
            wcout << "ERROR: up number not specified!" << endl;
            return 0;
        }
        SentenceCounter = atoi(argv[TerminalArgc + 1 ]);
    }


    TerminalArgc = argPos((char *)"-lang", argc, argv);
    if (TerminalArgc > 0 ) {
        if (TerminalArgc + 1 == argc) {
            wcout << "ERROR: up number not specified!" << endl;
            return 0;
        }
        if (!strcmp("en", argv[TerminalArgc + 1 ]))
        {
            lang = Tools::Language::EN;
        }
        else if (!strcmp("ru", argv[TerminalArgc + 1 ]))
        {
            lang = Tools::Language::RU;
        }
        else
        {
            wcout << "WARNING: unknown language. Setting up default (ru)!" << endl;
            lang = Tools::Language::RU;
        }
    }


    lemmatizer::Lemmatize(in_file, out_file, SentenceCounter, lang);
}
