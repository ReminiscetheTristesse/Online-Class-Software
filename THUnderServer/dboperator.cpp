/*
database operations
 */

#include "dboperator.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>

dboperator::dboperator(const char* path) {
    int result = sqlite3_open(path, &this->db);
    // CREATE TABLE ClientInfo (username TEXT [NOT NULL], pswd TEXT[NOT NULL], type INTEGER [NOT NULL])
    if (result != SQLITE_OK) {
        printf("Failed to initialize db\n");
    }
}

dboperator::~dboperator() {
    sqlite3_close(this->db);
}

dboperator::dboperator(const dboperator& x) {
    this->db = x.db;
}

dboperator& dboperator::operator = (const dboperator& x) {
    this->db = x.db;
    return *this;
}

unsigned dboperator::add_client(const char* username,
                               const char* pswd,
                               const unsigned& type) {
    // exclude existed username
    if (this->exist(username)) return 1;
    
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));
    char* str_type = new char[20];
    itoa(type, str_type, 10);

    // INSERT INTO ClientInfo VALUES (<username>, <pswd>, <type>)
    sql = strcat(sql, "INSERT INTO ClientInfo VALUES (");
    sql = strcat(strcat(strcat(sql, "'"), username), "',");
    sql = strcat(strcat(strcat(sql, "'"), pswd), "',");
    sql = strcat(strcat(strcat(sql, "'"), str_type), "')");

    int result = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

    delete[] sql;
    delete[] str_type;
    if (result != SQLITE_OK) {
        return 1;
    } else {
        return 0;
    }
}

unsigned dboperator::del_client(const char* username) {

    if (!this->exist(username)) return 1;// doesn't exist
    
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));

    // DELETE FROM ClientInfo WHERE username = <username>
    sql = strcat(sql, "DELETE FROM ClientInfo WHERE username = ");
    sql = strcat(strcat(strcat(sql, "'"), username), "'");

    int result = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

    delete[] sql;
    if (result != SQLITE_OK) {
        return 1;
    } else {
        return 0;
    }
}

unsigned dboperator::change_username(const char* username,
                                    const char* new_username) {
    // doesn't exist
    if (!this->exist(username)) return 1;
    // exclude existed username
    if (this->exist(new_username)) return 2;
    
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));

    // UPDATE ClientInfo SET username = <new_username> where username = <username>
    sql = strcat(sql, "UPDATE ClientInfo SET username = ");
    sql = strcat(strcat(strcat(sql, "'"), new_username), "'");
    const char* mid = " WHERE username = ";
    sql = strcat(sql, mid);
    sql = strcat(strcat(strcat(sql, "'"), username), "'");

    int result = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

    delete[] sql;
    if (result != SQLITE_OK) {
        return 1;
    } else {
        return 0;
    }
}

unsigned dboperator::change_pswd(const char* username,
                                const char* new_pswd) {

    // doesn't exist
    if (!this->exist(username)) return 1;
    
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));

    // UPDATE ClientInfo SET pswd = <new_pswd> where username = <username>
    sql = strcat(sql, "UPDATE ClientInfo SET pswd = ");
    sql = strcat(strcat(strcat(sql, "'"), new_pswd), "'");
    const char* mid = " WHERE username = ";
    sql = strcat(sql, mid);
    sql = strcat(strcat(strcat(sql, "'"), username), "'");

    int result = sqlite3_exec(this->db, sql, NULL, NULL, NULL);

    delete[] sql;
    if (result != SQLITE_OK) {
        return 1;
    } else {
        return 0;
    }
}

unsigned dboperator::checktype(const char* username,
                          const char* pswd) const {
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));

    // SELECT type FROM ClientInfo WHERE username = <username> and pswd = <pswd>
    sql = strcat(sql, "SELECT type FROM ClientInfo WHERE username = ");
    sql = strcat(strcat(strcat(sql, "'"), username), "'");
    sql = strcat(sql, " AND pswd = ");
    sql = strcat(strcat(strcat(sql, "'"), pswd), "'");

    char** azresult;
    int rows;
    int result = sqlite3_get_table(this->db, sql, &azresult,
                                   &rows, NULL, NULL);

    delete[] sql;
    if (result != SQLITE_OK) {
        printf("Failed to give check");
    }
    if (rows >= 1) {
        return azresult[1][0] - '0';
    } else return 0;
    sqlite3_free_table(azresult);
}

bool dboperator::exist(const char* username) const {
    char* sql = new char[200];
    memset(sql, 0, sizeof(sql));

    // SELECT type FROM ClientInfo WHERE username = <username>
    sql = strcat(sql, "SELECT type FROM ClientInfo WHERE username = ");
    sql = strcat(strcat(strcat(sql, "'"), username), "'");

    int rows;
    char** azresult;
    int result = sqlite3_get_table(this->db, sql,
                                   &azresult, &rows, NULL, NULL);

    sqlite3_free_table(azresult);
    if (result != SQLITE_OK) {
        printf("Failed to give existence");
    }
    
    if (rows) return true;
    else return false;
}
