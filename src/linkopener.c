#include <stdlib.h>
#include <stdio.h>
    
#if defined(_WIN32) || defined(_WIN64)
    static char* cmd = "start";
#elif defined(__APPLE__) || defined(__MACH__)
    static char* cmd = "open";
#else
    static char* cmd = "xdg-open";
#endif

void OpenLink(const char *url)
{

    char buffer[512];   
    sprintf(buffer, "%s %s", cmd, url); 
    buffer[511] = '\0';
    system(buffer);

}