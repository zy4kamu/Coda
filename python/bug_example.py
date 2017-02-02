# -*- coding: utf-8 -*-

import coda.common
coda.common.set_debug_mode()

from coda.tokenizer import Tokenizer
from coda.disambiguator import Disambiguator
from coda.dictionary import Dictionary


if __name__ == '__main__':
    tokenizer = Tokenizer("RU")
    disambiguator = Disambiguator("RU")
    wf_generator = Dictionary("RU")

    sentence = u'МИД пригрозил ограничить выезд американских дипломатов по России'
    tokens = tokenizer.tokenize(sentence)