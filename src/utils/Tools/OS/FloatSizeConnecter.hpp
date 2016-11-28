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

#ifndef FLOAT_SIZE_CONNECTER
#define FLOAT_SIZE_CONNECTER

#ifdef MSVC

#else

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/un.h>
#include <stdio.h>
#include <iostream>
#include <string>

namespace Tools
{

template<class SyzeType = int>
class FloatSizeConnecter
{
public:
    string Get(int connectionIndex)
    {
        try {
            SyzeType bufferSize;
            read(connectionIndex, &bufferSize, sizeof(SyzeType));

            char message[bufferSize + 1];
            memset(message, 0, bufferSize + 1);
            read(connectionIndex, message, bufferSize);
            return string(message);
        } catch(...) {
            throw ProcessServerConnectionException();
        }
    }

    void Send(const string& responce, int connectionIndex)
    {
        SyzeType size = static_cast<SyzeType>(responce.size());
        write(connectionIndex, &size, sizeof(SyzeType));
        write(connectionIndex, responce.c_str(), responce.size());
    }
};

}
#endif

#endif // FLOAT_SIZE_CONNECTER

