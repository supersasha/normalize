#include <stdlib.h>
#include <string.h>

enum {
    INIT = 0,
    DOT,
    DDOT,
    WORD
};

char* normalize(const char *path)
{
    int len = strlen(path);
    char *a = (char*)malloc(len+1);
    int state = INIT;
    int d = 0;
    int sl = 0;
    
    char *r = a + len;
    for(const char *p = path + len; p >= path; p--) {
        if (d == 0) {
            switch(state) {
            case INIT:
                switch(*p) {
                case '.':
                    state = DOT;
                    *(r--) = *p;
                    sl = 0;
                    break;
                case '/':
                    if(sl == 0)
                        *(r--) = *p;
                    sl = 1;
                    break;
                case 0:
                    *(r--) = *p;
                    sl = 0;
                    break;
                default:
                    state = WORD;
                    *(r--) = *p;
                    sl = 0;
                    break;
                }
                break;
            case DOT:
                switch(*p) {
                case '.':
                    state = DDOT;
                    *(r--) = *p;
                    break;
                case '/':
                    state = INIT;
                    r++;
                    sl = 1;
                    break;
                default:
                    state = WORD;
                    *(r--) = *p;
                    break;
                }
                break;
            case DDOT:
                switch(*p) {
                case '/':
                    state = INIT;
                    r += 2;
                    d++;
                    sl = 1;
                    break;
                default:
                    state = WORD;
                    *(r--) = *p;
                    break;
                }
                break;
            case WORD:
                switch(*p) {
                case '/':
                    state = INIT;
                    *(r--) = *p;
                    sl = 1;
                    break;
                default:
                    *(r--) = *p;
                    break;
                }
                break;
            }
        } else {
            switch(state) {
            case INIT:
                switch(*p) {
                case '.':
                    state = DOT;
                    sl = 0;
                    break;
                case '/':
                    sl = 1;
                    break;
                default:
                    state = WORD;
                    sl = 0;
                    break;
                }
                break;
            case DOT:
                switch(*p) {
                case '.':
                    state = DDOT;
                    break;
                case '/':
                    state = INIT;
                    sl = 1;
                    break;
                default:
                    state = WORD;
                    break;
                }
                break;
            case DDOT:
                switch(*p) {
                case '/':
                    state = INIT;
                    d++;
                    sl = 1;
                    break;
                default:
                    state = WORD;
                    break;
                }
                break;
            case WORD:
                switch(*p) {
                case '/':
                    state = INIT;
                    if(d > 0)
                        d--;
                    sl = 1;
                    break;
                default:
                    break;
                }
                break;
            }
        }
    }
    switch(state) {
    case DOT:
        r++;
        break;
    case DDOT:
        if(d == 0) {
            r += 2;
        }
        break;
    }
    return memmove(a, r + 1, a + len - r);
}

