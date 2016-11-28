/**
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

#ifndef TEMPLATEPARSER_H_
#define TEMPLATEPARSER_H_

#include <string>
#include <vector>
#include <iostream>

#include "Tools.h"

using std::wstring;
using std::vector;
using std::wcout;
using std::endl;

struct TemplateData
{
	wstring functionName;
	vector<string> columnNames;
	vector<wstring> specificParameters;

	TemplateData(const wstring& toParse);
};

class TemplateParser {
public:
	void Parse(
			const wstring& _template,
			vector<wstring>& text,
			vector<TemplateData>& toSubstitute,
			bool &isFirstText);
	TemplateParser();
	virtual ~TemplateParser();
};

#endif /* TEMPLATEPARSER_H_ */
