# -*- coding: utf-8 -*-
import ctypes, os, unittest, time
import tokenizer, common

disambiguator_lib = common.load_library('disambiguator')

class DisambigatedData(tokenizer.Token):
    def __int__(self):
        self.lemma = u''
        self.label = u''
        self.weight = 0
        self.lemma_id = -1

    def get_info_from_token(self, token):
        self.content = token.content
        self.punctuation = token.punctuation

class Disambiguator(object):
    def __init__(self, language):
        disambiguator_lib.CreateDisambiguator(language)
        self.language = language

    def disambiguate(self, tokens):
        tokenizer.push_tokens_to_cpp(tokens)
        disambiguator_lib.Disambiguate(self.language)
        disambiguated = []
        for token_index in range(len(tokens)):
            item = DisambigatedData()
            item.get_info_from_token(tokens[token_index])
            item.lemma = self.__request_lemma_from_cpp(token_index)
            item.label = self.__request_label_from_cpp(token_index)
            item.weight = self.__request_weight_from_cpp(token_index)
            item.lemma_id = self.__request_lemma_id_from_cpp(token_index)
            disambiguated.append(item)
        return disambiguated

    def __request_lemma_from_cpp(self, token_index):
        func = disambiguator_lib.RequestLemma
        func.res_type = ctypes.c_wchar_p
        content = ctypes.c_wchar_p(func(token_index)).value
        return content

    def __request_label_from_cpp(self, token_index):
        func = disambiguator_lib.RequestLabel
        func.res_type = ctypes.c_wchar_p
        content = ctypes.c_wchar_p(func(token_index)).value
        return content

    def __request_weight_from_cpp(self, token_index):
        func = disambiguator_lib.RequestLabel
        func.res_type = ctypes.c_double
        content = ctypes.c_double(func(token_index)).value
        return content

    def __request_lemma_id_from_cpp(self, token_index):
        func = disambiguator_lib.RequestLemmaId
        func.res_type = ctypes.c_int
        content = ctypes.c_int(func(token_index)).value
        return content

class TimeCreationTest(unittest.TestCase):
    def test_time_creation(self):
        disambiguator = Disambiguator("RU")
        start_time = time.time()
        disambiguator = Disambiguator("RU")
        spent_time = time.time() - start_time
        self.assertLess(spent_time, 1e-3)

class DisambiguationTest(unittest.TestCase):
    def test_disambiguation(self):
        tokenizator = tokenizer.Tokenizer("RU")
        tokens = tokenizator.tokenize(u"я купил себе пельменей")
        disambiguator = Disambiguator("RU")
        disambiguated = disambiguator.disambiguate(tokens)
        for index, item in enumerate(disambiguated):
            print index, item.content, item.label, item.lemma

if __name__ == '__main__':
    unittest.main()


