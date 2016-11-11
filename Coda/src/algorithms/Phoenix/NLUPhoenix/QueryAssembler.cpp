/** 
 * @file	QueryAssembler.cpp
 * @brief	The file contains description of class QueryAssembler.
 */

#include <cstring>
#include <list>

#include "QueryAssembler.h"
#include "sqliteClient/sql_table.h"
#include "sqliteClient/data_base_schema.h"
#include "commonExceptions.h"
#include "FileReader.h"

using std::list;
using std::wstring;
using std::vector;
using std::shared_ptr;
using std::unordered_map;

namespace Phoenix
{

//description of database table where response templates are stored
//START **********************************************************

typedef Loki::TL::MakeTypelist<string, string>::Result QueryTemplateTableColumnTypes;
typedef Loki::TL::MakeTypelist<string>::Result QueryColumnTypes;
typedef Loki::TL::MakeTypelist<string>::Result QueryResultTypes;

static const char * tableName = "QueryTemplates";

enum ResponseColumn{
	frame_name,
	_template
};

typedef SqlTable<QueryTemplateTableColumnTypes, string> QueryTemplateTable;

static std::shared_ptr<QueryTemplateTable> getQueryTable(const string& i_tableName)
{
	static const string columnNames[] =
	{
		"frame_name",
		"template"
	};

	std::shared_ptr<QueryTemplateTable> table =
				DataBaseSchema::instance().getTable<QueryTemplateTable>(i_tableName);

	if ( table == shared_ptr<QueryTemplateTable>())
	{
		DataBaseSchema::instance().registrationTable(
				QueryTemplateTable(
					i_tableName,
					vector<string>(columnNames, columnNames + sizeof(columnNames)/sizeof(string)),
					size_t(0)));
		table = DataBaseSchema::instance().getTable<QueryTemplateTable>(i_tableName);
	}

	return table;
}

bool getTemplate(const string& i_tableName, const  DataTuple<QueryColumnTypes>& key, list<DataTuple<QueryResultTypes>>& o_value)
{
	try
	{
		vector<size_t> keyColInd;
		keyColInd.push_back(frame_name);

		vector<size_t> resColInd;
		resColInd.push_back(_template);

		shared_ptr<QueryTemplateTable> featureTable = getQueryTable(i_tableName);
		o_value = featureTable->get<QueryResultTypes, QueryColumnTypes>(key, keyColInd, resColInd);
	}
	catch (const SQLiteException&) {
		return false;
	}
	return true;
}

QueryAssembler::QueryAssembler(const string& pathToDb)
{
	DataBaseSchema::instance().setDatabaseName(pathToDb);
}

wstring QueryAssembler::CreateQuery(const FilledFrame& targetFrame, const NetEvaluator& evaluator, std::wstring& question)
{
	wstring toReturn = L"";

	currentFrameTense = targetFrame.GetTense();

	wstring qTemplate = getQueryTemplateFromDB(targetFrame.GetSemanticFrame()->GetFrameName(), pathToDb);

	queryTemplate = parseTemplate(qTemplate);

	vector<wstring> queryParts;
	queryParts.push_back(L"");

	wstring queryClause;
	bool timeSpecified = false;

	for(auto slot_p = targetFrame.GetSlotIterator(); slot_p != targetFrame.GetSlotEndIterator(); slot_p++)
	{
		if((*slot_p)->GetNetName() == CONST_QUERY) //if this is a [Query] slot, we extract key word for question type for it: why, where, when...
		{
			question = evaluator.Evaluate(*slot_p, currentFrameTense)[NetEvaluator::VALUE];
		}

		try
		{
			if(queryTemplate.find((*slot_p)->GetNetName()) != queryTemplate.end())
			{
				queryClause = interpolate(queryTemplate.at((*slot_p)->GetNetName()), *slot_p, evaluator);
				if((*slot_p)->IsExternal()) //now it just means that we encountered time expression
				{
					timeSpecified = true;
				}

				if(queryClause.find(L"SELECT") == 0) //we reserve first position for the SELECT clause
				{
					if(queryClause.find(L"WHERE") == wstring::npos)
						queryClause.append(L" WHERE 1=1");
					queryParts[0] = queryClause;
				}
				else if(!queryClause.empty()) // other parts of the query are joined by conjunction
				{
					queryClause = wstring(L" AND ").append(queryClause);
					queryParts.push_back(queryClause);
				}
			}
		}
		catch(...)
		{
			throw EInvalidTemplate(targetFrame.GetSemanticFrame()->GetFrameName().c_str());
		}
	}

	if(queryParts[0].empty()) //if we didn't find any slot which could determine source table we force select from Afisha
	{
		//throw 0;
		queryParts[0] = L"SELECT Year, Sport, Event, Athlete, Result, Medal, Country, Country_Taker, City_Taker, Season, COUNT(*) as Times FROM OlympData WHERE 1=1";
	}

	if(!timeSpecified) //if we didn't find any slot which could determine time we set default timex slot based on tense
	{
		Slots def_timex = createDefaultTimex(currentFrameTense);

		shared_ptr<IMatch> def_time_match = shared_ptr<IMatch>(new TimexMatch(def_timex));

		//Print("/home/stepan/zz.txt", queryTemplate);
		wstring sdklfdjfdskljfds = def_time_match->GetNetName();
		queryClause = L" AND " + interpolate(queryTemplate.at(def_time_match->GetNetName()), def_time_match, evaluator);
		
		queryParts.push_back(queryClause);
	}

	for(size_t i = 0; i < queryParts.size(); i++)
	{
		toReturn.append(queryParts[i]);
	}

	return toReturn;
}

wstring QueryAssembler::getQueryTemplateFromDB(const wstring& frameName, const string& pathToDb)
{
	DataTuple<QueryColumnTypes> key;

	std::list<DataTuple<QueryResultTypes>> rows;

	string sframeName = "";

	string tabName(tableName);

	CONVERT_TO_UTF8_STRING(frameName,  sframeName);

	key.set<frame_name>(sframeName);

	getTemplate(tabName, key, rows);

	if(rows.size() == 0)
	{
		throw ENoInterpretation(frameName.c_str());
	}

	if(rows.size() > 1)
	{
		throw EMultipleInterpretations(frameName.c_str());
	}

	wstring wtemplate = L"";

	for (std::list<DataTuple<QueryResultTypes> >::iterator i = rows.begin(); i != rows.end(); ++i)
	{
		wtemplate =  CONVERT_TO_WSTRING(i->get<0>());
	}

	return wtemplate;
}

wstring QueryAssembler::interpolate(const wstring& rule, const shared_ptr<IMatch>& match, const NetEvaluator& evaluator)
{
	unordered_map<wstring, wstring> evaluation = evaluator.Evaluate(match, currentFrameTense); //evaluator calculates all values which could be interpolated into the template

	wstring filler;
	
	wstring toReturn = rule;

	size_t c_pos = 0;
	size_t pos = 0;

	while((pos = rule.find(L"${", c_pos)) != wstring::npos) //replace all placeholders in the template with calculated values
	{
		size_t pos2 = rule.find(L"}", pos);
		if(pos2 == wstring::npos)
		{
			throw EInvalidSlotRule(match->GetNetName().c_str());
		}

		wstring placeHolder = rule.substr(pos + 2, pos2 - pos - 2);

		size_t point_pos;

		if((point_pos = placeHolder.find(L".")) != wstring::npos) //reference to subfield ".field" is supported
		{
			wstring field = placeHolder.substr(point_pos + 1);
			if(evaluation.find(field) == evaluation.end())
			{
				throw ENoExistingFieldReference(match->GetNetName().c_str());
			}
			filler = evaluation.at(field);
		}
		else
		{
			filler = evaluation.at(NetEvaluator::VALUE); //if no field is referenced field .VALUE is referenced by default
		}

		c_pos = pos2;

		size_t repl1 = toReturn.find(L"${");
		size_t repl2 = toReturn.find(L"}") + 1;

		toReturn.replace(repl1, repl2 - repl1, filler);
	}

	return toReturn;
}

unordered_map<wstring, wstring> QueryAssembler::parseTemplate(const wstring& templateString)
{
	unordered_map<wstring, wstring> toReturn;

	vector<wstring> lines 
        = Tools::Split(templateString, L"\n");

	for(size_t i = 0; i < lines.size(); i++)
	{
		int s = lines[i].find_first_of(L":");

		wstring right = lines[i].substr(s + 1);
		
		right = right.substr(right.find_first_not_of(L" "));

		wstring left = lines[i].substr(0, s);

		toReturn[left] = right;
	}

	return toReturn;
}

Slots QueryAssembler::createDefaultTimex(Tense tense)
{
	Slots toReturn;

	switch(tense)
	{
	case PAST:
		//create structure for 'now'
		toReturn.time.timeofday = NOW;
		break;
	case PRESENT:
		//create structure for 'today'
		//toReturn.date.spec_day = TODAY;
		toReturn.time.timeofday = NOW;
		break;
	case FUTURE:
		//create structure for 'today'
		toReturn.date.spec_day = TODAY;
		break;
	}

	return toReturn;
}

} //namespace Phoenix
