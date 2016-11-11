#ifndef RTA_SYNTAX_CHUNKER_H
#define RTA_SYNTAX_CHUNKER_H

#include "CRF.h"
#include "LinguisticTools.h"
#include "tagsetConstants.h"
#include "Tools.h"

#include <memory>
#include <string>
#include <unordered_set>
#include <unordered_map>
#include <locale>

using std::string;
using std::wstring;
using std::unordered_set;
using std::unordered_map;

class SyntaxChunker
{
public:
	SyntaxChunker(const std::string& pathToModel);
	std::vector<std::wstring> Parse(
        const std::wstring& inputSentence, 
        const std::vector<std::wstring>& lexemes, 
        const std::vector<wstring>& features);
	~SyntaxChunker() {};
private:
    LinearCRF::CRF * crf;
	void extractFeatures(const std::vector<std::wstring>& words, const std::vector<std::wstring>& lexemes, const std::vector<std::wstring>& featuresFromDisambiguator, std::vector<std::vector<std::wstring>>& featuresToCRF);
	void setConjunctions(std::vector<std::vector<std::wstring>>& simpleFeatures);
};

#endif

