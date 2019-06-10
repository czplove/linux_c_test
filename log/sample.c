#include <stdio.h>

void Log(int  log_level, char* format, ...);

int main(int argc, char **argv)
{
    int i;
    char* file="log.c";
    int line = 10;

    for (i=0; i<5; i++) {
        printf("HelloWorld!\n");
    }
    Log(13, "Test log start\n");
    Log(13, "Test log : file %s line %d\n", file, line);
    
    return 0;
}

void Log(int  log_level, char* format, ...)
{
	printf("Log %s", format);
}

