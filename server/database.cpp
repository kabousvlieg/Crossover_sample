//
// Created by kobus on 2016/08/07.
//
#include <sqlite3.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "database.h"
#include "exceptions.h"

sqlite3 database::*db = nullptr;
const char* db_filename = "crossover.db";

database::database()
{
    char *zErrMsg = 0;

    std::ifstream dbfile(db_filename);
    auto db_file_exists = dbfile.good();

    auto rc = sqlite3_open(db_filename, &db);
    if (rc){
        throw new Except("Can't open database");
    }

    if(!db_file_exists)
    {
        /* Create SQL statement */
        char sql[] = "CREATE TABLE COMPANY("  \
         "PKEY           TEXT PRIMARY KEY       NOT NULL," \
         "ID             TEXT                   NOT NULL," \
         "EMAIL          TEXT                   NOT NULL," \
         "MEM            INT                    NOT NULL," \
         "CPU            INT                    NOT NULL," \
         "PROCESSES      INT                    NOT NULL);";
        //Todo add date time
        rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
        if( rc != SQLITE_OK ){
            sqlite3_free(zErrMsg);
            throw new Except("SQL error creating table");
        }
    }
}

database::~database()
{
    sqlite3_close(db);
}

void database::add(struct clientInfo ci)
{
    int rc;
    char *zErrMsg = 0;
    static int index = 0;

    std::string sql = "INSERT INTO COMPANY (PKEY,ID,EMAIL,MEM,CPU,PROCESSES) "  \
        "VALUES (" \
        "'" + std::to_string(index++) + "', "\
        "'" + ci.id + "', " \
        "'" + ci.email + "', " \
        + std::to_string(ci.mem) + ", "\
        + std::to_string(ci.cpu) + ", "\
        + std::to_string(ci.processes) + ");";
    rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        sqlite3_free(zErrMsg);
        throw new Except("SQL error adding entry");
    }
    return;
}
