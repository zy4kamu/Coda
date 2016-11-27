# -*- coding: utf-8 -*-
import ctypes, os, unittest, time
import common

tokenizer_lib = common.load_library('tokenizer')

class Token(object):
    def __int__(self):
        self.content = u''
        self.punctuation = []

def push_tokens_to_cpp(tokens):
    tokenizer_lib.ResetParsedTokens()
    for token in tokens:
        tokenizer_lib.PushParsedContent(token.content)
        for punct in token.punctuation:
            tokenizer_lib.PushParsedPunctuation(punct)

class Tokenizer(object):
    def __init__(self, language):
        tokenizer_lib.CreateTokenizer(language)
        self.language = language

    def tokenize(self, sentence):
        func = tokenizer_lib.Tokenize
        func.res_type = ctypes.c_size_t
        size = func(sentence, self.language)
        tokens = []
        for token_index in range(size):
            content = self.__request_content_from_cpp(token_index)
            punctuation_size = self.__request_punct_size_from_cpp(token_index)
            punctuation = []
            for punct_index in range(punctuation_size):
                punct = self.__request_punctuation_from_cpp(
                    token_index, punct_index)
                punctuation.append(punct)
            token = Token()
            token.content = content
            token.punctuation = punctuation
            tokens.append(token)
        return tokens

    def __request_content_from_cpp(self, token_index):
        func = tokenizer_lib.RequestContent
        func.res_type = ctypes.c_wchar_p
        content = ctypes.c_wchar_p(func(token_index)).value
        return content

    def __request_punct_size_from_cpp(self, token_index):
        func = tokenizer_lib.RequestPunctuationSize
        func.res_type = ctypes.c_size_t
        size = ctypes.c_size_t(func(token_index)).value
        return size

    def __request_punctuation_from_cpp(self, token_index, punct_index):
        func = tokenizer_lib.RequestPunctuation
        func.res_type = ctypes.c_wchar_p
        punctuation = ctypes.c_wchar_p(func(token_index, punct_index)).value
        return punctuation

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


