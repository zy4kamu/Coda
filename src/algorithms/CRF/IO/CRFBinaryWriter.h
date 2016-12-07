/** 
 * @file	CRFBinaryWriter.h
 * @brief	header file of the CRF-binary writer
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


#ifndef CRFBINARYWRITER
#define CRFBINARYWRITER

#include "CRF.h"

namespace LinearCRF
{

/**
 * @class	CRFBinaryWriter
 * @brief	Class for the CRF-binary writer
 */
class CRFBinaryWriter
{
public:
	
	/**
	 * @brief	Constructor
	 * @param[in]	_file - file where to write the model
	 */
	CRFBinaryWriter(const string& _file);

	/**
	 * @brief	Destructor
	 */
	~CRFBinaryWriter(void);

	/**
	 * @brief	Writes the model to file
	 * @param[in]	crf - the CRF-model
	 */
	void Write(const CRF& crf) const;
private:
	string file; /**< file where to write the model*/
};

};

#endif // CRFBINARYWRITER

