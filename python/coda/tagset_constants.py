# -*- coding: utf-8 -*-

'''
tagset_constants.py.py
Constants for opencorpora and syntagrus tagsets
Author: mkudinov
'''

#Grammar categories
CAT_POS = u'POS'
CAT_ANIMACY = u'ANIMACY'
CAT_GENDER = u'GENDER'
CAT_NUMBER  = u'NUMBER'
CAT_CASE = u'CASE'
CAT_ASPECT = u'ASPECT'
CAT_TENSE = u'TENSE'
CAT_MODE = u'MODE'
CAT_VOICE = u'VOICE'
CAT_PERSON = u'PERSON'
CAT_ADJ_TYPE = u'ADJF_TYPE'
CAT_ADJ_DEGREE = u'DEGREE'
CAT_TRANSIT = u'TRANS' #transitivity

#OpenCorpora constants
#Parts of speech
OC_NOUN = u"NOUN"
OC_VERB = u"VERB"
OC_INFN = u"INFN"
OC_PRTF = u"PRTF"
OC_PRTS = u"PRTS"
OC_GRND = u"GRND"
OC_ADJF = u"ADJF"
OC_ADJS = u"ADJS"
OC_COMP = u"COMP"
OC_NUMR = u"NUMR"
OC_ADVB = u"ADVB"
OC_NPRO = u"NPRO"
OC_PRED = u"PRED"
OC_PREP = u"PREP"
OC_CONJ = u"CONJ"
OC_PRCL = u"PRCL"
OC_INTJ = u"INTJ"
OC_UNKN = u"UNKN"
OC_PNCT = u"PNCT"
OC_DIGT = u"NUMB"
OC_LATN = u"LATN"
OC_ROMN = u"ROMN"
OC_POS = [OC_NOUN, OC_VERB, OC_INFN, OC_PRTF, OC_PRTS,
          OC_GRND, OC_ADJF, OC_ADJS, OC_COMP, OC_NUMR,
          OC_ADVB, OC_NPRO, OC_PRED, OC_PREP, OC_CONJ,
          OC_PRCL, OC_INTJ, OC_UNKN, OC_PNCT, OC_DIGT, OC_LATN , OC_ROMN]

#animacy
OC_ANIM_Y = u'anim'
OC_ANIM_N = u'inan'
OC_ANIM_P = u'Inmx'
OC_ANIMACY = [OC_ANIM_Y, OC_ANIM_N, OC_ANIM_P]

#gender
OC_MASC = u'masc'
OC_FEMN = u'femn'
OC_NEUT = u'neut'
OC_MSF = u'Ms-f'
OC_GENDER = [OC_MASC, OC_FEMN, OC_NEUT, OC_MSF]

#number
OC_SING = u'sing'
OC_PLUR = u'plur'
OC_SINGTANTUM = u'Sgtm'
OC_PLURTANTUM = u'Pltm'
OC_NUMBER = [OC_SING, OC_PLUR, OC_SINGTANTUM, OC_PLURTANTUM]

#case
OC_NOM = u'nomn'
OC_GEN = u'gent'
OC_DAT = u'datv'
OC_ACC = u'accs'
OC_ABL = u'ablt'
OC_VOC = u'voct'
OC_LOC = u'loct'
OC_GEN1 = u'gen1'
OC_GEN2 = u'gen2'
OC_LOC1 = u'loc1'
OC_LOC2 = u'loc2'
OC_CASE = [OC_NOM, OC_GEN, OC_DAT, OC_ACC, OC_ABL, OC_VOC,
           OC_LOC, OC_GEN1, OC_GEN2, OC_LOC1, OC_LOC2]

#aspect
OC_PERF = u'perf'
OC_IMPERF = u'impf'
OC_ASPECT = [OC_PERF, OC_IMPERF]

#tense
OC_PRES = u'pres'
OC_PAST = u'past'
OC_FUTR =  u'futr'
OC_TENSE = [OC_PRES, OC_PAST, OC_FUTR]

#mode
OC_INDIC = u'indc'
OC_IMPR = u'impr'
OC_MODE = [OC_INDIC, OC_IMPR]

#voice
OC_ACTV = u'actv'
OC_PASV = u'pssv'
OC_VOICE = [OC_ACTV, OC_PASV]

#person
OC_1PER = u'1per'
OC_2PER = u'2per'
OC_3PER = u'3per'
OC_PERSON = [OC_1PER, OC_2PER, OC_3PER]

#adjective type
OC_ADJ_QUAL = u'Qual'
OC_ADJ_PRO = u'Apro'
OC_ADJ_NUM = u'Anum'
OC_ADJ_POSS = u'Poss'
OC_ADJ_TYPE = [OC_ADJ_QUAL, OC_ADJ_PRO, OC_ADJ_NUM, OC_ADJ_POSS]

#adjective comp. degree
OC_SUPER = u'Supr'
OC_DEGREE = [OC_SUPER]

#transitivity
OC_INTRANS = u'intr'
OC_TRANS =  u'tran'
OC_TRANSIT = [OC_INTRANS, OC_TRANS]

#SynTagRus constants
#Parts of speech
STR_NOUN = u"S"
STR_VERB = u"V"
STR_ADJ = u"A"
STR_NUMR = u"NUM"
STR_ADVB = u"ADV"
STR_PREP = u"PR"
STR_CONJ = u"CONJ"
STR_PRCL = u"PART"
STR_INTJ = u"INTJ"
STR_UNKN = u"NID"
STR_EOS= u"P"

STR_POS = [STR_NOUN, STR_VERB, STR_ADJ, STR_NUMR, STR_ADVB,
          STR_PREP, STR_CONJ, STR_PRCL, STR_INTJ, STR_UNKN, STR_EOS]

#animacy
STR_ANIM_Y = u'ОД'
STR_ANIM_N = u'НЕОД'
STR_ANIMACY = [STR_ANIM_Y, STR_ANIM_N]

#gender
STR_MASC = u'МУЖ'
STR_FEMN = u'ЖЕН'
STR_NEUT = u'СРЕД'
STR_GENDER = [STR_MASC, STR_FEMN, STR_NEUT]

#number
STR_SING = u'ЕД'
STR_PLUR = u'МН'
STR_NUMBER = [STR_SING, STR_PLUR]

#case
STR_NOM = u'ИМ'
STR_GEN = u'РОД'
STR_DAT = u'ДАТ'
STR_ACC = u'ВИН'
STR_ABL = u'ТВОР'
STR_GEN2 = u'ПАРТ'
STR_LOC = u'ПР'
STR_LOC2 = u'МЕСТ'
STR_CASE = [STR_NOM, STR_GEN, STR_DAT, STR_ACC, STR_ABL, STR_GEN2, STR_LOC, STR_LOC2]

#adjective degree and form
STR_COMP = u'СРАВ'
STR_SUPERL = u'ПРЕВ'
STR_SHORT= u'КР'
STR_ADJFORM = [STR_COMP, STR_SUPERL, STR_SHORT]

#verb form
STR_GRND = u'ДЕЕПР'
STR_INF = u'ИНФ'
STR_PRT = u'ПРИЧ'
STR_VFORM = [STR_GRND, STR_INF, STR_PRT]

#mode
STR_INDIC = u'ИЗЪЯВ'
STR_IMPR = u'ПОВ'
STR_MODE = [STR_INDIC, STR_IMPR]

#aspect
STR_IMPERF = u'НЕСОВ'
STR_PERF = u'СОВ'
STR_ASPECT = [STR_IMPERF, STR_PERF]

#tense
STR_PAST = u'ПРОШ'
STR_NONPAST = u'НЕПРОШ'
STR_PRES = u'НАСТ'
STR_TENSE = [STR_PAST, STR_NONPAST, STR_PRES]

#person
STR_1PER = u'1-Л'
STR_2PER = u'2-Л'
STR_3PER = u'3-Л'
STR_PERSON = [STR_1PER, STR_2PER, STR_3PER]

#voice
STR_PASV = u'СТРАД'
STR_VOCIE = [STR_PASV]

#misc
STR_COMP_PART = u'СЛ'
STR_SOFT = u'СМЯГ'
STR_MISC = [STR_COMP_PART, STR_SOFT]

#tagset classes
class OpenCorporaTagset(object):
    '''
    OpenCorporaTagset class. Provides basic mappings for grammar markers for OpenCorpora
    '''
    def __init__(self):
        self.tagset_ = {}
        self.tagset_[CAT_POS] = OC_POS
        self.tagset_[CAT_ANIMACY] = OC_ANIMACY
        self.tagset_[CAT_GENDER] = OC_GENDER
        self.tagset_[CAT_NUMBER] = OC_NUMBER
        self.tagset_[CAT_CASE] = OC_CASE
        self.tagset_[CAT_ASPECT] = OC_ASPECT
        self.tagset_[CAT_TENSE] = OC_TENSE
        self.tagset_[CAT_MODE] = OC_MODE
        self.tagset_[CAT_VOICE] = OC_VOICE
        self.tagset_[CAT_PERSON] = OC_PERSON
        self.tagset_[CAT_ADJ_TYPE] = OC_ADJ_TYPE
        self.tagset_[CAT_ADJ_DEGREE] = OC_DEGREE
        self.tagset_[CAT_TRANSIT] = OC_TRANSIT
        self.inv_tagset_ = {}
        for cat in self.tagset_.keys():
            for val in self.tagset_[cat]:
                self.inv_tagset_[val] = cat

    def categories(self):
        cats = self.tagset_.keys()
        for cat in cats:
            yield cat