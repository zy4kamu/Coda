/*
 * WORKER.h
 *
 *  Created on: 20 \u0441\u0435\u043d\u0442. 2013 \u0433.//here was cyrrilic symbols: сент,г
 *      Author: stepan
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


#ifndef WORKER_H_
#define WORKER_H_

#include <unordered_map>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include "SynTagRusTagInterpreter.h"
#include "tagsetConstants.h"
#include "PhoenixParser.h"
#include "PhoenixSlotGraph.h"
#include "dataTools.h"
#include "Word.h"
#include "DisambiguatorInterface.h"
#include "FilledFrame.h"
#include "NetEvaluator.h"
#include "QueryAssembler.h"
#include "ResponseAssembler.h"
#include "sqliteClient/sqlite_helper.h"
#include "TimexNormalizer.h"
#include "CommandInterpreter.h"

using namespace Phoenix;

using std::string;
using std::wstring;
using std::shared_ptr;
using std::wcout;

namespace Phoenix {

class WORKER {
public:
	WORKER(const string& path);
	wstring ParseSentence(const wstring& sentence);
	virtual ~WORKER();
private:
	void readConfig(const string& path);
	string dict_file;
	string grammar_file;
	string frames_file;
	string disamb_dir;
	string database_file;
	string net_interp_file;

	shared_ptr<Grammar> G;
	shared_ptr<SlotTrellis> GS;
	shared_ptr<PhoenixParser> parser;
	shared_ptr<QueryAssembler> qa;
	shared_ptr<NetEvaluator> evaluator;
	shared_ptr<ResponseAssembler> ra;
	TimexNormalizer normalizer;
	CommandInterpreter com_interp;
};

}

#endif /* WORKER_H_ */
