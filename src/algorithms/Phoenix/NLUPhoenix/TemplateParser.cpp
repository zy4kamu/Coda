/*
 * TemplateParser.cpp
 *
 *  Created on: 23 \u0430\u0432\u0433. 2013 \u0433.//here was cyrrilic symbols: авг,г
 *      Author: stepan
 */

#include "TemplateParser.h"

TemplateData::TemplateData(const wstring& toParse)
{
	set<wchar_t> splitChars;
	splitChars.insert(L'(');
	splitChars.insert(L')');
	vector<wstring> splitted = Tools::Split(toParse, splitChars);
	if (splitted.size() != 3)
	{
		wcout << L"TemplateParser::TemplateData: wrong input wstring";
		throw "TemplateParser::TemplateData: wrong input wstring";
	}

	functionName = splitted[0];

    splitted = Tools::Split(splitted[1], L",");
	for (size_t i = 0; i < splitted.size(); ++i)
	{
		if (splitted[i][0] == L'[')
		{
			string aa = "";
			CONVERT_TO_UTF8_STRING(splitted[i], aa);
			columnNames.push_back(aa);
		}
		else specificParameters.push_back(splitted[i]);
	}
}

TemplateParser::TemplateParser() {
	// TODO Auto-generated constructor stub

}

TemplateParser::~TemplateParser() {
	// TODO Auto-generated destructor stub
}

void TemplateParser::Parse(
		const wstring& _template,
		vector<wstring>& text,
		vector<TemplateData>& toSubstitute,
		bool &isFirstText)
{
	text.clear();
	toSubstitute.clear();

    wstring data = Tools::Remove(_template, L'$');
	set<wchar_t> splitChars;
	splitChars.insert(L'{');
	splitChars.insert(L'}');
	vector<wstring> splitted = Tools::Split(data, splitChars);

	for (size_t i = 0; i < splitted.size(); i +=2)
		text.push_back(splitted[i]);

	for (size_t i = 1; i < splitted.size(); i +=2)
	{
		toSubstitute.emplace_back(splitted[i]);
	}
	isFirstText = true;
}

