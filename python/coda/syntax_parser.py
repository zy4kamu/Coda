 # -*- coding: utf-8 -*-
import ctypes, os, unittest, time
import tokenizer, disambiguator, common

syntax_parser_lib = common.load_library('syntax-parser')

class SyntaxNode(disambiguator.DisambiguatedData):
    def __init__(self):
        self.index = -1
        self.parent_index = -1

    def get_info_from_morphology(self, disambiguated):
        self.get_info_from_token(disambiguated)
        self.lemma = disambiguated.lemma
        self.label = disambiguated.label
        self.weight = disambiguated.weight
        self.lemma_id = disambiguated.lemma_id

    def to_string(self):
        return u'{} {} {} {} {}'.format(
            self.index, self.parent_index, 
            self.content, self.label, self.lemma)

class SyntaxTree(object):
    def __init__(self):
        self.root_index = -1
        self.nodes = []
        self.sentence = u''

    def to_string(self):
        return u'\n'.join([node.to_string() for node in self.nodes])

    def draw(self, dot_file="", show=False):
        func = syntax_parser_lib.Draw
        func(dot_file, show)

class SyntaxParser(object):
    def __init__(self, language):
        syntax_parser_lib.CreateSyntaxParser(language)
        self.language = language

    def parse(self, disambiguated):
        disambiguator.push_disambiguated_to_cpp(disambiguated)
        syntax_parser_lib.SyntaxParse(self.language)
        tree = SyntaxTree()
        tree.nodes = []
        for index, item in enumerate(disambiguated):
            syntax_node = SyntaxNode()
            syntax_node.index = index
            syntax_node.parent_index = self.__request_parent_index(index)
            syntax_node.get_info_from_morphology(item)
            tree.nodes.append(syntax_node)
        tree.root_index = self.__request_root_index()
        tree.sentence = self.__request_sentence()
        return tree

    def __request_parent_index(self, index):
        func = syntax_parser_lib.GetParentIndex
        func.restype = ctypes.c_int
        parent_index = ctypes.c_int(func(index)).value
        return parent_index

    def __request_root_index(self):
        func = syntax_parser_lib.GetRootIndex
        func.restype = ctypes.c_int
        root_index = ctypes.c_int(func()).value
        return root_index

    def __request_sentence(self):
        func = syntax_parser_lib.GetSentence
        func.restype = ctypes.c_wchar_p
        sentence = ctypes.c_wchar_p(func()).value
        return sentence

class SyntaxParserTest(unittest.TestCase):
    def test_time_creation(self):
        parser = SyntaxParser("RU")
        start_time = time.time()
        parser = SyntaxParser("RU")
        spent_time = time.time() - start_time
        self.assertLess(spent_time, 1e-3)

    def test_syntax_parsing(self):
        print '----------------------------------------------------------------------'
        sentence = u"мать видит дочь"
        print "Input sentence: ", sentence
        print ''
        tokenizator = tokenizer.Tokenizer("RU")
        tokens = tokenizator.tokenize(sentence)
        disambiger = disambiguator.Disambiguator("RU")
        disambiguated = disambiger.disambiguate(tokens)
        parser = SyntaxParser("RU")
        parsed = parser.parse(disambiguated)
        print parsed.to_string()
        parsed.draw(show=True)

if __name__ == '__main__':
    unittest.main()
