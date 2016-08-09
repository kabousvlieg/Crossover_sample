//
// Created by kobus on 2016/08/07.
//

#include <iostream>
#include <windows.h>
#include "monitor.h"

int monitor::memory(void) {
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    return (static_cast<double>(status.ullAvailPhys) / static_cast<double>(status.ullTotalPhys)) * 100;
}
