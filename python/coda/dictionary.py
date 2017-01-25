# -*- coding: utf-8 -*-

import cffi, os, unittest, time
import common

dicitonry_lib = None
ffi = None


def initialize():
    global dicitonry_lib
    src = """
    typedef struct
    {
      wchar_t lemma[80];
      int nFeatures;
      wchar_t **features;
    } MorphologyWrapper;

    void createDictionary(const char*  i_language);
    size_t getGramInfo(const wchar_t* i_token, const char * i_language);
    const MorphologyWrapper* requestGetGramInfoReturnValue(size_t i_index);
    void cleanGetGramInfoReturnValue();
    size_t synthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures, const char* i_language);
    const wchar_t* requestSynthesizeTokenFromLemmaReturnValue(size_t i_index);
    void cleanSynthesizeTokenFromLemmaReturnValue();
    """
    # Parse
    global ffi
    ffi = cffi.FFI()
    ffi.cdef(src)

    # Loadself.features = {}

    full_path = os.path.join(common.build_path, 'libdictionary.so')
    dicitonry_lib = ffi.dlopen(full_path)


class MorphologicalInformation:
    '''
    Morphological Information. A simple structure for storing morphological infomation: lemma and OpenCorpora tags.
    '''
    def __init__(self):
        self.lemma = None
        self.features = []

class Dictionary:
    '''
    Dictionary class. Provides basic dictionary functions: morphological analysis and synthesis
    Basic format is OpenCorpora

    Parameters
    ----------

    language : str
        Language used for disambiguation (RU, EN, ...)
    '''
    def __init__(self, language):
        initialize()
        self.dictionary_lib = dicitonry_lib
        self.dictionary_lib.createDictionary(language)
        self.language = language

    def morphological_information(self, word):
        '''
        Morphological analysis for input token


        Parameters
        ----------
        word (unicode): word to analyze


        Returns
        -------
        out : list of structures MorphologicalInformation: lemma and feature tags (OpenCorpora tagset)
        '''
        number_of_variants = self.dictionary_lib.getGramInfo(word, self.language)
        variants = []
        for i in range(number_of_variants):
            morphology_ptr = self.dictionary_lib.requestGetGramInfoReturnValue(i)
            morphology = MorphologicalInformation()
            morphology.lemma = ffi.string(morphology_ptr.lemma)
            number_of_features = morphology_ptr.nFeatures
            for feature_id in range(number_of_features):
                morphology.features.append(ffi.string(morphology_ptr.features[feature_id]))
            variants.append(morphology)
        self.dictionary_lib.cleanGetGramInfoReturnValue()
        return variants

    def synthesize_wordform(self, lemma, features):
        '''
        Get correct wordform based for given lemma and morphological features


        Parameters
        ----------
        lemma (unicode): word to analyze
        features (list of unicode strings): OpenCorpora morphological tags

        Returns
        -------
        out : list of unicode strings. Correct wordforms for given lemma and features
        '''
        lemma = lemma.lower()
        ffi_feature_list = []
        for feature in features:
            ffi_feature_list.append(ffi.new("wchar_t[]", feature))
        ffi_features = ffi.new("wchar_t *[]", ffi_feature_list)
        number_of_variants = self.dictionary_lib.synthesizeTokenFromLemma(lemma, ffi_features, len(features), self.language)
        variants = []
        for i in range(number_of_variants):
            var_ptr = self.dictionary_lib.requestSynthesizeTokenFromLemmaReturnValue(i)
            variants.append(ffi.string(var_ptr))
        self.dictionary_lib.cleanSynthesizeTokenFromLemmaReturnValue()
        return variants


class DictionaryTest(unittest.TestCase):
    def test_time_creation(self):
        print '----------------------------------------------------------------------'
        dictionary = Dictionary("RU")
        start_time = time.time()
        dictionary = Dictionary("RU")
        spent_time = time.time() - start_time
        self.assertLess(spent_time, 10e-3)

    def test_wordform_synthesis(self):
        print "----Testing wordform synthesis. Set word 'Россия' into Genitive case--"
        test_word = u"Россия"
        dictionary = Dictionary("RU")
        test_variant = dictionary.synthesize_wordform(test_word, [u"NOUN", u"inan", u"femn", u"sing", u"Sgtm", u"gent", u"Geox"])[0]
        print "Synthesized variant: {}".format(test_variant.encode('utf-8'))

    def test_morphological_info(self):
        print "----Test morphological info. Testing word 'России'-------------------"
        test_word = u"России"
        dictionary = Dictionary("RU")
        morphological_variants = dictionary.morphological_information(test_word)
        print "Number of variants: {}".format(len(morphological_variants))
        for variant in morphological_variants:
            print "Lemma: {}".format(variant.lemma.encode('utf-8'))
            print "Features: ", variant.features


if __name__ == '__main__':
    unittest.main()