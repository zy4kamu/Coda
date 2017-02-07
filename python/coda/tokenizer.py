# -*- coding: utf-8 -*-
import cffi, os, unittest, time
import common

tokenizer_lib = None #common.load_library('tokenizer')
ffi = None

def initialize():
    global tokenizer_lib
    src = """
    void createTokenizer(const char* languagePtr);
    size_t tokenize(const wchar_t* sentencePtr, const char* languagePtr);
    const wchar_t* requestContent(size_t index);
    size_t requestPunctuationSize(size_t index);
    const wchar_t* requestPunctuation(size_t tokenIndex, size_t punctIndex);
    void resetParsedTokens();
    void pushParsedContent(const wchar_t* content);
    void pushParsedPunctuation(const wchar_t* punctuation);
    """
    # Parse
    global ffi
    ffi = cffi.FFI()
    ffi.cdef(src)

    full_path = os.path.join(common.get_build_path(), 'libtokenizer.so')
    tokenizer_lib = ffi.dlopen(full_path)

class Token(object):
    '''
    Word plus punctuation followed by this word, 
    a base class for morphological and syntax analysers.
    '''
    def __init__(self):
        self.content = u''
        self.punctuation = []

def push_tokens_to_cpp(tokens):
    tokenizer_lib.resetParsedTokens()
    for token in tokens:
        tokenizer_lib.pushParsedContent(token.content)
        for punct in token.punctuation:
            tokenizer_lib.pushParsedPunctuation(punct)

class Tokenizer(object):
    '''
    Given a sentence produces a list of Tokens.

    Parameters
    ----------

    language: str
        Language used for tokenization (RU, EN, ...)
    '''
    def __init__(self, language):
        initialize()
        self.tokenizer_lib = tokenizer_lib
        self.tokenizer_lib.createTokenizer(language)
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
        number_of_tokens = self.tokenizer_lib.tokenize(sentence, self.language)
        tokens = []
        for token_index in range(number_of_tokens):
            #print("Ready")
            content = self.__request_content_from_cpp(token_index)
            #print("OK!")
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
        content_ptr = tokenizer_lib.requestContent(token_index)
        content = ffi.string(content_ptr)
        return content

    def __request_punct_size_from_cpp(self, token_index):
        size = self.tokenizer_lib.requestPunctuationSize(token_index)
        return size

    def __request_punctuation_from_cpp(self, token_index, punct_index):
        punct_ptr = tokenizer_lib.requestPunctuation(token_index, punct_index)
        punctuation = ffi.string(punct_ptr)
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
