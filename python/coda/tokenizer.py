# -*- coding: utf-8 -*-
import unittest, time
import common

src = '''
void CreateTokenizer(const char* languagePtr);

/* FUNCTIONS RELATED TO TOKENIZATION */

size_t Tokenize(const wchar_t* sentencePtr, const char* languagePtr);
const wchar_t* RequestContent(size_t index);
size_t RequestPunctuationSize(size_t index);
const wchar_t* RequestPunctuation(size_t tokenIndex, size_t punctIndex);

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

void ResetParsedTokens();
void PushParsedContent(const wchar_t* content);
void PushParsedPunctuation(const wchar_t* punctuation);
'''
ffi, tokenizer_lib = common.load_cffi_library(src, 'tokenizer')

class Token(object):
    '''
    Word plus punctuation followed by this word, 
    a base class for morphological and syntax analysers.
    '''
    def __init__(self):
        self.content = u''
        self.punctuation = []

def push_tokens_to_cpp(tokens):
    tokenizer_lib.ResetParsedTokens()
    for token in tokens:
        tokenizer_lib.PushParsedContent(token.content)
        for punct in token.punctuation:
            tokenizer_lib.PushParsedPunctuation(punct)

class Tokenizer(object):
    '''
    Given a sentence produces a list of Tokens.

    Parameters
    ----------

    language: str
        Language used for tokenization (RU, EN, ...)
    '''
    def __init__(self, language):
        tokenizer_lib.CreateTokenizer(language)
        self.language = language

    def tokenize(self, sentence):
        '''
        Splits sentence by tokens


        Parameters
        ----------
        sentence: unicode string

        Returns
        -------
        out: list of Tokens
        '''
        size = tokenizer_lib.Tokenize(sentence, self.language)
        tokens = []
        for token_index in range(size):
            content = ffi.string(tokenizer_lib.RequestContent(token_index))
            punctuation_size = tokenizer_lib.RequestPunctuationSize(token_index)
            punctuation = []
            for punct_index in range(punctuation_size):
                punct = ffi.string(tokenizer_lib.RequestPunctuation(
                    token_index, punct_index))
                punctuation.append(punct)
            token = Token()
            token.content = content
            token.punctuation = punctuation
            tokens.append(token)
        return tokens

class TokenizationTest(unittest.TestCase):
    def test_tokenize(self):
        tokenizer = Tokenizer("RU")
        line = u"hello, привет!!!"
        tokens = tokenizer.tokenize(line)
        self.assertEqual(tokens[0].content, u'hello')
        self.assertEqual(len(tokens[0].punctuation), 1)
        self.assertEqual(tokens[0].punctuation[0], u',')
        self.assertEqual(tokens[1].content, u'привет')
        self.assertEqual(len(tokens[1].punctuation), 3)

class CppCommuncationTest(unittest.TestCase):
    def test_push_to_cpp(self):
        tokenizer = Tokenizer("RU")
        line = u"hello, привет!!!"
        tokens = tokenizer.tokenize(line)
        push_tokens_to_cpp(tokens)

class TimeCreationTest(unittest.TestCase):
    def test_time_creation(self):
        tokenizer = Tokenizer("RU")
        start_time = time.time()
        tokenizer = Tokenizer("RU")
        spent_time = time.time() - start_time
        self.assertLess(spent_time, 1e-3)

if __name__ == '__main__':
    unittest.main()
