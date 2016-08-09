//
// Created by kobus on 2016/08/07.
//

#pragma once

#include <sqlite3.h>
#include "clientInfo.h"

class database {
public:
    database();
    void add(struct clientInfo ci);
    ~database();
private:
    sqlite3 *db;
};


