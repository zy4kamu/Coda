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

#ifndef SLOTINFLECTOR_H_
#define SLOTINFLECTOR_H_

#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>
#include <vector>

#include "Tools.h"
#include "sqliteClient/sqlite_string_processor_base.h"

class SlotTransformer;

typedef std::vector<std::shared_ptr<ISQLQuery> > QUERY_MAP;
typedef std::wstring (SlotTransformer::* FUNC)(const QUERY_MAP&, const std::vector<std::wstring>&);

class SlotTransformer
{
	std::unordered_map<std::wstring, FUNC> name_FUNC;
public:
	SlotTransformer();
	virtual ~SlotTransformer();
	std::wstring Transform(
			const std::wstring& function,
			const QUERY_MAP& fields,
			const std::vector<std::wstring>& specialParameters);
private:
	template <class T>
	T getField(int index, const QUERY_MAP&);

	std::wstring convertHours(const QUERY_MAP& fields, const std::vector<std::wstring>& specialParameters);
	std::wstring normalizeTime(const QUERY_MAP& fields, const std::vector<std::wstring>& specialParameters);
	std::wstring inflectPhrase(const QUERY_MAP& fields, const std::vector<std::wstring>& specialParameters);
};

template <class T>
T SlotTransformer::getField(int index, const QUERY_MAP& fields)
{
	std::shared_ptr<ISQLQuery> item = fields[index];
	return (std::dynamic_pointer_cast<SQLLiteralQuery<T> >(item))->value();

//	QUERY_MAP::const_iterator it = fields.find(name);
//	if (it == fields.end())
//	{
//		wcout << L"SlotTransformer::getField<T>: Wrong input fields" << std::endl;
//		throw "SlotTransformer::getField<T>: Wrong input fields";
//	}
//	return (std::dynamic_pointer_cast<SQLLiteralQuery<T> >(it->second->value()))->value();
}

#endif /* SLOTINFLECTOR_H_ */
