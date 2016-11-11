/** 
 * @file	commonExceptions.h 
 * @brief	Exceptions used by different classes insede Phoenix namespace
 * @details Initially (and possibly mostly or even only) consisted(s) of exceptions thrown during I/O operations 
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


#ifndef PHOENIX_COMMON_EXCEPTIONS
#define PHOENIX_COMMON_EXCEPTIONS

namespace Phoenix
{

/**
@class EFileNotFound 
@brief Exception. One of the configuration files was not found 
@author M.Kudinov, SRR
 */
class EFileNotFound
{
public:
	EFileNotFound(const char * filename)
    {
		fprintf(stderr, "ERROR: File not found: %s \n", filename);
    }
};

/**
@class EBadFileFormat 
@brief Exception thrown if the configuration file is ill-formed
@author M.Kudinov, SRR
 */
class EBadFileFormat
{
public:
	EBadFileFormat(const char * filename) {fprintf(stderr, "ERROR: Bad file format: %s \n", filename);};
};

/**
@class EDataBaseError 
@brief Exception thrown if the configuration file is ill-formed
@author M.Kudinov, SRR
 */
class EDataBaseError
{
public:
	EDataBaseError(const char * errmsg) {fprintf(stderr, "ERROR: Database caused error: %s \n", errmsg);};
};

class ENoResource
{
public:
	ENoResource() {fwprintf(stderr, L"ERROR: No resource for query templates was specified");}
};

class ENoInterpretation
{
public:
	ENoInterpretation(const wchar_t * frameName) {fwprintf(stderr, L"ERROR: No frame template specified for the frame %s\n", frameName);}
};

class EMultipleInterpretations
{
public:
	EMultipleInterpretations(const wchar_t * frameName) {fwprintf(stderr, L"ERROR: multiple templates defined for the frame %s\n", frameName);}
};

class EInvalidSlotRule
{
public:
	EInvalidSlotRule(const wchar_t * slot) {fwprintf(stderr, L"ERROR: syntax error in rule definition for slot %s\n", slot);}
};

class EInvalidTemplate
{
public:
	EInvalidTemplate(const wchar_t * frameName) {fwprintf(stderr, L"ERROR: syntax error in template definition for frame %s\n", frameName);}
};

class ENoExistingFieldReference
{
public:
	ENoExistingFieldReference(const wchar_t * slot) {fwprintf(stderr, L"ERROR: no-existing field is referenced during evaluation of slot %s\n", slot);}
};

} //namespace Phoenix

#endif // PHOENIX_COMMON_EXCEPTIONS
