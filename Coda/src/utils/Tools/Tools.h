/**
 * @file	Tools.h
 * @brief	header file containing common-used functions (splitting, trimming, sorting etc.)
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


#ifndef TOOLS
#define TOOLS

#include "CrossPlatformHeader.h"
#include "DataFunctors.h"

#include "Containers/Alphabet.h"
#include "Containers/ContainerConverters.hpp"
#include "Containers/ContainerEraser.h"
#include "Containers/ContainerGenerators.hpp"
#include "Containers/ContainerInserter.h"
#include "Containers/ContainerSearcher.hpp"
#include "Containers/ContainerSorter.hpp"
#include "Containers/ContainerTools.h"
#include "Containers/Document.h"
#include "Containers/SparseVector.h"
#include "Containers/TypeTraits.h"
#include "Containers/Trie.h"

#include "Exceptions.h"

#include "IO/BinaryIO.h"
#include "IO/ConsoleTools.h"

#include "Locale/Language.h"
#include "Locale/LocaleSettings.hpp"

#include "Math/TableCalculator.hpp"

#include "OS/FixedSizeConnecter.hpp"
#include "OS/FloatSizeConnecter.hpp"
#include "OS/OS.h"
#include "OS/ProcessClient.hpp"
#include "OS/ProcessServer.hpp"
#include "OS/ProcessInetClient.hpp"
#include "OS/ProcessInetServer.hpp"
#include "OS/PythonFloatSizeConnecter.hpp"
#include "OS/time-calc.h"

#include "Locale/RussianLocale.h"

#include "Strings/AhoCorasickParser.hpp"
#include "Strings/StringConvertions.h"
#include "Strings/StringTools.h"

#include "Strings/Splitter.h"

#endif // TOOLS
