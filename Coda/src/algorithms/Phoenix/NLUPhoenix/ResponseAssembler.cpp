/*
 * ResponseAssembeler.cpp
 *
 *  Created on: 22 \u0430\u0432\u0433. 2013 \u0433.//here was cyrrilic symbols: авг,г
 *      Author: mkudinov
 */

#include <vector>
#include <list>
#include "Tools.h"
#include "ResponseAssembler.h"
#include "sqliteClient/sql_table.h"
#include "sqliteClient/data_base_schema.h"
#include "commonExceptions.h"

#include <iostream>

using std::shared_ptr;
using std::string;
using std::wstring;
using std::vector;
using std::list;

namespace Phoenix
{
//description of database table where response templates are stored
//START **********************************************************

typedef Loki::TL::MakeTypelist<string, string, string, string>::Result ResponseTableColumnTypes;
typedef Loki::TL::MakeTypelist<string, string, string>::Result QueryColumnTypes;
typedef Loki::TL::MakeTypelist<string>::Result QueryResultTypes;

static const char * tableName = "ResponseTemplates";

enum ResponseColumn{
	frame_name,
	question,
	type,
	_template
};

typedef SqlTable<ResponseTableColumnTypes, string> ResponseTable;

static std::shared_ptr<ResponseTable> getResponseTable(const string& i_tableName)
{
	static const string columnNames[] =
	{
		"frame_name",
		"question",
		"type",
		"template"
	};

	std::shared_ptr<ResponseTable> table =
				DataBaseSchema::instance().getTable<ResponseTable>(i_tableName);

	if ( table == shared_ptr<ResponseTable>())
	{
		DataBaseSchema::instance().registrationTable(
			ResponseTable(
					i_tableName,
					vector<string>(columnNames, columnNames + sizeof(columnNames)/sizeof(string)),
					size_t(0)));
		table = DataBaseSchema::instance().getTable<ResponseTable>(i_tableName);
	}

	return table;
}

bool getTemplate(const string& i_tableName, const  DataTuple<QueryColumnTypes>& key, list<DataTuple<QueryResultTypes>>& o_value)
{
	try
	{
		vector<size_t> keyColInd;
		keyColInd.push_back(frame_name);
		keyColInd.push_back(question);
		keyColInd.push_back(type);

		vector<size_t> resColInd;
		resColInd.push_back(_template);

		shared_ptr<ResponseTable> featureTable = getResponseTable(i_tableName);
		o_value = featureTable->get<QueryResultTypes, QueryColumnTypes>(key, keyColInd, resColInd);
	}
	catch (const SQLiteException&) {
		return false;
	}
	return true;
}
//END**************************************************************

ResponseAssembler::ResponseAssembler(const string& _pathToDb)
{
	DataBaseSchema::instance().setDatabaseName(_pathToDb);

	responseData = list<unordered_map<string, shared_ptr<ISQLQuery>>>();
}

void ResponseAssembler::AddRow(const SQLColumnDataQueryCollections<List, std::shared_ptr<ISQLColumnDataQuery>>& responseRow)
{
	std::unordered_map<std::string, std::shared_ptr<ISQLQuery>> rowMap;

	for(auto column_p = responseRow.begin(); column_p != responseRow.end(); column_p++)
	{

		std::shared_ptr<ISQLColumnDataQuery> col = (*column_p);

		std::string colname = col->name();
		rowMap[colname] = col->value();
	}
	responseData.push_back(rowMap);
}

wstring ResponseAssembler::assembleRow(unordered_map<string, shared_ptr<ISQLQuery>> row)
{
	wstring toReturn = L"";
	size_t size1 = tinfo.dataPartsOfTemplate.size();
	size_t size2 = tinfo.textPartsOfTemplate.size();
	size_t from = 0;
	size_t upTo = (size1 >= size2) ? size1 :  size2;

	if(tinfo.beginsWithText)
	{
		toReturn = tinfo.textPartsOfTemplate[0];
		from++;
	}

	for(size_t i = 0; i < upTo; i++)
	{
		if(i < size1)
		{
			wstring fname = tinfo.dataPartsOfTemplate[i].functionName;
			vector<wstring> specialParameters = tinfo.dataPartsOfTemplate[i].specificParameters;
			vector<string> cnames = tinfo.dataPartsOfTemplate[i].columnNames;

			vector<shared_ptr<ISQLQuery>> fieldArgs;

			for(size_t j = 0; j < cnames.size(); j++)
			{
				string col_without_brackets = cnames[j].substr(1, cnames[j].length() - 2);
				fieldArgs.push_back(row[col_without_brackets]);
			}

			toReturn = toReturn.append(library.Transform(fname, fieldArgs, specialParameters)) + L" ";
		}

		if(i + from < size2)
		{
			toReturn = toReturn.append(tinfo.textPartsOfTemplate[i + from]);
		}
	}

	return toReturn;
}

wstring ResponseAssembler::insertMultipleStringsTemplate()
{
	return L"\u041a\u0430\u0436\u0435\u0442\u0441\u044f, \u0435\u0441\u0442\u044c \u043d\u0435\u0441\u043a\u043e\u043b\u044c\u043a\u043e \u0432\u0430\u0440\u0438\u0430\u043d\u0442\u043e\u0432. \u0412\u043e\u0442 \u043e\u043d\u0438. ";//here was cyrrilic symbols: Кажется,есть,несколько,вариантов,Вот,они
}

wstring ResponseAssembler::reportNoData()
{
	return L"Sorry, but I could not remember anything relevant. Would you clarify your question?";
}

wstring ResponseAssembler::CreateResponse(const FilledFrame& frameToResponse, const wstring& _question)
{
	wstring currentTemplate = getResponseTemplateFromDB(frameToResponse.GetSemanticFrame()->GetFrameName(), _question, TEMPLATE_TYPE_SIMPLE);

	vector<wstring> textPartsOfTemplate;
	vector<TemplateData> dataPartsOfTemplate;
	bool beginsWithText = false;

	tparser.Parse(currentTemplate, textPartsOfTemplate, dataPartsOfTemplate, beginsWithText);

	tinfo.textPartsOfTemplate = textPartsOfTemplate;
	tinfo.dataPartsOfTemplate = dataPartsOfTemplate;
	tinfo.beginsWithText = beginsWithText;

	size_t totRows = responseData.size();

	if(totRows == 0)
	{
		responseData.clear();
		return reportNoData();
	}
	else 
	{
		wstring toReturn;
		
		if(totRows > 1 )
			toReturn = getResponseTemplateFromDB(frameToResponse.GetSemanticFrame()->GetFrameName(), _question, TEMPLATE_TYPE_MULTIROW);

	//	toReturn = insertMultipleStringsTemplate();

		int resCounter = 0;

		for(auto row_p = responseData.begin(); row_p != responseData.end(); row_p++)
		{
			toReturn = toReturn.append(assembleRow(*row_p)) + L"  ";
			resCounter++;
			if(resCounter > 3)
			{
				toReturn.append(L"There are also other variants.");
				break;
			}
		}
		//toReturn = toReturn.substr(0, toReturn.length() - 7);
		responseData.clear();
		return toReturn;
	}
	return reportNoData();
}

std::wstring ResponseAssembler::getResponseTemplateFromDB(const wstring& frameName, const wstring& _question, const string& _templateType)
{
	std::list<DataTuple<QueryResultTypes> > rows;

	string sframeName = "";
	CONVERT_TO_UTF8_STRING(frameName, sframeName);

	string squestion = "";
	CONVERT_TO_UTF8_STRING(_question, squestion);

	string templateType = _templateType;

	string tabName(tableName);

	DataTuple<QueryColumnTypes> key;

	key.set<frame_name>(sframeName);
	key.set<question>(squestion);
	key.set<type>(templateType);

	getTemplate(tabName, key, rows);

	std::wstring currentTemplate = L"";

	for (std::list<DataTuple<QueryResultTypes> >::iterator i = rows.begin(); i != rows.end(); ++i)
	{
		currentTemplate = CONVERT_TO_WSTRING(i->get<0>());
	}

	if(rows.size() > 1)
	{
		throw EMultipleInterpretations(frameName.c_str());
	}
	else if(rows.size() == 0)
	{
		ENoInterpretation(frameName.c_str());
	}

	return currentTemplate;

}

} /* namespace Phoenix */
