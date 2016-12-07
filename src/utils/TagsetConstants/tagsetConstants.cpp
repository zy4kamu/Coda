#include "tagsetConstants.h"

namespace OpenCorpora
{
//OpenCorpora tagset constants. BEGIN:

	//Categories
	const wchar_t *  POS_NOUN = L"NOUN"; 
	const wchar_t *  POS_VERB = L"VERB"; 
	const wchar_t *  POS_INFN = L"INFN"; 
	const wchar_t *  POS_PRTF = L"PRTF"; 
	const wchar_t *  POS_PRTS = L"PRTS";
	const wchar_t *  POS_GRND = L"GRND"; 
	const wchar_t *  POS_ADJF = L"ADJF";
	const wchar_t *  POS_ADJS = L"ADJS"; 
	const wchar_t *  POS_COMP = L"COMP";
	const wchar_t *  POS_NUMR = L"NUMR"; 
	const wchar_t *  POS_ADVB = L"ADVB"; 
	const wchar_t *  POS_NPRO = L"NPRO"; 
	const wchar_t *  POS_PRED = L"PRED"; 
	const wchar_t *  POS_PREP = L"PREP"; 
	const wchar_t *  POS_CONJ = L"CONJ"; 
	const wchar_t *  POS_PRCL = L"PRCL"; 
	const wchar_t *  POS_INTJ = L"INTJ"; 
	const wchar_t *  POS_UNKN = L"UNKN"; 
	const wchar_t *  POS_PNCT = L"PNCT";
	const wchar_t *  POS_NUMB = L"NUMB"; 
	const wchar_t *  POS_LATN = L"LATN"; 
	const wchar_t *  POS_ROMN = L"ROMN";

	//1. animacy (NOUN,ADJF,PRTF)
	const wchar_t *  ANIM_Y = L"anim";
	const wchar_t *  ANIM_N = L"inan";
	const wchar_t *  ANIM_P = L"Inmx";
	//2. gender (NOUN, ADJF, ADJS, VERB, PRTF, PRTS)
	const wchar_t* G_MASC = L"masc";
	const wchar_t *  G_FEMN = L"femn";
	const wchar_t *  G_NEUT = L"neut";
	const wchar_t *  G_COMMON = L"Ms-f";
	//3. number (NOUN, ADJF, ADJS, VERB, PRTF, PRTS)
	const wchar_t *  N_SING = L"sing";
	const wchar_t *  N_PLUR = L"plur";
	const wchar_t *  N_SG = L"Sgtm";
	const wchar_t *  N_PL = L"Pltm";

	//4. case (NOUN, ADJF, PRTF)
	const wchar_t *  C_NOM = L"nomn";
	const wchar_t *  C_GEN = L"gent";
	const wchar_t *  C_DAT = L"datv";
	const wchar_t *  C_ACC = L"accs";
	const wchar_t *  C_ABL = L"ablt";
	const wchar_t *  C_VOC = L"voct";
	const wchar_t *  C_LOC = L"loct";
	const wchar_t *  C_GEN1 = L"gen1";
	const wchar_t *  C_GEN2 = L"gen2";
	const wchar_t *  C_LOC1 = L"loc1";
	const wchar_t *  C_LOC2 = L"loc2";
	//5. name type (NOUN)
	const wchar_t *  NT_ABBR = L"Abbr";
	const wchar_t *  NT_NAME = L"Name";
	const wchar_t *  NT_SURN = L"Surn";
	const wchar_t *  NT_PATR = L"Patr";
	const wchar_t *  NT_GEOX = L"Geox";
	const wchar_t *  NT_ORGN = L"Orgn";
	const wchar_t *  NT_TRAD = L"Trad";
	//6. aspect (VERB, INFN, GRND, PRTF, PRTS)
	const wchar_t *  A_PERF = L"perf";
	const wchar_t *  A_IMPRF = L"impf";
	//7.tense (VERB, INFN, GRND, PRTF, PRTS)
	const wchar_t *  TN_PRES = L"pres";
	const wchar_t *  TN_PAST = L"past";
	const wchar_t *  TN_FUTR = L"futr";
	//8.mode (VERB)
	const wchar_t *  M_INDC = L"indc";
	const wchar_t *  M_IMPER = L"impr";
	//9.voice (PRTF, PRTS)
	const wchar_t *  V_ACT = L"actv";
	const wchar_t *  V_PASS = L"pssv";
	//10.person (VERB, NPRO)
	const wchar_t *  P_1 = L"1per";
	const wchar_t *  P_2 = L"2per";
	const wchar_t *  P_3 = L"3per";
	//11. Adjective Type (ADJF)
	const wchar_t *  AT_QUAL = L"Qual";
	const wchar_t *  AT_ANUM = L"Apro";
	const wchar_t *  AT_POSS = L"Anum";
	const wchar_t *  AT_APRO = L"Poss";
	//12. Adjective degree (ADJF)
	const wchar_t *  AD_SUPR = L"Supr";
	//13. transitiveness (VERB, INFN)
	const wchar_t *  VT_INTR = L"intr";
	const wchar_t *  VI_TRAN = L"tran";

	//detailed adjective features
	const wchar_t *  ADJ_SUBT = L"Subx"; //possibly substantivated adjective

	const wchar_t *  ADJ_CMP2 = L"Cmp2"; //comparative \u043f\u043e-* COMP//here was cyrrilic symbols: по

	//detailed aspect features

	const wchar_t *  V_PIMP = L"Impe"; //permanently impersonal VERB
	const wchar_t *  V_UIMP = L"Uimp"; //impersonal usage VERB
	const wchar_t *  V_MULT = L"Mult"; //multiplicative VERB
	const wchar_t *  V_REFL = L"Refl"; //reflective VERB

	//inclusiveness
	const wchar_t *  V_INCL = L"incl"; //inclusive VERB
	const wchar_t *  V_EXCL = L"excl"; //exclusive VERB

	const wchar_t *  V_GIMP = L"Fimp"; //impersonal gerund

	//lexical marks
	const wchar_t *  LEX_INFR = L"Infr"; //conversational speech
	const wchar_t *  LEX_SLNG = L"Slng"; //slang
	const wchar_t *  LEX_ARCH = L"Arch"; //old
	const wchar_t *  LEX_LITR = L"Litr"; //standard form
	const wchar_t *  LEX_TYPO = L"Erro"; //typo
	const wchar_t *  LEX_DIST = L"Dist"; //distorted form

	//syntax
	const wchar_t *  SYN_PRNT = L"Prnt"; //parenthetical insertion

	//detailed pronoun features
	const wchar_t *  PR_QUES = L"Ques";
	const wchar_t *  PR_DMNS = L"Dmns";

	//remarkable wordforms
	const wchar_t *  F_bE = L"V-be"; //NOUN form
	const wchar_t *  F_EN = L"V-en"; 
	const wchar_t *  F_IE = L"V-ie"; //NOUN form
	const wchar_t *  F_bI = L"V-bi"; 
	const wchar_t *  ADJ_FVEY = L"V-ey"; //form *-\u0435\u044e ADJ/NOUN//here was cyrrilic symbols: ею
	const wchar_t *  ADJ_FVOY = L"V-oy"; //form *-\u043e\u044e ADJ/NOUN//here was cyrrilic symbols: ою
	const wchar_t *  ADJ_FVEJ = L"V-ej"; //form *-\u0435\u0439 ADJ//here was cyrrilic symbols: ей
	const wchar_t *  V_GSH = L"V-sh"; //gerund on *-\u0448\u0438//here was cyrrilic symbols: ши


	const wchar_t *  F_PRDX = L"Prdx";
	const wchar_t *  F_COUN = L"Coun";
	const wchar_t *  F_COLL = L"Coll";

	const wchar_t *  F_AFP = L"Af-p";

	const wchar_t *  PREP_V = L"Vpre";		

	const wchar_t *  F_FIXED = L"Fixd";
	
	//OpenCorpora tagset constants. END
} 

namespace MALT {
//Malt tagset constants. BEGIN:
	const wchar_t COMPOUND =  'c';

	//name type
	const wchar_t NT_PROPER = 'p';
	const char NT_COMMON = 'c';

	//Gender
	const wchar_t G_MASC = 'm';
	const wchar_t G_FEMN = 'f';
	const wchar_t G_NEUT = 'n';
	const wchar_t G_COMMON = 'c';

	//case
	const wchar_t C_NOM = 'n';
	const wchar_t C_GEN = 'g';
	const wchar_t C_DAT = 'd';
	const wchar_t C_ACC = 'a';
	const wchar_t C_ABL = 'i';
	const wchar_t C_LOC = 'l';
	const wchar_t C_VOC = 'v';

	//number
	const wchar_t N_PLUR = 'p';
	const wchar_t N_SING = 's';

	//verb type
	const wchar_t VT_GRND = 'g';
	const wchar_t VT_INF = 'n';
	const wchar_t VT_MAIN = 'm';
	const wchar_t VT_PRT = 'p';

	//voice
	const wchar_t V_ACT = 'a';
	const wchar_t V_PASS = 'p';
	const wchar_t V_MED = 'm';

	//tense
	const wchar_t TN_PRES = 'p';
	const wchar_t TN_PAST = 's';
	const wchar_t TN_FUTR = 'f';

	//aspect
	const wchar_t A_PERF = 'e';
	const wchar_t A_IMPERF = 'p';

	//mode
	const wchar_t M_IMPER = 'm';
	const wchar_t M_INDC = 'i';

	//person
	const wchar_t P_1 = '1';
	const wchar_t P_2 = '2';
	const wchar_t P_3 = '3';

	//degree
	const wchar_t DEG_COMP = 'c';
	const wchar_t DEG_POS = 'p';
	const wchar_t DEG_SUPERL = 's';

	const wchar_t PREP = 'p';

	//adjective type
	const wchar_t AT_QUAL = 'f';
	const wchar_t AT_POSS = 'p';

	//full/short form
	const wchar_t D_SHORT = 's';
	const wchar_t D_FULL	= 'f';

	//numeral type
	const wchar_t NUMT_CARD = 'c';
	const wchar_t NUMT_ORD = 'o';

	//animacy
	const wchar_t ANIM_N = 'n';
	const wchar_t ANIM_Y = 'y';

	const wchar_t PT_NON = 'n';

	//dummy category. Do not exist in MALT grammar
	const wchar_t *  TAG = L"TAG";

	//POS
	const wchar_t POS_NOUN = 'N';
	const wchar_t POS_VERB = 'V';
	const wchar_t POS_ADJ = 'A';
	const wchar_t POS_PRON = 'P';
	const wchar_t POS_ADVB = 'R';
	const wchar_t POS_ADP = 'S';
	const wchar_t POS_CONJ = 'C';
	const wchar_t POS_NUMR = 'M';
	const wchar_t POS_PRCL = 'Q';
	const wchar_t POS_INTJ = 'I';	
	const wchar_t POS_ABBR = 'Y';

	//Residual
	const wchar_t EMPTY = '-';


//Malt tagset constants. END
}

namespace SynTagRusTagset
{
	const wchar_t *  POS_NOUN = L"S"; 
	const wchar_t *  POS_VERB = L"V";  
	const wchar_t *  POS_ADJ = L"A";
	const wchar_t *  POS_NUMR = L"NUM"; 
	const wchar_t *  POS_ADVB = L"ADV"; 
	const wchar_t *  POS_PREP = L"PR"; 
	const wchar_t *  POS_CONJ = L"CONJ"; 
	const wchar_t *  POS_PRCL = L"PART"; 
	const wchar_t *  POS_INTJ = L"INTJ"; 
	const wchar_t *  POS_UNKN = L"NID";
	const wchar_t *  POS_SENT = L"P";

	const wchar_t * ANIM_Y = L"\u041e\u0414";//here was cyrrilic symbols: ОД
	const wchar_t * ANIM_N = L"\u041d\u0415\u041e\u0414";//here was cyrrilic symbols: НЕОД

	const wchar_t * G_MASC = L"\u041c\u0423\u0416";//here was cyrrilic symbols: МУЖ
	const wchar_t *  G_FEMN = L"\u0416\u0415\u041d";//here was cyrrilic symbols: ЖЕН
	const wchar_t *  G_NEUT = L"\u0421\u0420\u0415\u0414";//here was cyrrilic symbols: СРЕД

	const wchar_t *  N_SING = L"\u0415\u0414";//here was cyrrilic symbols: ЕД
	const wchar_t *  N_PLUR = L"\u041c\u041d";//here was cyrrilic symbols: МН

	const wchar_t *  C_NOM = L"\u0418\u041c";//here was cyrrilic symbols: ИМ
	const wchar_t *  C_DAT = L"\u0414\u0410\u0422";//here was cyrrilic symbols: ДАТ
	const wchar_t *  C_ACC = L"\u0412\u0418\u041d";//here was cyrrilic symbols: ВИН
	const wchar_t *  C_ABL = L"\u0422\u0412\u041e\u0420";//here was cyrrilic symbols: ТВОР
	const wchar_t *  C_GEN1 = L"\u0420\u041e\u0414";//here was cyrrilic symbols: РОД
	const wchar_t *  C_GEN2 = L"\u041f\u0410\u0420\u0422";//here was cyrrilic symbols: ПАРТ
	const wchar_t *  C_LOC1 = L"\u041f\u0420";//here was cyrrilic symbols: ПР
	const wchar_t *  C_LOC2 = L"\u041c\u0415\u0421\u0422";//here was cyrrilic symbols: МЕСТ

	const wchar_t * DEG_COMP = L"\u0421\u0420\u0410\u0412";//here was cyrrilic symbols: СРАВ
	const wchar_t * DEG_SUPERL = L"\u041f\u0420\u0415\u0412";//here was cyrrilic symbols: ПРЕВ

	const wchar_t * D_SHORT = L"\u041a\u0420";//here was cyrrilic symbols: КР

	const wchar_t * VT_GRND = L"\u0414\u0415\u0415\u041f\u0420";//here was cyrrilic symbols: ДЕЕПР
	const wchar_t * VT_INF = L"\u0418\u041d\u0424";//here was cyrrilic symbols: ИНФ
	const wchar_t * VT_PRT = L"\u041f\u0420\u0418\u0427";//here was cyrrilic symbols: ПРИЧ

	const wchar_t *  M_INDC = L"\u0418\u0417\u042a\u042f\u0412";//here was cyrrilic symbols: ИЗЪЯВ
	const wchar_t *  M_IMPER = L"\u041f\u041e\u0412";//here was cyrrilic symbols: ПОВ

	const wchar_t *  A_PERF = L"\u0421\u041e\u0412";//here was cyrrilic symbols: СОВ
	const wchar_t *  A_IMPRF = L"\u041d\u0415\u0421\u041e\u0412";//here was cyrrilic symbols: НЕСОВ

	const wchar_t *  TN_NONPAST = L"\u041d\u0415\u041f\u0420\u041e\u0428";//here was cyrrilic symbols: НЕПРОШ
	const wchar_t *  TN_PAST = L"\u041f\u0420\u041e\u0428";//here was cyrrilic symbols: ПРОШ
	const wchar_t *  TN_PRES = L"\u041d\u0410\u0421\u0422";//here was cyrrilic symbols: НАСТ

	const wchar_t * P_1 = L"1-\u041b";//here was cyrrilic symbols: Л
	const wchar_t * P_2 = L"2-\u041b";//here was cyrrilic symbols: Л
	const wchar_t * P_3 = L"3-\u041b";//here was cyrrilic symbols: Л

	const wchar_t * V_PASS = L"\u0421\u0422\u0420\u0410\u0414";//here was cyrrilic symbols: СТРАД

	const wchar_t * COMP_PART = L"\u0421\u041b";//here was cyrrilic symbols: СЛ
	const wchar_t * SOFT = L"\u0421\u041c\u042f\u0413";//here was cyrrilic symbols: СМЯГ

	const wchar_t * DEG_POSITIVE = L"pos";
	const wchar_t * VT_FINITE = L"finite";
	const wchar_t * TIME_FUTURE = L"fut";
	const wchar_t * TIME_PAST = L"past";
	const wchar_t * TIME_PRESENT = L"pres";
	const wchar_t * V_ACT = L"act";
	const wchar_t * VT_FIN = L"fin";
	const wchar_t * D_FULL = L"full";

};

namespace PrivateTagset
{
	//grammemes
	const wchar_t *  CAT_POS = L"POS";
	const wchar_t * CAT_GEN = L"GENDER";
	const wchar_t *  CAT_NUM = L"NUMBER";
	const wchar_t * CAT_CASE = L"CASE";
	const wchar_t * CAT_CASE2 = L"CASE2";

	const wchar_t * CAT_ANIM = L"ANIMACY";
	const wchar_t * CAT_DEG = L"ADJ_GRAD";
	const wchar_t * CAT_SHORT = L"ADJ_FORM";
	const wchar_t * CAT_VERB_FORM = L"VERB_FORM";
	const wchar_t * CAT_MODE = L"MODE";
	const wchar_t * CAT_ASPECT = L"ASPECT";
	const wchar_t * CAT_TENSE = L"TENSE";
	const wchar_t * CAT_PERSON = L"PERS";
	const wchar_t * CAT_VOICE = L"VOICE";
	const wchar_t * CAT_TIME = L"TIME";

	const wchar_t CHUNK_O = 'O';
	const wchar_t CHUNK_B = 'B';
	const wchar_t CHUNK_I = 'I';

	const wchar_t * EMPTY = L"*NONE*";

}
