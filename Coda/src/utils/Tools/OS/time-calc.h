/**
* @file	time-calc.h
* @brief	header file for cross-platform handling gettimeofday function
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


#ifndef _TIME_CALC_
#define _TIME_CALC_

#ifdef MSVC
#include <time.h>
#include <windows.h> 

#define DELTA_EPOCH_IN_MICROSECS  116444736000000000Ui64

/* FILETIME of Jan 1 1970 00:00:00, the PostgreSQL epoch */
static const unsigned __int64 epoch = 116444736000000000Ui64;

/*
* FILETIME represents the number of 100-nanosecond intervals since
* January 1, 1601 (UTC).
*/
#define FILETIME_UNITS_PER_SEC	10000000L
#define FILETIME_UNITS_PER_USEC	10

int gettimeofday(struct timeval * tp, struct timezone * tzp);

#else

#include <sys/time.h>

#endif // MSVC


#endif //_TIME_