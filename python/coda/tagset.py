# -*- coding: utf-8 -*-
import re
import tagset_constants as tc


class Syntagrus2OpenCorpora(object):
    '''
    Syntagrus2OpenCorpora class
    Converts SynTagRus tag in lemmatizer output format into a list of OpenCorpora grammar markers
    '''
    def __init__(self):
        self.mapping_ = {}
        self.mapping_[tc.STR_NOUN] = tc.OC_NOUN
        self.mapping_[tc.STR_VERB] = tc.OC_VERB
        self.mapping_[tc.STR_NUMR] = tc.OC_NUMR
        self.mapping_[tc.STR_ADVB] = tc.OC_ADVB
        self.mapping_[tc.STR_PREP] = tc.OC_PREP
        self.mapping_[tc.STR_CONJ] = tc.OC_CONJ
        self.mapping_[tc.STR_INTJ] = tc.OC_INTJ
        self.mapping_[tc.STR_UNKN] = tc.OC_UNKN
        self.mapping_[tc.STR_EOS] =  tc.OC_VERB
        self.mapping_[tc.STR_PRCL] = tc.OC_PRCL

        self.mapping_[tc.STR_NOM] = tc.OC_NOM
        self.mapping_[tc.STR_GEN] = tc.OC_GEN
        self.mapping_[tc.STR_DAT] = tc.OC_DAT
        self.mapping_[tc.STR_ACC] = tc.OC_ACC
        self.mapping_[tc.STR_ABL] = tc.OC_ABL
        self.mapping_[tc.STR_GEN2] = tc.OC_GEN
        self.mapping_[tc.STR_LOC] = tc.OC_LOC
        self.mapping_[tc.STR_LOC2] = tc.OC_LOC

        self.mapping_[tc.STR_ANIM_Y] = tc.OC_ANIM_Y
        self.mapping_[tc.STR_ANIM_N] = tc.OC_ANIM_N

        self.mapping_[tc.STR_MASC] = tc.OC_MASC
        self.mapping_[tc.STR_FEMN] = tc.OC_FEMN
        self.mapping_[tc.STR_NEUT] = tc.OC_NEUT

        self.mapping_[tc.STR_PLUR] = tc.OC_PLUR
        self.mapping_[tc.STR_SING] = tc.OC_SING

        self.mapping_[tc.STR_NONPAST] = tc.OC_PRES
        self.mapping_[tc.STR_PAST] = tc.OC_PAST
        self.mapping_[tc.STR_PRES] = tc.OC_PRES

        self.mapping_[tc.OC_IMPERF] = tc.OC_IMPERF
        self.mapping_[tc.OC_PERF] = tc.OC_PERF

        self.mapping_[tc.STR_1PER] = tc.OC_1PER
        self.mapping_[tc.STR_2PER] = tc.OC_2PER
        self.mapping_[tc.STR_3PER] = tc.OC_3PER

        self.regs_ = []
        self.regs_.append(re.compile(ur'V.+?ПРИЧ.+КР'))
        self.regs_.append(re.compile(ur'V.+?ПРИЧ'))
        self.regs_.append(re.compile(ur'V.+?ДЕЕПР'))
        self.regs_.append(re.compile(ur'V'))
        self.regs_.append(re.compile(ur'V.+?ИНФ'))
        self.regs_.append(re.compile(ur'СРАВ'))
        self.regs_.append(re.compile(ur'ADV'))
        self.regs_.append(re.compile(ur'A.+?КР'))
        self.regs_.append(re.compile(ur'A'))
        self.regs_.append(re.compile(ur'NUM'))
        self.regs_.append(re.compile(ur'PR'))
        self.regs_.append(re.compile(ur'CONJ'))
        self.regs_.append(re.compile(ur'PART'))
        self.regs_.append(re.compile(ur'INTJ'))
        self.regs_.append(re.compile(ur'S'))

        self.processors_ = []
        self.processors_.append(self.process_prts_)
        self.processors_.append(self.process_prtf_)
        self.processors_.append(self.process_grnd_)
        self.processors_.append(self.process_inf_)
        self.processors_.append(self.process_verb_)
        self.processors_.append(self.process_cmp_)
        self.processors_.append(self.process_adv_)
        self.processors_.append(self.process_adjs_)
        self.processors_.append(self.process_adjf_)
        self.processors_.append(self.process_num_)
        self.processors_.append(self.process_pr_)
        self.processors_.append(self.process_conj_)
        self.processors_.append(self.process_part_)
        self.processors_.append(self.process_intj_)
        self.processors_.append(self.process_noun_)

        self.manager_ = zip(self.regs_, self.processors_)

        self.reg_fut_ = re.compile(ur'СОВ.+?НЕПРОШ')
        self.reg_pres_ = re.compile(ur'НЕСОВ.+?НЕПРОШ')

    def convert_syntagrus_tag_to_opencorpora(self, tag):
        '''
        converts SynTagRus tag in pos@gram1@gram2....@gramN format into OpenCorpora markers
        input: unicode string - syntagrus tag pos@gram1@gram2....@gramN
        output: list - list of OpenCorpora markers
        '''
        tag = tag.strip()
        for reg, processor in self.manager_:
            if reg.find(tag) is not None:
                return processor(tag)
        raise ValueError('unknown tag %s' % tag)

    def process_prts_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_PRTS)
        oc_markers += self.process_verb_form_(markers[1:])
        return oc_markers

    def process_verb_form_(self, markers):
        result = [self.mapping_[marker] for marker in markers if marker in self.mapping_]
        if tc.STR_PASV not in markers:
            result.append(tc.OC_ACTV)
        return result

    def process_prtf_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_PRTF)
        oc_markers += self.process_verb_form_(markers[1:])
        return oc_markers

    def process_grnd_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_GRND)
        oc_markers += self.process_verb_form_(markers[1:])
        return oc_markers

    def process_inf_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_INFN)
        oc_markers += self.process_verb_form_(markers[1:])
        return oc_markers

    def process_verb_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_VERB)
        markers = markers[1:]
        oc_markers += [self.mapping_[marker] for marker in markers if marker in self.mapping_ and marker not in tc.OC_TENSE]
        if tc.STR_PASV not in markers:
            oc_markers.append(tc.OC_ACTV)
        if tag.find(tc.STR_PRES):
            oc_markers.append(tc.OC_PRES)
        elif tag.find(tc.STR_PAST):
            oc_markers.append(tc.OC_PAST)
        elif self.reg_fut_.search(tag) is not None:
            oc_markers.append(tc.OC_FUTR)
        elif self.reg_pres_.search(tag) is not None:
            oc_markers.append(tc.OC_PRES)
        return oc_markers

    def process_cmp_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_adv_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_adjs_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_ADJS)
        oc_markers += [self.mapping_[marker] for marker in markers if marker in self.mapping_]
        return oc_markers

    def process_adjf_(self, tag):
        oc_markers = []
        markers = tag.split('@')
        oc_markers.append(tc.OC_ADJF)
        oc_markers += [self.mapping_[marker] for marker in markers if marker in self.mapping_]
        return  oc_markers

    def process_num_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_pr_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_conj_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_part_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_intj_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]

    def process_noun_(self, tag):
        markers = tag.split('@')
        return [self.mapping_[marker] for marker in markers if marker in self.mapping_]
