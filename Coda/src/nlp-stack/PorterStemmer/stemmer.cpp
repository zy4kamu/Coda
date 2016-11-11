/* This is a simple program which uses libstemmer to provide a command
 * line interface for stemming using any of the algorithms provided.
 */

#include <stdio.h>
#include <stdlib.h> /* for malloc, free */
#include <string.h> /* for memmove */
#include <ctype.h>  /* for isupper, tolower */
#include <iostream>
#include "libstemmer.h"
#include "stemmer.h"

using std::string;
using Tokenization::Token;

namespace stemmer {

template<>
std::string
PorterStemmer<Token>::strToString(Token &i_str)
{
    string result;
    Tools::ConvertWstringToUTF8(i_str.content,result);
    return result;
}

template<>
Token
PorterStemmer<Token>::StringToStr(string& i_str)
{
    Token temp;
    temp.content = Tools::ConvertUTF8StringToWstring(i_str);
    return temp;
}

template<>
std::string
PorterStemmer<std::string>::strToString(std::string &i_str)
{
    return i_str;
}

template<>
std::string
PorterStemmer<string>::StringToStr(string& i_str)
{
    return i_str;
}

}

