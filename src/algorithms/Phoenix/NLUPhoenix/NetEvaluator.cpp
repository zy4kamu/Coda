/** 
 * @file	NetEvaluator.cpp
 * @brief	The file contains description of class NetEvaluator.
 */

#define _CRT_SECURE_NO_WARNINGS

#include "NetEvaluator.h"
#include "Tools.h"

using std::string;
using std::wstring;
using std::unordered_map;
using std::vector;
using std::shared_ptr;

namespace Phoenix
{

wstring NetEvaluator::VALUE = L"VALUE";

NetEvaluator::NetEvaluator(const string& programFile)
{
	initConstants();
	readConfig(programFile);
}

void NetEvaluator::readConfig(const string& programFile)
{
	wifstream in(programFile);

	wstring str;
	vector<wstring> splitted;
	
	while (getline(in, str))
	{
        splitted = Tools::Split(str, L":");
		if (splitted.size() != 2)
			throw EBadFileFormat(programFile.c_str());
        wstring left = Tools::Trim(splitted[0]);
        wstring right = Tools::Trim(splitted[1]);
		if(left.size() < 1 || right.size() < 1 )
			throw EBadFileFormat(programFile.c_str());
		addToTable(left, right);
	}
	in.close();
}

void NetEvaluator::addToTable(const wstring& leftValue, const wstring& rightValue)
{
	if(functionTable.find(leftValue) != functionTable.end())
	{
		throw ESymbolRedefinition(leftValue.c_str());
	}

	functionTable[leftValue] = rightValue;
}

void NetEvaluator::initConstants()
{
	DICTIONARY_ENTRY = L"$TERM_DICTIONARY";
	TIMEX = L"$TIMEX";
}

unordered_map<wstring, wstring> NetEvaluator::Evaluate(const shared_ptr<IMatch>& netToEval, Tense tense) const
{
	wstring name = netToEval->GetNetName();

	if(functionTable.find(name) == functionTable.end()) //if net is not described in net_interp we by default just compose result of the results of the children
	{
		return defaultEvaluation(std::dynamic_pointer_cast<ChartCell>(netToEval)->GetMatch(), tense);
	}

	if(functionTable.at(name)[0] != L'$') //if the rule is [LEFT]: right, then we just return constant "right".
	{
		unordered_map<wstring, wstring> toReturn;
		toReturn[VALUE] = functionTable.at(name);
		return  toReturn;
	}

	if(functionTable.at(name) == DICTIONARY_ENTRY) //for rules with $TERM_DICTIONARY we must return dictionary form of the matched phrase 
	{
		return dictionaryEvaluation(std::dynamic_pointer_cast<ChartCell>(netToEval)->GetMatch());
	}

	if(functionTable.at(name) == TIMEX) //timex's have therir own processing function
	{
		return timexEvaluation(std::dynamic_pointer_cast<TimexMatch>(netToEval), tense);
	}

	return defaultEvaluation(std::dynamic_pointer_cast<ChartCell>(netToEval)->GetMatch(), tense);
}

unordered_map<wstring, wstring> NetEvaluator::evaluate(const shared_ptr<MatchTree>& netToEval, Tense tense) const
{
	wstring name = netToEval->GetNet()->GetName(); //we got here because we are processing normal Phoenix frame and can do evaluation recursively 

	if(functionTable.find(name) == functionTable.end())
	{
		return defaultEvaluation(netToEval, tense);
	}

	if(functionTable.at(name)[0] != L'$')
	{
		unordered_map<wstring, wstring> toReturn;
		toReturn[VALUE] = functionTable.at(name);
		return  toReturn;
	}

	if(functionTable.at(name) == DICTIONARY_ENTRY)
	{
		return dictionaryEvaluation(netToEval);
	}

	return defaultEvaluation(netToEval, tense);
}

unordered_map<wstring, wstring> NetEvaluator::defaultEvaluation(const shared_ptr<MatchTree>& netToEval, Tense tense) const
{
	unordered_map<wstring, wstring> toReturn;
	toReturn[VALUE] = L"";

	for(auto child_p = netToEval->GetChildMatchesIterator(); child_p != netToEval->GetChildMatchesEndIterator(); child_p++) 
	{
		toReturn[VALUE].append(evaluate(*child_p, tense)[VALUE]);
	}

	return toReturn;
}

unordered_map<wstring, wstring> NetEvaluator::dictionaryEvaluation(const shared_ptr<MatchTree>& netToEval) const
{
	unordered_map<wstring, wstring> toReturn;
	toReturn[VALUE] = L"";
	{
		for(size_t i = 0; i < netToEval->GetMatchedWordEntries().size() - 1; i++)
		{
			toReturn[VALUE].append(netToEval->GetMatchedWordEntries()[i]);
			toReturn[VALUE].append(L" ");
		}
		toReturn[VALUE].append(netToEval->GetMatchedWordEntries()[netToEval->GetMatchedWordEntries().size() - 1]);
	}

	return toReturn;
}

unordered_map<wstring, wstring> NetEvaluator::timexEvaluation(const std::shared_ptr<TimexMatch>& argument, Tense tense) const
{
	unordered_map<wstring, wstring> toReturn;

	TimexFormatConverter tconv;

	wstring slotName;

	vector<tm> timePoints = tconv.ReturnTimeAndDateForPhoenix(argument->GetSlots(), tense, slotName); //time points are calculated according to time feature of the sentence

	int i = 1;

	//then the time points are converted to the form in which the query template is written, ex: [NET].FIELD1, [NET].FIELD2...
	for(auto tm_p = timePoints.begin(); tm_p != timePoints.end(); tm_p++)
	{
		wstring time;
		wstring date;
		if(tm_p->tm_hour == -1)
				tm_p->tm_hour = 0;
		if(tm_p->tm_min == -1)
				tm_p->tm_min = 0;
		if(tm_p->tm_hour < 10)
		{
			time = L"0" + std::to_wstring((unsigned long long)tm_p->tm_hour) + L":";
		}
		else
		{
			time = std::to_wstring((unsigned long long)tm_p->tm_hour) + L":";
		}

		if(tm_p->tm_min < 10)
		{
			time = time + L"0" + std::to_wstring((unsigned long long)tm_p->tm_min);
		}
		else
		{
			time = time + std::to_wstring((unsigned long long)tm_p->tm_min);
		}

		date = std::to_wstring((unsigned long long)(tm_p->tm_year + 1900)) + L"-";

		if(tm_p->tm_mon < 9)
		{
			date = date + L"0" + std::to_wstring((unsigned long long)tm_p->tm_mon + 1) + L"-";
		}
		else
		{
			date = date + std::to_wstring((unsigned long long)tm_p->tm_mon + 1) + L"-";
		}

		if(tm_p->tm_mday < 10)
		{
			date = date + L"0" + std::to_wstring((unsigned long long)tm_p->tm_mday);
		}
		else
		{
			date = date + std::to_wstring((unsigned long long)tm_p->tm_mday);
		}

		wstring ind = std::to_wstring((unsigned long long)i);

		//or in this case [TIMEX].TIME1, [TIMEX].DATE1...
		wstring timeCode = L"TIME" + ind;
		wstring dateCode = L"DATE" + ind;
		
		toReturn[timeCode] = time;
		toReturn[dateCode] = date;
		i++;
	}

	return toReturn;
}

} //namespace Phoenix
