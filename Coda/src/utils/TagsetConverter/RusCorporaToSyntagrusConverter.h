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

#ifndef RUSCORPORATOSYNTAGRUSCONVERTER
#define RUSCORPORATOSYNTAGRUSCONVERTER

#include <unordered_map>

#include "FileReader.h"
#include "Tools.h"
#include "TagsetConverterInterface.h"

using std::unordered_map;

namespace TagsetConverting
{

class RusCorporaToSyntagrusConverter : public TagsetConverterInterface
{
public:
	RusCorporaToSyntagrusConverter(const string& file);
	~RusCorporaToSyntagrusConverter(void);
	vector<wstring> PartialConvert(const wstring& tag);
	vector<wstring> PartialConvert(const wstring& token, 
        const wstring& tag);
private:
	unordered_map<wstring, wstring> mapConverter;
	unordered_map<wstring, vector<wstring> > pos_orders;
	void correctPredictedLabel(const wstring& token, 
        vector<wstring>& convertedTags);
};

};

#endif

