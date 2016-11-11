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

#ifndef PHOENIX_H_DBTABLES
#define PHOENIX_H_DBTABLES

#include "sqliteClient/sql_table.h"
#include "sqliteClient/data_base_schema.h"
#include <string>

namespace Phoenix
{
//Afisha table***************************************************************

typedef Loki::TL::MakeTypelist<int64_t, std::string, std::string, std::string, std::string>::Result ProgrammesColumnTypes;

enum ProgrammesColumn{
Chanel,
Time,
Overview,
Detail
};

typedef SqlTable<ProgrammesColumnTypes, int64_t> ProgrammesTable;

std::shared_ptr<ProgrammesTable > getProgrammesTable(const std::string& i_tableName);

template<class TList>
bool readProgramm(const std::string& i_tableName, const std::string& query, std::list< DataTuple<TList> >& o_value){
	try {
		std::shared_ptr<ProgrammesTable> featureTable = getProgrammesTable(i_tableName);
		o_value = featureTable->get<TList>(query);
	}
	catch (const SQLiteException&) {
		return false;
	}
	return true;
}

#if 0
bool writeGlobalFeature(const std::string& i_tableName, int64_t i_imageID, const cv::Mat& i_value){
	try {
		SHARED_PTR<GlobaFeatureTable> featureTable = getGlobaFeatureTable(i_tableName);
		featureTable->insert(i_imageID, 0, i_imageID);
		featureTable->insert(i_imageID, 1, serialization(i_value));
	}
	catch (const SQLiteException&) {
		return false;
	}
	return true;
}
#endif

//***************************************************************************

} //namespace Phoenix

#endif //PHOENIX_H_DBTABLES
