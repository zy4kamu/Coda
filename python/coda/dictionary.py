# -*- coding: utf-8 -*-

import os
import cffi

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
    build_path = '/opt/coda/lib'

    full_path = os.path.join(build_path, 'libdictionary.so')
    dicitonry_lib = ffi.dlopen(full_path)


class MorphologicalInformation:
    def __init__(self):
        self.lemma = None
        self.features = []

class Dictionary:
    def __init__(self, language):
        initialize()
        self.dictionary_lib = dicitonry_lib
        self.dictionary_lib.createDictionary(language)
        self.language = language

    def morphological_information(self, word):
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


d = Dictionary("RU")
test_word = u"Россия"
print d.synthesize_wordform(test_word, [u"NOUN", u"inan", u"femn", u"sing", u"Sgtm", u"gent", u"Geox"])[0]