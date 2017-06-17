# -*- coding: utf-8 -*-


import cffi, os, unittest, time
import common

src = """
/* VARIABLES TO STORE TEMPORARY RESULTS */
typedef struct
{
  wchar_t lemma[80];
  int nFeatures;
  wchar_t **features;
} MorphologyWrapper;

/* DICTIONARY INTERFACE FUNCTIONS */
void CreateDictionary(const char*  i_language);
size_t GetGramInfo(const wchar_t* i_token, const char * i_language);
size_t SynthesizeTokenFromLemma(const wchar_t* i_lemma, const wchar_t ** i_grammarFeatures, size_t i_numberOfFeatures, const char* i_language);
size_t GetParadigmForLemma(const wchar_t* i_lemma, const char* i_language);


/* GETTING RESULTS FOR PYTHON */
const MorphologyWrapper* RequestGetGramInfoReturnValue(size_t i_index);
const wchar_t* RequestSynthesizeTokenFromLemmaReturnValue(size_t i_index);
const size_t RequestGetParadigmForLemmaSize(size_t i_index);
const MorphologyWrapper* RequestGetParadigmForLemmaReturnValueMorphology(size_t i_paradigm_index, size_t i_element_index);
const wchar_t* RequestGetParadigmForLemmaReturnValueWordform(size_t i_paradigm_index, size_t i_element_index);

/* CLEAR MEMORY */
void CleanGetGramInfoReturnValue();
void CleanSynthesizeTokenFromLemmaReturnValue();
void CleanGetParadigmForLemmaReturnValue();
"""

ffi, dictionary_lib = common.load_cffi_library(src, 'dictionary')

class MorphologicalInformation:
    '''
    Morphological Information. A simple structure for storing morphological information: lemma and OpenCorpora tags.
    '''
    def __init__(self):
        self.lemma = None
        self.features = []
    
    def init_from_c_ptr(self, morphology_ptr):
        self.lemma = ffi.string(morphology_ptr.lemma)
        self.features = []
        number_of_features = morphology_ptr.nFeatures
        for feature_id in range(number_of_features):
            self.features.append(ffi.string(morphology_ptr.features[feature_id]))
            

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
        self.dictionary_lib = dictionary_lib
        self.dictionary_lib.CreateDictionary(language)
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
        number_of_variants = self.dictionary_lib.GetGramInfo(word, self.language)
        variants = []
        for i in range(number_of_variants):
            morphology_ptr = self.dictionary_lib.RequestGetGramInfoReturnValue(i)
            morphology = MorphologicalInformation()
            morphology.init_from_c_ptr(morphology_ptr)
            variants.append(morphology)
            
        self.dictionary_lib.CleanGetGramInfoReturnValue()
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
        number_of_variants = self.dictionary_lib.SynthesizeTokenFromLemma(lemma, ffi_features, len(features), self.language)
        variants = []
        for i in range(number_of_variants):
            var_ptr = self.dictionary_lib.RequestSynthesizeTokenFromLemmaReturnValue(i)
            variants.append(ffi.string(var_ptr))
        self.dictionary_lib.CleanSynthesizeTokenFromLemmaReturnValue()
        return variants

    def get_paradigms(self, lemma):
        '''
        Get full paradigms for lemma


        Parameters
        ----------
        lemma (unicode): word in initial form

        Returns
        -------
        out : list of paradigms. Each paradigm is the dictionary of wordforms with their MorphologicalInformation structures (OpenCorpora tagset)
        '''
        lemma = lemma.lower()
        number_of_paradigms = self.dictionary_lib.GetParadigmForLemma(lemma, self.language)
        paradigms = []
        for i in range(number_of_paradigms):
            number_of_elements = self.dictionary_lib.RequestGetParadigmForLemmaSize(i)
            new_paradigm = {}
            for j in range(number_of_elements):
                morphology_ptr = self.dictionary_lib.RequestGetParadigmForLemmaReturnValueMorphology(i, j)
                morphology = MorphologicalInformation()
                morphology.init_from_c_ptr(morphology_ptr)
                word = ffi.string(self.dictionary_lib.RequestGetParadigmForLemmaReturnValueWordform(i, j))
                if word in new_paradigm:
                    new_paradigm[word].append(morphology)
                else:
                    new_paradigm[word] = [morphology]
            paradigms.append(new_paradigm)

        self.dictionary_lib.CleanGetParadigmForLemmaReturnValue()
        return paradigms



class DictionaryTest(unittest.TestCase):
    def test_time_creation(self):
        print '----------------------------------------------------------------------'
        start_time = time.time()
        dictionary = Dictionary("RU")
        spent_time = time.time() - start_time
        self.assertLess(spent_time, 10e-3)

    def test_wordform_synthesis(self):
        test_word = u"Россия"
        print (u"----Testing wordform synthesis. Set word '%s to Genitive case--" % test_word)
        dictionary = Dictionary("RU")
        variants = dictionary.synthesize_wordform(test_word, [u'NOUN', u'inan', u'femn', u'sing', u'Sgtm', u'gent', u'Geox'])
        test_variant = u''
        if variants:
            test_variant = variants[0]
        print "Synthesized variant: {}".format(test_variant.encode('utf-8'))
        test_word = u"закупить"
        expected_result = u"закуплена"
        print (u"----Testing wordform synthesis. Set word '%s' into '%s' --" % (test_word, expected_result))
        dictionary = Dictionary("RU")
        variants = dictionary.synthesize_wordform(test_word, [u'PRTS', u'femn', u'sing', u'perf', u'past', u'pssv'])
        test_variant = u''
        if variants:
            test_variant = variants[0]
        print "Synthesized variant: {}".format(test_variant.encode('utf-8'))

    def test_morphological_info(self):
        test_word = u"закуплена"
        print (u"----Test morphological info. Testing word '%s'-------------------" % test_word)
        dictionary = Dictionary("RU")
        morphological_variants = dictionary.morphological_information(test_word)
        print "Number of variants: {}".format(len(morphological_variants))
        for variant in morphological_variants:
            print "Lemma: {}".format(variant.lemma.encode('utf-8'))
            print "Features: ", variant.features

    def test_paradigm(self):
        test_word = u"сине-зелёный"
        print (u"----Test paradigm. Testing word '%s'-------------------" % test_word)
        dictionary = Dictionary("RU")
        paradigms = dictionary.get_paradigms(test_word)
        print "Number of paradigms: {}".format(len(paradigms))
        number = 0 
        for paradigm in paradigms:
            print "=====Paradigm {}=====".format(number)
            number+=1
            for word in paradigm:
                print "{} :".format(word.encode('utf-8'))
                for element in paradigm[word]:
                    print "\t{} {}".format(element.lemma.encode('utf-8'), element.features)

if __name__ == '__main__':
    unittest.main()
