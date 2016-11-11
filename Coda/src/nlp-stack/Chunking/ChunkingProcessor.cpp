#include "ChunkingProcessor.h"

using std::shared_ptr;
using std::wstring;
using std::wcout;
using std::endl;
using std::tuple;

void ChunkingProcessor::print(const vector<wstring>& tokens, const vector<wstring>& labels)
{
//	for(size_t i = 0; i < tokens.size(); i++)
//	{
//		wcout << labels[i] << L" ";
//	}
//	wcout << endl;
	ChunkFormattingBrackets format(true);

	for(size_t i = 0; i < tokens.size(); i++)
	{
		wcout << format.FormatWord(tokens[i], labels[i]) << L" ";
	}

	wcout << format.LastSymbol() << endl;
}

ChunkingProcessor::ChunkingProcessor(
    const std::string& modelFile
    , shared_ptr<IDisambiguator> _disambiguator)
{
	chunker = shared_ptr<SyntaxChunker>(new SyntaxChunker(modelFile));
	disambiguator = _disambiguator;
}

//void ChunkingProcessor::Process(
//	const wstring& sentence, vector<wstring>& lemmas, 
//    vector<wstring>& tokens, vector<wstring>& stress,
//	vector<wstring>& tags)
//{
//	vector<vector<int>> groups;
//	disambiguator->Disambiguate(
//        sentence, lemmas, tokens, 
//        stress, tags, true);
//	vector<wstring> labels = chunker->Parse(sentence, lemmas, tags);
//	labels.erase(labels.begin());
//	LinguisticTools toolbox;
//	print(tokens, labels);
//}

void ChunkingProcessor::Process(
	const wstring& sentence,
	const vector<DisambiguatedData>& disambiguatedData)
{
	vector<vector<int>> groups;
	vector<wstring> lemmas;
	vector<wstring> tags;

	for(size_t i = 0; i < disambiguatedData.size(); i++)
	{
		lemmas.push_back(disambiguatedData[i].lemma);
		tags.push_back(disambiguatedData[i].label);
	}
	
	vector<wstring> labels = chunker->Parse(sentence, lemmas, tags);
	labels.erase(labels.begin());
	LinguisticTools toolbox;
	vector<wstring> tokens = toolbox.tokenize(sentence);
	print(tokens, labels);
}

ChunkFormattingBrackets :: ChunkFormattingBrackets(bool _markHeads)
{
	prevBIO = PrivateTagset::CHUNK_O;
	markHeads = _markHeads;
}

wstring ChunkFormattingBrackets ::  FormatWord(const wstring& token, const wstring& label)
{
	wstring bio = label;
	wstring prev = prevBIO;
	prevBIO = bio;

	wstring res0;

	if(label == L"BH" || label == L"IH")
	{
		res0 = token;
		res0.append(L"[X]");
	}
	else
	{
		res0 = token;
	}

	if(bio == L"I" || bio == L"IH")
	{
		return res0;
	}

	if(bio == L"B" || bio == L"BH")
	{
		if(prev != L"O")
		{
			return wstring(L"] [").append(res0);
		}
		else
		{
			return wstring(L"[").append(res0);
		}
	}

	if(prev != L"O")
	{
		return wstring(L"] ").append(res0);
	}

	return res0;
}

wstring ChunkFormattingBrackets :: LastSymbol()
{
	wstring res0 = L"";
	if (prevBIO != L"O")
		res0 = L"]";
	prevBIO = L"O";
	return res0;
}
