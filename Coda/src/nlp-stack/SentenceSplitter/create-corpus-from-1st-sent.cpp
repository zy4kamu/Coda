#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "sentence-splitter.h"
#include "Tools.h"


int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");

    std::ifstream list_in("/home/mkudinov/Data/KINOPOISK_ALL/movie_news/file_list");

    std::string buffer;

    _sentence_splitter::SentenceSplitter spltr(SSPLITTER_CONFIG);

    while(std::getline(list_in, buffer))
    {
        std::wifstream in("/home/mkudinov/Data/KINOPOISK_ALL/movie_news/" + buffer);

        SetLocale(in);

        std::wstring buffer2;


        while(std::getline(in, buffer2))
        {
            buffer2 = Tools::Trim(buffer2);
            if(!buffer2.empty())
            {
                break;
            }
        }

        std::vector<size_t> borders = spltr.split(buffer2);

        int a = borders.size() > 1 ? 1 : 0;

        if(borders.empty())
        {
            continue;
        }

        for(int i = borders[a]; i >= 0; i--)
        {
            if(buffer2[i] == L']')
            {
                std::wcout << buffer2 << std::endl;
                break;
            }
            else if(buffer2[i] == L'[')
            {
                break;
            }
        }

        in.close();
    }

    list_in.close();

}
