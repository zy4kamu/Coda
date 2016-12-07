/** 
 * @file	targetConstants.h
 * @brief	header file with tools and tags from different texts collections
 *
 * .. invisible:
 *     _   _ _____ _     _____ _____
 *
 *    | | | |  ___| |   |  ___/  ___|
 *
 *    | | | | |__ | |   | |__ \ `--.
 *
 *    | | | |  __|| |   |  __| `--. \
 *
 *    \ \_/ / |___| |___| |___/\__/ /
 *
 *     \___/\____/\_____|____/\____/
 *
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *   http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License
 *
 */


#ifndef _TAGSET_CONSTANTS_H_
#define _TAGSET_CONSTANTS_H_

/**
 * @namespace	OpenCorpora
 * @brief	Namespace for set of tags (short variant, english) using in Opencorpora corpus
 */
namespace OpenCorpora
{
//OpenCorpora tagset extern constants. BEGIN:

	//Categories
	extern const wchar_t *  POS_NOUN; 
	extern const wchar_t *  POS_VERB; 
	extern const wchar_t *  POS_INFN; 
	extern const wchar_t *  POS_PRTF; 
	extern const wchar_t *  POS_PRTS;
	extern const wchar_t *  POS_GRND; 
	extern const wchar_t *  POS_ADJF;
	extern const wchar_t *  POS_ADJS; 
	extern const wchar_t *  POS_COMP;
	extern const wchar_t *  POS_NUMR; 
	extern const wchar_t *  POS_ADVB; 
	extern const wchar_t *  POS_NPRO; 
	extern const wchar_t *  POS_PRED; 
	extern const wchar_t *  POS_PREP; 
	extern const wchar_t *  POS_CONJ; 
	extern const wchar_t *  POS_PRCL; 
	extern const wchar_t *  POS_INTJ; 
	extern const wchar_t *  POS_UNKN; 
	extern const wchar_t *  POS_PNCT;
	extern const wchar_t *  POS_NUMB; 
	extern const wchar_t *  POS_LATN; 
	extern const wchar_t *  POS_ROMN;

	//1. animacy (NOUN,ADJF,PRTF)
	extern const wchar_t *  ANIM_Y;
	extern const wchar_t *  ANIM_N;
	extern const wchar_t *  ANIM_P;
	//2. gender (NOUN, ADJF, ADJS, VERB, PRTF, PRTS)
	extern const wchar_t * G_MASC;
	extern const wchar_t *  G_FEMN;
	extern const wchar_t *  G_NEUT;
	extern const wchar_t *  G_COMMON;
	//3. number (NOUN, ADJF, ADJS, VERB, PRTF, PRTS)
	extern const wchar_t *  N_SING;
	extern const wchar_t *  N_PLUR;
	extern const wchar_t *  N_SG;
	extern const wchar_t *  N_PL;

	//4. case (NOUN, ADJF, PRTF)
	extern const wchar_t *  C_NOM;
	extern const wchar_t *  C_GEN;
	extern const wchar_t *  C_DAT;
	extern const wchar_t *  C_ACC;
	extern const wchar_t *  C_ABL;
	extern const wchar_t *  C_VOC;
	extern const wchar_t *  C_LOC;
	extern const wchar_t *  C_GEN1;
	extern const wchar_t *  C_GEN2;
	extern const wchar_t *  C_LOC1;
	extern const wchar_t *  C_LOC2;
	//5. name type (NOUN)
	extern const wchar_t *  NT_ABBR;
	extern const wchar_t *  NT_NAME;
	extern const wchar_t *  NT_SURN;
	extern const wchar_t *  NT_PATR;
	extern const wchar_t *  NT_GEOX;
	extern const wchar_t *  NT_ORGN;
	extern const wchar_t *  NT_TRAD;
	//6. aspect (VERB, INFN, GRND, PRTF, PRTS)
	extern const wchar_t *  A_PERF;
	extern const wchar_t *  A_IMPRF;
	//7.tense (VERB, INFN, GRND, PRTF, PRTS)
	extern const wchar_t *  TN_PRES;
	extern const wchar_t *  TN_PAST;
	extern const wchar_t *  TN_FUTR;
	//8.mode (VERB)
	extern const wchar_t *  M_INDC;
	extern const wchar_t *  M_IMPER;
	//9.voice (PRTF, PRTS)
	extern const wchar_t *  V_ACT;
	extern const wchar_t *  V_PASS;
	//10.person (VERB, NPRO)
	extern const wchar_t *  P_1;
	extern const wchar_t *  P_2;
	extern const wchar_t *  P_3;
	//11. Adjective Type (ADJF)
	extern const wchar_t *  AT_QUAL;
	extern const wchar_t *  AT_ANUM;
	extern const wchar_t *  AT_POSS;
	extern const wchar_t *  AT_APRO;
	//12. Adjective degree (ADJF)
	extern const wchar_t *  AD_SUPR;
	//13. transitiveness (VERB, INFN)
	extern const wchar_t *  VT_INTR;
	extern const wchar_t *  VI_TRAN;

	//detailed adjective features
	extern const wchar_t *  ADJ_SUBT; //possibly substantivated adjective

	extern const wchar_t *  ADJ_CMP2; //comparative \u043f\u043e-* COMP//here was cyrrilic symbols: по

	//detailed aspect features

	extern const wchar_t *  V_PIMP; //permanently impersonal VERB
	extern const wchar_t *  V_UIMP; //impersonal usage VERB
	extern const wchar_t *  V_MULT; //multiplicative VERB
	extern const wchar_t *  V_REFL; //reflective VERB

	//inclusiveness
	extern const wchar_t *  V_INCL; //inclusive VERB
	extern const wchar_t *  V_EXCL; //exclusive VERB

	extern const wchar_t *  V_GIMP; //impersonal gerund

	//lexical marks
	extern const wchar_t *  LEX_INFR; //conversational speech
	extern const wchar_t *  LEX_SLNG; //slang
	extern const wchar_t *  LEX_ARCH; //old
	extern const wchar_t *  LEX_LITR; //standard form
	extern const wchar_t *  LEX_TYPO; //typo
	extern const wchar_t *  LEX_DIST; //distorted form

	//syntax
	extern const wchar_t *  SYN_PRNT; //parenthetical insertion

	//detailed pronoun features
	extern const wchar_t *  PR_QUES;
	extern const wchar_t *  PR_DMNS;

	//remarkable wordforms
	extern const wchar_t *  F_bE; //NOUN form
	extern const wchar_t *  F_EN; 
	extern const wchar_t *  F_IE; //NOUN form
	extern const wchar_t *  F_bI; 
	extern const wchar_t *  ADJ_FVEY; //form *-\u0435\u044e ADJ/NOUN//here was cyrrilic symbols: ею
	extern const wchar_t *  ADJ_FVOY; //form *-\u043e\u044e ADJ/NOUN//here was cyrrilic symbols: ою
	extern const wchar_t *  ADJ_FVEJ; //form *-\u0435\u0439 ADJ//here was cyrrilic symbols: ей
	extern const wchar_t *  V_GSH; //gerund on *-\u0448\u0438//here was cyrrilic symbols: ши


	extern const wchar_t *  F_PRDX;
	extern const wchar_t *  F_COUN;
	extern const wchar_t *  F_COLL;

	extern const wchar_t *  F_AFP;

	extern const wchar_t *  PREP_V;		

	extern const wchar_t *  F_FIXED;
};

/**
 * @namespace	MALT
 * @brief	Namespace for set of tags using in Malt format
 */
namespace MALT {
//Malt tagset extern constants. BEGIN:
	extern const wchar_t COMPOUND;

	//name type
	extern const wchar_t NT_PROPER;
	extern const char NT_COMMON;

	//Gender
	extern const wchar_t G_MASC;
	extern const wchar_t G_FEMN;
	extern const wchar_t G_NEUT;
	extern const wchar_t G_COMMON;

	//case
	extern const wchar_t C_NOM;
	extern const wchar_t C_GEN;
	extern const wchar_t C_DAT;
	extern const wchar_t C_ACC;
	extern const wchar_t C_ABL;
	extern const wchar_t C_LOC;
	extern const wchar_t C_VOC;

	//number
	extern const wchar_t N_PLUR;
	extern const wchar_t N_SING;

	//verb type
	extern const wchar_t VT_GRND;
	extern const wchar_t VT_INF;
	extern const wchar_t VT_MAIN;
	extern const wchar_t VT_PRT;

	//voice
	extern const wchar_t V_ACT;
	extern const wchar_t V_PASS;
	extern const wchar_t V_MED;

	//tense
	extern const wchar_t TN_PRES;
	extern const wchar_t TN_PAST;
	extern const wchar_t TN_FUTR;

	//aspect
	extern const wchar_t A_PERF;
	extern const wchar_t A_IMPERF;

	//mode
	extern const wchar_t M_IMPER;
	extern const wchar_t M_INDC;

	//person
	extern const wchar_t P_1;
	extern const wchar_t P_2;
	extern const wchar_t P_3;

	//degree
	extern const wchar_t DEG_COMP;
	extern const wchar_t DEG_POS;
	extern const wchar_t DEG_SUPERL;

	extern const wchar_t PREP;

	//adjective type
	extern const wchar_t AT_QUAL;
	extern const wchar_t AT_POSS;

	//full/short form
	extern const wchar_t D_SHORT;
	extern const wchar_t D_FULL;

	//numeral type
	extern const wchar_t NUMT_CARD;
	extern const wchar_t NUMT_ORD;

	//animacy
	extern const wchar_t ANIM_N;
	extern const wchar_t ANIM_Y;

	extern const wchar_t PT_NON;

	//dummy category. Do not exist in MALT grammar
	extern const wchar_t *  TAG;

	//POS
	extern const wchar_t POS_NOUN;
	extern const wchar_t POS_VERB;
	extern const wchar_t POS_ADJ;
	extern const wchar_t POS_PRON;
	extern const wchar_t POS_ADVB;
	extern const wchar_t POS_ADP;
	extern const wchar_t POS_CONJ;
	extern const wchar_t POS_NUMR;
	extern const wchar_t POS_PRCL;
	extern const wchar_t POS_INTJ;	
	extern const wchar_t POS_ABBR;

	extern const wchar_t EMPTY;


//Malt tagset extern constants. END
};

namespace SynTagRusTagset
{
	extern const wchar_t *  POS_NOUN; 
	extern const wchar_t *  POS_VERB;  
	extern const wchar_t *  POS_ADJ;
	extern const wchar_t *  POS_NUMR; 
	extern const wchar_t *  POS_ADVB; 
	extern const wchar_t *  POS_PREP; 
	extern const wchar_t *  POS_CONJ; 
	extern const wchar_t *  POS_PRCL; 
	extern const wchar_t *  POS_INTJ; 
	extern const wchar_t *  POS_UNKN; 
	extern const wchar_t *  POS_SENT; 

	extern const wchar_t * ANIM_Y;
	extern const wchar_t * ANIM_N;

	extern const wchar_t * G_MASC;
	extern const wchar_t *  G_FEMN;
	extern const wchar_t *  G_NEUT;

	extern const wchar_t *  N_SING;
	extern const wchar_t *  N_PLUR;

	extern const wchar_t *  C_NOM;
	extern const wchar_t *  C_DAT;
	extern const wchar_t *  C_ACC;
	extern const wchar_t *  C_ABL;
	extern const wchar_t *  C_GEN1;
	extern const wchar_t *  C_GEN2;
	extern const wchar_t *  C_LOC1;
	extern const wchar_t *  C_LOC2;

	extern const wchar_t * DEG_COMP;
	extern const wchar_t * DEG_SUPERL;

	extern const wchar_t * D_SHORT;

	extern const wchar_t * VT_GRND;
	extern const wchar_t * VT_INF;
	extern const wchar_t * VT_PRT;

	extern const wchar_t * M_INDC;
	extern const wchar_t * M_IMPER;

	extern const wchar_t * A_PERF;
	extern const wchar_t * A_IMPRF;

	extern const wchar_t * TN_NONPAST;
	extern const wchar_t * TN_PAST;
	extern const wchar_t * TN_PRES;

	extern const wchar_t * P_1;
	extern const wchar_t * P_2;
	extern const wchar_t * P_3;

	extern const wchar_t * V_PASS;

	extern const wchar_t * COMP_PART;
	extern const wchar_t * SOFT;

	//Additional tags non-present in the tagset but neede for symmetry.
	//e.g. DEG_POSITIVE is added to get symmetry with DEG_COMP and DEG_SUPERL
	extern const wchar_t * DEG_POSITIVE;
	extern const wchar_t * D_FULL;
	extern const wchar_t * VT_FIN;
	extern const wchar_t * TIME_FUTURE;
	extern const wchar_t * TIME_PAST;
	extern const wchar_t * TIME_PRESENT;
	extern const wchar_t * V_ACT;

};

namespace PrivateTagset
{
	//grammemes
	extern const wchar_t * CAT_POS;
	extern const wchar_t * CAT_GEN;
	extern const wchar_t * CAT_NUM;
	extern const wchar_t * CAT_CASE;
	extern const wchar_t * CAT_CASE2;
	extern const wchar_t * CAT_ANIM;
	extern const wchar_t * CAT_DEG;
	extern const wchar_t * CAT_SHORT;
	extern const wchar_t * CAT_VERB_FORM;
	extern const wchar_t * CAT_MODE;
	extern const wchar_t * CAT_ASPECT;
	extern const wchar_t * CAT_TENSE;
	extern const wchar_t * CAT_PERSON;
	extern const wchar_t * CAT_VOICE;
	extern const wchar_t * CAT_TIME;

	extern const wchar_t CHUNK_O;
	extern const wchar_t CHUNK_B;
	extern const wchar_t CHUNK_I;

	extern const wchar_t * EMPTY;
};

#endif //_TAGSET_CONSTANTS_H_
