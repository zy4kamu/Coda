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

#ifndef PROCESS_INET_SERVER
#define PROCESS_INET_SERVER

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

using std::string;

template <class Connector>
class ProcessInetServer
{
public:
    ProcessInetServer(int port);
    ~ProcessInetServer();
    void Start();
    virtual string GetResponce(const string& request) const = 0;
private:
    Connector connector;

    int socketIndex;
    sockaddr_in address;
};

template <class Connector>
ProcessInetServer<Connector>::ProcessInetServer(int port)
{
    // Creating socket
    socketIndex = socket(AF_INET, SOCK_STREAM, 0);
    if (socketIndex < 0)
    {
        throw ProcessServerCreationException();
    }
    // Bind socket to file
    memset(&address, 0, sizeof(struct sockaddr_in)); // start with a clean addres structure
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(socketIndex, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        throw ProcessServerCreationException();
    }
    // Listen
    if (listen(socketIndex, 5) != 0)
    {
        throw ProcessServerCreationException();
    }
}

template <class Connector>
void ProcessInetServer<Connector>::Start()
{
    int connectionIndex;
    socklen_t addressLength = sizeof(address);
    while(true)
    {
        connectionIndex = accept(socketIndex
                                 , (struct sockaddr *) &address
                                 , &addressLength
                                 );
        if (connectionIndex > -1) {
            string responce;
            string request;
            while (true)
            {
                try {
                    request = connector.Get(connectionIndex);
                    if (request == END_SESSION_STRING)
                    {
                        close(connectionIndex);
                        break;
                    }
                } catch (...) {
                    close(connectionIndex);
                    break;
                }
                responce = GetResponce(request);
                try {
                    connector.Send(responce, connectionIndex);
                } catch (...) {
                    close(connectionIndex);
                    break;
                }
            }
        }
    }
}

template <class Connector>
ProcessInetServer<Connector>::~ProcessInetServer()
{
    shutdown(socketIndex,2);
}

}

#endif //MSVC
#endif // PROCESS_INET_SERVER
