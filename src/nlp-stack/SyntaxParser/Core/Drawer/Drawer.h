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

#ifndef TREE_DRAWER
#define TREE_DRAWER

#include "SyntaxTree.h"

namespace SyntaxParser
{

class Drawer
{
public:
    Drawer(const string& outputFile = SYNTAX_OUTPUT_TREE_PATH);
    void Draw(const SyntaxTree& real, const SyntaxTree& predicted, bool openPDF=true);
    void Draw(const SyntaxTree& tree, bool openPDF=true);
private:
    void draw(const wstring& dotGraph, bool openPDF);
    wstring getDualDotText(const SyntaxTree& real, const SyntaxTree& predicted);

    string outputFile;
};

}

#endif // TREE_DRAWER
