# -*- coding: utf-8 -*-
import ctypes, os, unittest, time
import tokenizer, common

disambiguator_lib = common.load_library('disambiguator')

class DisambigatedData(tokenizer.Token):
    def __init__(self):
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

    def disambiguate(self, tokens, num_hypothesis=None, 
        coverage_percent=None, max_number_of_hypothesis=1024):
        '''
        '''
        tokenizer.push_tokens_to_cpp(tokens)
        if num_hypothesis is None:
            if coverage_percent is None:
                disambiguator_lib.Disambiguate(
                    self.language)
                return self.__request_result_from_cpp(tokens)[0][0]
            else:
                disambiguator_lib.DisambiguateCoverage(
                    self.language,
                    ctypes.c_double(coverage_percent),
                    ctypes.c_size_t(max_number_of_hypothesis))
        elif coverage_percent is None:
            disambiguator_lib.DisambiguateHypothesis(
                self.language, 
                ctypes.c_size_t(num_hypothesis)) 
        else:
            raise ValueError("Either num_hypothesis or coverage_percent must be None")
        return self.__request_result_from_cpp(tokens)

    def __request_result_from_cpp(self, tokens):
        number_of_hypothesis = self.__request_number_of_hypothesis()
        hypothesis = []
        for hyp_index in range(number_of_hypothesis):
            disambiguated = []
            for token_index in range(len(tokens)):
                item = DisambigatedData()
                item.get_info_from_token(tokens[token_index])
                item.lemma = self.__request_lemma_from_cpp(hyp_index, token_index)
                item.label = self.__request_label_from_cpp(hyp_index, token_index)
                item.weight = self.__request_weight_from_cpp(hyp_index, token_index)
                item.lemma_id = self.__request_lemma_id_from_cpp(hyp_index, token_index)
                disambiguated.append(item)
            probability = self.__request_hypothesis_probability(hyp_index)
            hypothesis.append((disambiguated, probability))
        return hypothesis

    def __request_number_of_hypothesis(self):
        func = disambiguator_lib.RequestNumberOfHypothesis
        func.restype = ctypes.c_size_t
        content = ctypes.c_size_t(
            func()).value
        return content

    def __request_hypothesis_probability(self, hypothesis_index):
        func = disambiguator_lib.RequestHypothesisProbability
        func.restype = ctypes.c_double
        content = ctypes.c_double(
            func(ctypes.c_size_t(hypothesis_index))).value
        return content

    def __request_lemma_from_cpp(self, hypothesis_index, token_index):
        func = disambiguator_lib.RequestLemma
        func.restype = ctypes.c_wchar_p
        content = ctypes.c_wchar_p(
            func(hypothesis_index, token_index)).value
        return content

    def __request_label_from_cpp(self, hypothesis_index, token_index):
        func = disambiguator_lib.RequestLabel
        func.restype = ctypes.c_wchar_p
        content = ctypes.c_wchar_p(
            func(hypothesis_index, token_index)).value
        return content

    def __request_weight_from_cpp(self, hypothesis_index, token_index):
        func = disambiguator_lib.RequestLabel
        func.restype = ctypes.c_double
        content = ctypes.c_double(
            func(hypothesis_index, token_index)).value
        return content

    def __request_lemma_id_from_cpp(self, hypothesis_index, token_index):
        func = disambiguator_lib.RequestLemmaId
        func.restype = ctypes.c_int
        content = ctypes.c_int(
            func(hypothesis_index, token_index)).value
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
        print '----------------------------------------------------------------------'
        tokenizator = tokenizer.Tokenizer("RU")
        tokens = tokenizator.tokenize(u"я купил себе пельменей")
        disambiguator = Disambiguator("RU")
        disambiguated = disambiguator.disambiguate(tokens)
        for index, item in enumerate(disambiguated):
            print index, item.content, item.label, item.lemma

    def test_several_hypothesis(self):
        print '----------------------------------------------------------------------'
        tokenizator = tokenizer.Tokenizer("RU")
        tokens = tokenizator.tokenize(u"сорока жила на горе")
        disambiguator = Disambiguator("RU")
        disambiguated = disambiguator.disambiguate(tokens, num_hypothesis=3)
        for (hypothesis, probability) in disambiguated:
            print "With probability {}:".format(probability)
            for index, item in enumerate(hypothesis):
                print index, item.content, item.label, item.lemma
            print ''

if __name__ == '__main__':
    unittest.main()

