/** 
 * @file	OpenCorporaToSynagrusConverter.h
 * @brief	header file for converter from OpenCorpora properties to Synatgrus properties
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


#ifndef OPENCORPORA_TO_SYNTAGRUS_CONVERTER
#define OPENCORPORA_TO_SYNTAGRUS_CONVERTER

#include "FileReader.h"
#include "Tools.h"
#include "TagsetConverterInterface.h"

namespace TagsetConverting
{

/** 
 * @class	OpenCorporaToSynagrusConverter
 * @brief	Converter from OpenCorpora properties to Synatgrus properties
 */
class OpenCorporaToSynagrusConverter : public TagsetConverterInterface
{
public:
	/**
     * @brief	Constructor
     * @param[in]	fileConverter - file containing rules of translations OpenCorpora features to Syntagrus
     */
	OpenCorporaToSynagrusConverter(
        const string& fileConverter);

	/**
     * @brief	Destructor
     */
	~OpenCorporaToSynagrusConverter(void);
	
	/**
     * @brief	Converts OpenCorpora tags to the vector of corresponding Syntagrus properties
     * @param[in]	opFeatures - OpenCorpora feature
     */
	vector<wstring> PartialConvert(const wstring& openCorporaTags);

private:
	map<wstring, wstring> opCorpPropertyTosyntagrusProperty; /**< map for converting OpenCorpora properties to Synatgrus properties*/
	map<wstring, vector<wstring>> pos_orders; /**< map containing priority of properties for each part of speach*/
};

};

#endif // OPENCORPORA_TO_SYNTAGRUS_CONVERTER
