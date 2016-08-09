//
// Created by kobus on 2016/08/08.
//

#include <windows.h>
#include "mailer.h"

void mailer::sendAlert(struct clientInfo ci) {
    //TODO Sensitive information removed
    char const *command = "curl smtp://smtp.gmail.com:587 -v "\
        "--mail-from \"kobusnexus@gmail.com\" "\
        "--mail-rcpt \"kobus@embeddedfool.net\" "\
        "--ssl -u kobusnexus@gmail.com:Raa1h0mn0u "\
        "-k --anyauth";
    WinExec(command, SW_HIDE);
}
