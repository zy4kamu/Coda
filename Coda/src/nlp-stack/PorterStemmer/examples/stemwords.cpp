#include "../stemmer.h"

using std::string;

int main(int argc, char * argv[])
{
    stemmer::PorterStemmer<Tokenization::Token> lsa("english");
    Tokenization::Token temp;
    temp.content = L"cars cars Starts heek";
    temp = lsa.stemmStr(temp);
    std::wcout<< temp.content<<std::endl;

    return 0;
}

