 # -*- coding: utf-8 -*-
import unittest, time
import tokenizer, disambiguator, common

src = '''
/* FUNCTIONS RELATED TO SYNTAX PARSER */

void CreateSyntaxParser(const char* languagePtr);
void SyntaxParse(const char* languagePtr);
int GetRootIndex();
const wchar_t* GetSentence();
int GetParentIndex(size_t tokenIndex);
void Draw(const char* outputFile, bool openPDF);

/* FUNCTION RELATED TO PARSING PYTHON INPUT */

void CreateTreeFromParsedDisambiguated();
void SetParent(int nodeIndex, int parentIndex);
'''
ffi, syntax_parser_lib = common.load_cffi_library(src, 'syntax-parser')

class SyntaxNode(disambiguator.DisambiguatedData):
    '''
    Extends disambiguator.DisambiguatedData 
    with dependency parsing information:
        index: index of token in the sentence
        parent_index: index of parent in the sentence (-1 for root)
    '''
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
    '''
    Dependency parsing tree
    '''
    def __init__(self):
        self.root_index = -1
        self.nodes = []
        self.sentence = u''

    def to_string(self):
        '''
        Converts tree to a readable string.

        Notes
        -----
        We don't use str or repr functions to 
        express that output is unicode string

        '''
        return u'\n'.join([node.to_string() for node in self.nodes])

    def draw(self, dot_file, show=False):
        '''
        Produces .dot format file and visualizes it.

        Parameters
        ----------

        dot_file: str
            output .dot file 

        show: boolean
            if true then it uses graphiz to create 
            .pdf file and okular to open it.
        
        Notes
        -----
        After you close the .pdf file it is deleted by default.
        '''
        self.__push_to_cpp()
        func = syntax_parser_lib.Draw
        func(dot_file, show)

    def __push_to_cpp(self):
        disambiguator.push_disambiguated_to_cpp(self.nodes)
        syntax_parser_lib.CreateTreeFromParsedDisambiguated()
        for i, node in enumerate(self.nodes):
            syntax_parser_lib.SetParent(i, node.parent_index)

class SyntaxParser(object):
    '''
    Given output of disambiguator creates dependency parser tree

    Parameters
    ----------

    language: str
        Language used for disambiguation (RU, EN, ...)

    '''
    def __init__(self, language):
        syntax_parser_lib.CreateSyntaxParser(language)
        self.language = language

    def parse(self, disambiguated):
        '''
        performs syntax parsing

        Parameters
        ----------

        disambiguated: list of disambiguator.DisambiguatedData

        Returns
        -------
        out: instance of SyntaxTree class
        '''
        disambiguator.push_disambiguated_to_cpp(disambiguated)
        syntax_parser_lib.SyntaxParse(self.language)
        tree = SyntaxTree()
        tree.nodes = []
        for index, item in enumerate(disambiguated):
            syntax_node = SyntaxNode()
            syntax_node.index = index
            syntax_node.parent_index = syntax_parser_lib.GetParentIndex(index)
            syntax_node.get_info_from_morphology(item)
            tree.nodes.append(syntax_node)
        tree.root_index = syntax_parser_lib.GetRootIndex()
        tree.sentence = ffi.string(syntax_parser_lib.GetSentence())
        return tree

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
        parsed.draw(dot_file="/tmp/tree.dot", show=False)

if __name__ == '__main__':
    unittest.main()
