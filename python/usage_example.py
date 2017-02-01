# -*- coding: utf-8 -*-

# uncomment in order to use version for c++ debugging
# import coda.common
# coda.common.set_debug_mode()

from coda.tokenizer import Tokenizer
from coda.disambiguator import Disambiguator
from coda.syntax_parser import SyntaxParser

if __name__ == '__main__':
    tokenizer = Tokenizer("RU")
    disambiguator = Disambiguator("RU")
    syntax_parser = SyntaxParser("RU")

    sentence = u'МИД пригрозил ограничить поездки американских дипломатов по России.'
    tokens = tokenizer.tokenize(sentence)
    disambiguated = disambiguator.disambiguate(tokens)
    tree = syntax_parser.parse(disambiguated)

    print tree.to_string()
    tree.draw(dot_file="/tmp/tree1.dot", show=True)
