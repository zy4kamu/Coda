#include "SyntaxChunker.h"
#include "Splitter.h"

using std::string;
using std::wstring;
using std::unordered_set;
using std::unordered_map;

SyntaxChunker::SyntaxChunker(const string& pathToModel)
{
    crf = new LinearCRF::CRF(pathToModel, true);
}

vector<wstring> SyntaxChunker::Parse(
    const wstring& inputSentence, 
    const vector<wstring>& lexemes, 
    const vector<wstring>& features)
{
	LinguisticTools toolbox;
	vector<vector<wstring>> newFeatures;
	vector<wstring> newTokens = toolbox.tokenize(inputSentence);;
	vector<wstring> newLexemes;
	vector<wstring> featuresWithPunct;

	int j = 0;

	for(size_t i = 0; i < newTokens.size(); i++)
	{
		if(toolbox.punctuation.find(newTokens[i][0]) != toolbox.punctuation.end())
		{
			newLexemes.push_back(wstring(&newTokens[i][0]));
			featuresWithPunct.push_back(OpenCorpora::POS_PNCT);
		}
		else
		{
			newLexemes.push_back(lexemes[j]);
			featuresWithPunct.push_back(features[j]);
			j++;
		}
	}

	extractFeatures(newTokens, featuresWithPunct, newLexemes, newFeatures);
	vector<wstring> labelSequence;

	vector<vector<int>> constraints;
	vector<int> toAdd0;
    toAdd0.push_back(1);
	constraints.push_back(toAdd0);
	for (size_t i = 0; i < newFeatures.size(); ++i)
	{
		vector<int> toAdd;
		toAdd.push_back(0);
		toAdd.push_back(1);
		toAdd.push_back(2);
		toAdd.push_back(3);
		toAdd.push_back(4);
		constraints.push_back(toAdd);
	}

//	for (size_t i = 0; i < newFeatures.size();++i)
//	{
//		for (size_t j = 0; j < newFeatures[i].size(); ++j)
//		{
//			wcout << newFeatures[i][j] << endl;
//		}
//	}
	crf->FindBestSequence(newFeatures, constraints, labelSequence);

	return labelSequence;
}



void SyntaxChunker::extractFeatures(const vector<wstring>& words, const vector<wstring>& featuresFromDisambiguator, const vector<wstring>& lexeme, vector<vector<wstring>>& featuresToCRF)
{
	unordered_map<wstring, wstring> SynTagRusToOC;
	unordered_map<wstring, wstring> cats;
	unordered_set<wstring> NPROS;

	SynTagRusToOC[SynTagRusTagset::POS_NOUN] = OpenCorpora::POS_NOUN;
	SynTagRusToOC[SynTagRusTagset::POS_VERB] = OpenCorpora::POS_VERB;

	SynTagRusToOC[SynTagRusTagset::POS_NUMR] = OpenCorpora::POS_NUMR;
	SynTagRusToOC[SynTagRusTagset::POS_ADVB] = OpenCorpora::POS_ADVB;
	SynTagRusToOC[SynTagRusTagset::POS_PREP] = OpenCorpora::POS_PREP;
	SynTagRusToOC[SynTagRusTagset::POS_CONJ] = OpenCorpora::POS_CONJ;
	SynTagRusToOC[SynTagRusTagset::POS_INTJ] = OpenCorpora::POS_INTJ;
	SynTagRusToOC[SynTagRusTagset::POS_UNKN] = OpenCorpora::POS_UNKN;

	SynTagRusToOC[SynTagRusTagset::POS_SENT] = OpenCorpora::POS_VERB;
	SynTagRusToOC[SynTagRusTagset::POS_PRCL] = OpenCorpora::POS_PRCL;

	SynTagRusToOC[OpenCorpora::POS_PREP] = OpenCorpora::POS_PREP;
	SynTagRusToOC[OpenCorpora::POS_PNCT] = OpenCorpora::POS_PNCT;

	SynTagRusToOC[SynTagRusTagset::C_NOM] = OpenCorpora::C_NOM;
	SynTagRusToOC[SynTagRusTagset::C_GEN1] = OpenCorpora::C_GEN;
	SynTagRusToOC[SynTagRusTagset::C_GEN2] = OpenCorpora::C_GEN;
	SynTagRusToOC[SynTagRusTagset::C_DAT] = OpenCorpora::C_DAT;
	SynTagRusToOC[SynTagRusTagset::C_ACC] = OpenCorpora::C_ACC;
	SynTagRusToOC[SynTagRusTagset::C_ABL] = OpenCorpora::C_ABL;
	SynTagRusToOC[SynTagRusTagset::C_LOC1] = OpenCorpora::C_LOC;
	SynTagRusToOC[SynTagRusTagset::C_LOC2] = OpenCorpora::C_LOC;

	SynTagRusToOC[SynTagRusTagset::G_MASC] = OpenCorpora::G_MASC;
	SynTagRusToOC[SynTagRusTagset::G_FEMN] = OpenCorpora::G_FEMN;
	SynTagRusToOC[SynTagRusTagset::G_NEUT] = OpenCorpora::G_NEUT;

	SynTagRusToOC[SynTagRusTagset::N_SING] = OpenCorpora::N_SING;
	SynTagRusToOC[SynTagRusTagset::N_PLUR] = OpenCorpora::N_PLUR;

	cats[SynTagRusTagset::POS_NOUN] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_VERB] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_NUMR] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_ADVB] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_PREP] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_CONJ] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_INTJ] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_UNKN] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_SENT] = PrivateTagset::CAT_POS;
	cats[SynTagRusTagset::POS_PRCL] = PrivateTagset::CAT_POS;
	cats[OpenCorpora::POS_PREP] = PrivateTagset::CAT_POS;
	cats[OpenCorpora::POS_PNCT] = PrivateTagset::CAT_POS;

	cats[SynTagRusTagset::C_NOM] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_GEN1] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_GEN2] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_DAT] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_ACC] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_ABL] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_LOC1] = PrivateTagset::CAT_CASE;
	cats[SynTagRusTagset::C_LOC2] = PrivateTagset::CAT_CASE;

	cats[SynTagRusTagset::G_MASC] = PrivateTagset::CAT_GEN;
	cats[SynTagRusTagset::G_FEMN] = PrivateTagset::CAT_GEN;
	cats[SynTagRusTagset::G_NEUT] = PrivateTagset::CAT_GEN;

	cats[SynTagRusTagset::N_SING] = PrivateTagset::CAT_NUM;
	cats[SynTagRusTagset::N_PLUR] = PrivateTagset::CAT_NUM;

	NPROS.insert(L"\u042f");//here was cyrrilic symbols: Я
	NPROS.insert(L"\u0422\u042b");//here was cyrrilic symbols: ТЫ
	NPROS.insert(L"\u041e\u041d");//here was cyrrilic symbols: ОН
	NPROS.insert(L"\u041e\u041d\u0410");//here was cyrrilic symbols: ОНА
	NPROS.insert(L"\u041a\u041e\u0422\u041e\u0420\u042b\u0419");//here was cyrrilic symbols: КОТОРЫЙ
	NPROS.insert(L"\u0427\u0415\u0419");//here was cyrrilic symbols: ЧЕЙ
	NPROS.insert(L"\u041e\u041d\u0418");//here was cyrrilic symbols: ОНИ
	NPROS.insert(L"\u041a\u0410\u041a\u041e\u0419");//here was cyrrilic symbols: КАКОЙ
	NPROS.insert(L"\u041c\u042b");//here was cyrrilic symbols: МЫ
	NPROS.insert(L"\u0412\u042b");//here was cyrrilic symbols: ВЫ
	NPROS.insert(L"\u0412\u0421\u0415");//here was cyrrilic symbols: ВСЕ
	NPROS.insert(L"\u042d\u0422\u041e");//here was cyrrilic symbols: ЭТО
	NPROS.insert(L"\u041a\u0422\u041e");//here was cyrrilic symbols: КТО
	NPROS.insert(L"\u0427\u0422\u041e");//here was cyrrilic symbols: ЧТО
	NPROS.insert(L"\u0422\u041e");//here was cyrrilic symbols: ТО
	LinguisticTools toolbox;

	for(size_t i = 0; i < lexeme.size(); i++)
	{
		unordered_map<wstring, wstring> features;
		vector<wstring> values = Tools::Split(
            featuresFromDisambiguator[i], L"@");

		bool underspec = false;

		for(auto value_p = values.begin(); value_p != values.end(); value_p++)
		{
			if(*value_p == SynTagRusTagset::POS_ADJ)
			{
				underspec = true;
			}

			else if(*value_p == SynTagRusTagset::D_SHORT)
			{
				underspec = false;
				features[PrivateTagset::CAT_POS] = OpenCorpora::POS_ADJS;
			}

			else if(SynTagRusToOC.find(*value_p) != SynTagRusToOC.end())
			{
				features[cats[*value_p]] = SynTagRusToOC[*value_p];
			}
		}

		if(underspec)
		{
			features[PrivateTagset::CAT_POS] = OpenCorpora::POS_ADJF;
		}

		if(NPROS.find(lexeme[i]) != NPROS.end())
		{
			features[PrivateTagset::CAT_POS] = OpenCorpora::POS_NPRO;
		}

        if(Tools::IsUpper(words[i].c_str()[0]))
		{
			features[L"UPPERCASE"] = L"TRUE";
		}

		featuresToCRF.push_back(vector<wstring>());
		for(auto p_feat = features.begin(); p_feat != features.end(); p_feat++)
		{
			wstring featStr = p_feat->first;
			featStr.append(L"=");
			featStr.append(p_feat->second);
			featuresToCRF[i].push_back(featStr);
		}

	}

	setConjunctions(featuresToCRF);
}

void SyntaxChunker::setConjunctions(vector<vector<wstring>>& simpleFeatures)
{
	vector<vector<wstring>> initial = simpleFeatures;

	initial.insert(initial.begin(), vector<wstring>());
	initial[0].push_back(L"<START0>");
	initial.push_back(vector<wstring>());
	initial[initial.size() - 1].push_back(L"<END0>");

	wstring AMP = L"_&_";

	for(size_t w = 1; w < initial.size() - 1; w++)
	{
		for(size_t f0 = 0; f0 < initial[w].size(); f0++)
		{
			for(size_t fm1 = 0; fm1 < initial[w - 1].size(); fm1++)
			{
				wstring a = initial[w - 1][fm1];
				a.append(L"@-1");
				a.append(AMP);
				a.append(initial[w][f0]);
				simpleFeatures[w - 1].push_back(a);
			}
			for(size_t f1 = 0; f1 < initial[w + 1].size(); f1++)
			{
				wstring a = initial[w][f0];
				a.append(AMP);
				a.append(initial[w + 1][f1]);
				a.append(L"@1");
				simpleFeatures[w - 1].push_back(a);
			}
		}

			for(size_t fm1 = 0; fm1 < initial[w - 1].size(); fm1++)
			{
				wstring a = initial[w - 1][fm1];
				a.append(L"@-1");
				simpleFeatures[w - 1].push_back(a);
			}
			for(size_t f1 = 0; f1 < initial[w + 1].size(); f1++)
			{
				wstring a = initial[w + 1][f1];
				a.append(L"@1");
				simpleFeatures[w - 1].push_back(a);
			}
	}
}


