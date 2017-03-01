#include <stdio.h>
#include "main.h"
#include "common.h"
#include "cvar.h"
#include "video_main.h"

int main(int argc, char ** argv) {

    Comm_Init();
    sleep(1);
/*    Cvar_Set("test_var", "test_value");
    Cvar_Set("test_var2", "test_value");
    Cvar_Set("test_var44", "testsssssssssss_value");
    Cvar_Set("test_var4", "test_value");
    Cvar_Set("test_var555", "test_value");

    Cvar_PrintAll();

        cvar_t *var = Cvar_FindVar("test_var44");
    printf("Found var: %s\n", var->name);
*/
    return 0;
};

