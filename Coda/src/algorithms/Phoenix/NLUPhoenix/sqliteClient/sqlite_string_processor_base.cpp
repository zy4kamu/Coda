#include "sqlite_string_processor_base.h"

const std::string SQLiteStringProcessorBase::CREATE_TABLE = "CREATE TABLE IF NOT EXISTS";
const std::string SQLiteStringProcessorBase::CREATE = "CREATE";
const std::string SQLiteStringProcessorBase::ALERT_TABLE = "ALERT TABLE";
const std::string SQLiteStringProcessorBase::INSERT = "INSERT";
const std::string SQLiteStringProcessorBase::SELECT ="SELECT";
const std::string SQLiteStringProcessorBase::UPDATE = "UPDATE";
const std::string SQLiteStringProcessorBase::FROM = "FROM";
const std::string SQLiteStringProcessorBase::WHERE = "WHERE";
const std::string SQLiteStringProcessorBase::JOIN = "JOIN";
const std::string SQLiteStringProcessorBase::USING = "USING";
const std::string SQLiteStringProcessorBase::INDEX = "INDEX";

const std::string SQLiteStringProcessorBase::GROUP_BY = "GROUP BY";
const std::string SQLiteStringProcessorBase::HAVING = "HAVING";
const std::string SQLiteStringProcessorBase::ORDER_BY = "ORDER BY";
const std::string SQLiteStringProcessorBase::LIMIT = "LIMIT";
const std::string SQLiteStringProcessorBase::OFFSET = "OFFSET";
const std::string SQLiteStringProcessorBase::SET = "SET";
const std::string SQLiteStringProcessorBase::DELETE_ = "DELETE";
const std::string SQLiteStringProcessorBase::DROP_TABLE ="DROP TABLE";
const std::string SQLiteStringProcessorBase::DROP="DROP";
const std::string SQLiteStringProcessorBase::DEFAULT = "DEFAULT";
const std::string SQLiteStringProcessorBase::VALUES = "VALUES";
const std::string SQLiteStringProcessorBase::AUTOINCREMENT = "AUTOINCREMENT";
const std::string SQLiteStringProcessorBase::UNIQUE = "UNIQUE";
const std::string SQLiteStringProcessorBase::PRIMARY_KEY = "PRIMARY KEY";
const std::string SQLiteStringProcessorBase::FOREIGN_KEY = "FOREIGN KEY";
const std::string SQLiteStringProcessorBase::REFERENCES = "REFERENCES";

const std::string SQLiteStringProcessorBase::INTO = "INTO";
const std::string SQLiteStringProcessorBase::OR = "OR";
const std::string SQLiteStringProcessorBase::ROLLBACK = "ROLLBACK";
const std::string SQLiteStringProcessorBase::ABORT = "ABORT";
const std::string SQLiteStringProcessorBase::REPLACE = "REPLACE";
const std::string SQLiteStringProcessorBase::FAIL = "FAIL";
const std::string SQLiteStringProcessorBase::_IGNORE = "IGNORE";
const std::string SQLiteStringProcessorBase::IF_NOT_EXISTS = "IF NOT EXISTS";

const std::string SQLiteStringProcessorBase::BLOB = "BLOB";
const std::string SQLiteStringProcessorBase::TEXT = "TEXT";
const std::string SQLiteStringProcessorBase::INTEGER = "INTEGER";
const std::string SQLiteStringProcessorBase::REAL = "REAL";

const std::string SQLiteStringProcessorBase::SPACE = " ";
const std::string SQLiteStringProcessorBase::COMMA = ", ";
const std::string SQLiteStringProcessorBase::SEMICOLON = "; ";
const std::string SQLiteStringProcessorBase::BRECET_OPEN = "(";

const std::string SQLiteStringProcessorBase::CROSS = "CROSS";
const std::string SQLiteStringProcessorBase::INNER = "INNER";
const std::string SQLiteStringProcessorBase::ON = "ON";
const std::string SQLiteStringProcessorBase::NATURAL = "NATURAL";
const std::string SQLiteStringProcessorBase::LEFT = "LEFT";
const std::string SQLiteStringProcessorBase::RIGHT = "RIGHT";

const std::string
SQLPredicateAscedColumnQuery::OperationCollection[SQLPredicateAscedColumnQuery::SQLPredicateOperator::LIMIT_OPER] = {
    "=",
    "<",
    ">",
    "<=",
    ">=",
    "!=",
    "IS",
    "IS NOT",
    "AND",
    "||",
    "IN",
    "NOT IN",
    "BETWEEN"
};

