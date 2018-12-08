#include <stdio.h>
#include "utilities.h"

char * __strcat(const char *des,const char *source, int bufferSize){
    char *buffer = new char[bufferSize];
    int n = 0;
    while(des != nullptr && *des){
        buffer[n++] = *des++;
    }
    while(source != nullptr && *source){
        buffer[n++] = *source++;
    }

    buffer[n] = '\0';
    
    char *res = new char[n + 1];
    
    n = 0;
    while (buffer[n]){
        res[n] = buffer[n++];
    }
    delete[] buffer;

    res[n] = '\0';
    return res;
}


char * __strcpy(const char *source, int bufferSize){
    
    char *buffer = new char[bufferSize];
    
    int n = 0;
    while (buffer != nullptr && *source){
        buffer[n++] = *source++;
    }
    
    buffer[n] = '\0';
    char *res = new char[n+1];
    n = 0;
    while (buffer[n]){
        res[n] = buffer[n++];
    }
    
    delete[] buffer;
    res[n] = '\0';
    return res;
}

void __int2str(int i, char *s){
    int n = 0;
    char t[100];
    while (i > 0){
        int d = i % 10;
        t[n] = d + 48;
        n++;
        i = i / 10;
    }

    for (int i = n - 1; i>=0; i--){
        s[n - i - 1] = t[i];
    }
    s[n] = '\0';
}

void __str2int(const char *s, int *i){
    *i = 0;
    int n = strlen(s);
    if (n > 10){
        *i = 0;
        return;
    }
    for (int x = 0; x<n; x++){
        int d = s[x] - 48;
        if (d < '0' && d > '9'){
            *i = 0;
            return;
        }
        *i = (*i) * 10 + d;
    }
}

HANDLE CreateMarker(const char * curDir, const char *fileName, int timeout){
    char * t = __strcat(curDir, "/");
    char * filePath = __strcat(t, fileName);
    
    FILE * f = fopen(filePath, "r");
    
    delete[] t;
    delete[] filePath;

    if (f == nullptr || timeout < 1){
        return nullptr;
    }
    fclose(f);

    // Get string format of timeout
    // store it in time
    char time[10];
    __int2str(timeout, time);

    // Concatenate executable file name and its arguments
    char * s = __strcat("start.exe ", fileName);
    char * d = __strcat(" ", time);

    char * command = __strcat(s, d);

    STARTUPINFOA siStartupInfo;
    ZeroMemory(&siStartupInfo, sizeof(STARTUPINFOA));
    PROCESS_INFORMATION piProcInfo;
    ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));

    bool bSuccess = CreateProcessA(
        NULL,               
        command,                  // Command Line
        NULL,               // Security Attribute of Process
        NULL,               // Security Attribute of Thread
        FALSE,
        0,
        NULL,
        curDir,
        &siStartupInfo,
        &piProcInfo
    );

    delete[] s;
    delete[] d;
    delete[] command;

    if (!bSuccess){
        // If create process fail
        // Exit with a error
        return nullptr;
    }
    CloseHandle(piProcInfo.hThread);
    return piProcInfo.hProcess;
}