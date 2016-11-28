#include <cassert>

#include "sqlite_helper.h"
#include "sqlite_string_processor_base.h"
#include "blob.h"

std::shared_ptr<ISQLColumnDataQuery> deMarshalColumnDataQuery(int i_columnIterator, const std::string& i_name,
        const std::string& i_type, sqlite3_stmt* i_statment ){
    std::shared_ptr<ISQLColumnDataQuery> rezult;
    if(i_type == SQLiteStringProcessorBase::BLOB){
        size_t size = sqlite3_column_bytes(i_statment, i_columnIterator);
        const char* returnedBlob = static_cast<const char*>(
                sqlite3_column_blob(i_statment, i_columnIterator));
        std::pair<char*, size_t> decodedData = SQLiteStringProcessorBase::HexString2Blob(returnedBlob, size);
        rezult = std::shared_ptr<ISQLColumnDataQuery>(new SQLColumnDataQuery<Blob>(i_name,
                Blob(decodedData.first, decodedData.second)));
        delete[] decodedData.first;
    }
    else if(i_type == SQLiteStringProcessorBase::TEXT){
        const unsigned char* text = sqlite3_column_text(i_statment, i_columnIterator);
        size_t size = sqlite3_column_bytes(i_statment, i_columnIterator);
        rezult = std::shared_ptr<ISQLColumnDataQuery>(new SQLColumnDataQuery<std::string>(i_name,
                std::string(text, text + size)));
    }
    else if(i_type == SQLiteStringProcessorBase::INTEGER){
        rezult = std::shared_ptr<ISQLColumnDataQuery>(
                new SQLColumnDataQuery<int64_t>(i_name, sqlite3_column_int(i_statment, i_columnIterator)));
    }
    else if(i_type == SQLiteStringProcessorBase::REAL){
        rezult = std::shared_ptr<ISQLColumnDataQuery>(new SQLColumnDataQuery<double>(i_name, sqlite3_column_double(i_statment, i_columnIterator)));
    }
    else {
        assert(0); //"unknown sql type"
        return std::shared_ptr<ISQLColumnDataQuery>(NULL);
    }
    return rezult;
}

bool Iterator::operator == (const Iterator& i_other) const {
    bool cmp = false;
    if(m_pzTail == NULL || i_other.m_pzTail == NULL) {
        cmp = m_pzTail == i_other.m_pzTail ;
    }
    else {
        cmp = (strcmp(m_pzTail, i_other.m_pzTail) == 0);
    }
    if(m_rezData != std::shared_ptr<reference_type>() && i_other.m_rezData != std::shared_ptr<reference_type>()) {
        cmp = cmp && m_rezData == i_other.m_rezData;
    }
    else {
        cmp = cmp && (m_rezData == std::shared_ptr<reference_type>()) == (i_other.m_rezData == std::shared_ptr<reference_type>());
    }
    return cmp;
}

Iterator& Iterator::operator=(const Iterator& i_other) {
    if (this != &i_other) {
        flushQuery();
        m_database = (i_other.m_database);

        if (i_other.m_oldPzTail != NULL) {
            if (*(i_other.m_oldPzTail) != '\0') {
                int queriBuffereLength = strlen(i_other.m_oldPzTail) + 1;
                m_strQuery = new char[queriBuffereLength];
                strcpy(m_strQuery, i_other.m_oldPzTail);
                m_pzTail = m_strQuery;
                initial();
            }
        }
    }
    return *this;
}

void Iterator::next() {
    m_lastRet = sqlite3_step(m_statment);

    if (m_lastRet == SQLITE_DONE) {
        if (m_pzTail == NULL || *m_pzTail == '\0') {
            flushQuery();
            return;
        }
        sqlite3_finalize(m_statment);
        initial();
        return ;
    }

    if (m_lastRet == SQLITE_ROW) {
        readData();
        return;
    }

    {
        std::string errMessage(sqlite3_errmsg(m_database.database()->database()));
        size_t lastQuerySize = m_pzTail - m_oldPzTail + 1;
        char* query = new char[lastQuerySize];
        memcpy(query, m_oldPzTail, lastQuerySize - 1);
        query[lastQuerySize - 1] = 0;
        fprintf(stderr, "SQLite ERROR - %s;\nfailed query - %s;\nset of query %s."
                , errMessage.c_str()
                , query, m_strQuery
                );
        delete[] query;
        m_lastRet = SQLITE_OK;
        sqlite3_finalize(m_statment);
        initial();
        //flushQuery();
        throw -1;
        return;
    }
}

void Iterator::initial() {
    if(m_lastRet == SQLITE_DONE || m_lastRet == 0) {
        m_rezData.reset();
        if (m_pzTail == NULL || *m_pzTail == '\0') {
            flushQuery();
            return;
        }
        m_oldPzTail = m_pzTail;
        m_lastRet = sqlite3_prepare_v2(m_database.database()->database(),
        m_pzTail,
        -1,
        &m_statment,
        const_cast<const char**>(&m_pzTail));
    }

    if (m_lastRet != SQLITE_OK && m_lastRet != SQLITE_ROW) {
        std::string errMessage(sqlite3_errmsg(m_database.database()->database()));
        size_t lastQuerySize = m_pzTail - m_oldPzTail + 1;
        char* query = new char[lastQuerySize];
        memcpy(query, m_oldPzTail, lastQuerySize - 1);
        query[lastQuerySize - 1] = 0;
        fprintf(stderr, "SQLite ERROR - %s;\nfailed query - %s;\nset of query %s."
                , errMessage.c_str()
                , query, m_strQuery
                );
        delete[] query;
//        flushQuery();
        throw -1;
//        return;
    }

    next();
}

void Iterator::flushQuery(){
    m_rezData.reset();
    if(m_statment){
        sqlite3_finalize(m_statment);
        m_statment = NULL;
    }
    m_pzTail = NULL;
    if(m_strQuery != NULL){
        delete[] m_strQuery;
    }
    m_strQuery = NULL;
    m_oldPzTail = NULL;
    m_lastRet = 0;
}

void Iterator::readData(){
    m_rezData.reset();
    m_rezData = std::shared_ptr<reference_type>(new reference_type());
    int columnCount = sqlite3_column_count(m_statment);
    for(int i = 0; i < columnCount; ++i){
        m_rezData->push_back(deMarshalColumnDataQuery(i, m_statment));
    }
}
