#ifndef RTA_SENTENCE_PROCESSOR_H
#define RTA_SENTENCE_PROCESSOR_H

#include <memory>
#include <iostream>
#include <string>
#include <tuple>

#include "IDisambiguator.h"
#include "SyntaxChunker.h"
#include "tagsetConstants.h"

using std::shared_ptr;
using std::wstring;
using std::wcout;
using std::endl;

using Disambiguation::DisambiguatedData;
using Disambiguation::IDisambiguator;

class ChunkingProcessor
{
public:
	ChunkingProcessor(
        const std::string& modelFile
        , shared_ptr<IDisambiguator> disambiguator);
	~ChunkingProcessor() {};

	void Process(
		const wstring& sentence,
		const vector<DisambiguatedData>& disambiguatedData);

private:
    shared_ptr<IDisambiguator> disambiguator;
	std::shared_ptr<SyntaxChunker> chunker;
	void print(
        const std::vector<std::wstring>& tokens
        , const std::vector<std::wstring>& labels);
};

class ChunkFormattingBrackets
{
public:
	ChunkFormattingBrackets(bool _markHeads);
	wstring FormatWord(const std::wstring& token, const std::wstring&);
	wstring LastSymbol();
private:
	wstring prevBIO;
	bool markHeads;
};

#endif

