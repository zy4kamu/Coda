# -*- coding: utf-8 -*-
from tokenizer import Tokenizer
from disambiguator import Disambiguator

if __name__ == '__main__':
    tokenizer = Tokenizer("RU")
    disambiguator = Disambiguator("RU")

    sentence = u'Большая часть джедаев погибает, а остальные оказываются окружены дроидами.'
    tokens = tokenizer.tokenize(sentence)
    disambiguated = disambiguator.disambiguate(tokens)
    for index, item in enumerate(disambiguated):
        print index, item.content, item.label, item.lemma
