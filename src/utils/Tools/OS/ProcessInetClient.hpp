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

#ifndef PROCESS_INET_CLIENT
#define PROCESS_INET_CLIENT

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
#include <arpa/inet.h>

namespace Tools
{

using std::string;

//static const string RESPONSE_ERROR_MESSAGE = "bad responce message";
//static const string END_SESSION_STRING = "end session";

template<class Connector>
class ProcessInetClient
{
public:
    ProcessInetClient(const string server_ip, int port, size_t numberOfTrials = size_t(-1));
    ~ProcessInetClient();
    void Send(const string& request);
    void Close();
    string Get();
private:
    Connector connector;
    int socketIndex;
};

template<class Connector>
ProcessInetClient<Connector>::ProcessInetClient(const string server_ip, int port, size_t numberOfTrials)
{
    socketIndex = socket(AF_INET, SOCK_STREAM, 0);
    if (socketIndex < 0)
    {
        throw ProcessClientCreationException();
    }
    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    if(inet_pton(AF_INET, server_ip.c_str(), &address.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        throw ProcessClientCreationException();
    }
    int result = -1;
    int enumerator = 0;
    while (result == -1)
    {
        result = connect(socketIndex, (sockaddr*)&address, sizeof(address));
        ++enumerator;
        if (numberOfTrials != size_t(-1) && enumerator > numberOfTrials) {
            throw ProcessClientCreationException();
        }
    }
}

template<class Connector>
void ProcessInetClient<Connector>::Send(const string& request)
{
    try {
        connector.Send(request, socketIndex);
    } catch(...) {
        shutdown(socketIndex,2);
    }
}

template<class Connector>
string ProcessInetClient<Connector>::Get()
{
    string recieved = connector.Get(socketIndex);
    if (recieved == RESPONSE_ERROR_MESSAGE)
    {
        this->Close();
    }
    return recieved;
}

template<class Connector>
void ProcessInetClient<Connector>::Close()
{
    this->Send(END_SESSION_STRING);
    std::cout<<END_SESSION_STRING<<std::endl;
    shutdown(socketIndex,2);
}

template<class Connector>
ProcessInetClient<Connector>::~ProcessInetClient()
{
    close(socketIndex);
}

}
#endif //MSVC
#endif // PROCESS_INET_CLIENT
