#include <windows.h>
char * __strcat(const char *des, const char *source, int bufferSize = 1024);
char * __strcpy(const char *source, int bufferSize = 1024);
HANDLE CreateMarker(const char *, const char *, int);
void __int2str(int, char *);
void __str2int(const char *, int *);

typedef struct __DATA{
    char data[65353 * 255];
    int cursor;
    int len;

    __DATA(){
        ZeroMemory(data, sizeof(data));
    }

    bool add(const char *s){
        if (len + strlen(s) > 65353 * 255){
            return false;
        }
        for (int i = 0; i< strlen(s); i++){
            if (s[i] == '\n' || s[i] == '\t' || s[i] == 13)
                data[len] = ' ';
            else
                data[len] = s[i];
            len++;
        }
        return true;
    }

    void open(){
        cursor = 0;
    }
    
    void scanf(char * s){
        while (cursor < len && data[cursor] == ' ')
            cursor ++;
      
        int oldcursor = cursor;

        int n = 0;
        while (cursor < len && data[cursor] != ' '){
            s[n++] = data[cursor];
            cursor ++;
        }
        s[n] = '\0';
    }// End Scanf

// END STRUCT
} __DATA, *P__DATA;