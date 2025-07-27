#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Headers/pre_assembler.h"
#include "Headers/first_pass.h"

int main(int argc, char *argv[])
{
    char *file_name = argv[1];
    pre_assembler(file_name);
    first_pass(file_name);
    return 0;
}
