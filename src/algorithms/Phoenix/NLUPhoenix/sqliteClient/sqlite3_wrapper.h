/*! @file sqlite3_wrapper.h
 *  @section Copyright
 *  Object Retrieval Engine
 *  @brief implementation wrapper for sqlite3 pointer
 *  @author Moiseenko Andrey <moiseenko.a@samsung.com>
 *  @version 1.0
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


#include <string>
#include <memory>
#include <iostream>
#include "sqlite/sqlite3.h"

#include "sqlite_exceptions.h"

#ifndef SQLITE3_WRAPPER_H
#define SQLITE3_WRAPPER_H

/*! @class SQLiteDataBaseWrapper
 * @brief guard for pointer to SQLiet DB
 */
class SQLiteDataBaseWrapper {
public:
    class SQLiteDataBaseGuard{
        sqlite3* m_database;
    public:
        SQLiteDataBaseGuard(sqlite3* i_db)
        : m_database(i_db){
        }
        ~SQLiteDataBaseGuard(){
            sqlite3_close(m_database);
        }
        sqlite3* database() const {
            return m_database;
        }
    };

    typedef std::shared_ptr<SQLiteDataBaseGuard> SharedSQLiteDataBaseGuard;
    SQLiteDataBaseWrapper(std::string name) {
        sqlite3* db;
        int rezalt;
        if(sqlite3_threadsafe()){
            rezalt = sqlite3_open_v2(name.c_str(), &db, SQLITE_OPEN_READWRITE|SQLITE_OPEN_CREATE, NULL);
        }
        else {
            rezalt = sqlite3_open(name.c_str(), &db);
        }
        if(rezalt) {
            throw SQLiteException(name + " isn't accessible");
        }
        m_database = SharedSQLiteDataBaseGuard(new SQLiteDataBaseGuard(db));
    }

    SharedSQLiteDataBaseGuard database()const {
        return m_database;
    }

private:
    SharedSQLiteDataBaseGuard m_database;
};

inline bool operator == (const SQLiteDataBaseWrapper& i_db1, const SQLiteDataBaseWrapper& i_db2){
    return i_db1.database()->database() == i_db2.database()->database();
}

#endif //SQLITE3_WRAPPER_H
