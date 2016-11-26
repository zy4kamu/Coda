# -*- coding: utf-8 -*-
import ctypes, os

build_path = '/home/stepan/veles.nlp/Coda/python/lib'

tokenizer_lib = ctypes.CDLL(os.path.join(build_path, 'libtokenizer.so'))

class Token(object):
    def __int__(self):
        self.content = u''
        self.punctuation = []

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


if __name__ == '__main__':
    tokenizer = Tokenizer("RU")
    line = u"hello, привет!!!"
    tokens = tokenizer.tokenize(line)
    for token in tokens:
        print token.content
        print token.punctuation

