#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

extern char * normalize(const char *);

char * mallocfree(unsigned long cycles, int inplen)
{
    char *a;
    for(int i = 0; i < cycles; i++) {
        a = (char*)malloc(inplen);
        free(a);
    }
    return a;
}

int test()
{
    // Additional features (seem reasonable):
    // - keep trailing slashes
    // - remove duplicate slashes
    // Not sure how to deal with missing slashes at the beginning of a path
    const char * tests[] = {
        "./bar", "/bar",
        "../bar", "/bar",
        "../../bar", "/bar",
        "foo", "foo",
        "/foo/", "/foo/",
        "//foo", "/foo",
        "/foo/bar", "/foo/bar",
        "/foo/bar/../baz", "/foo/baz",
        "/foo/bar/./baz", "/foo/bar/baz",
        "/foo/../../baz", "/baz",
        "/foo//../bar/.././/baz//", "/baz/",
        "/foo/bar/../baz//pi/../ro/./sigma/", "/foo/baz/ro/sigma/",
    };

    int cnt = sizeof(tests) / sizeof(const char*);
    int err = 0;

    for(const char** t = tests; t < tests + cnt; t += 2) {
        char * res = normalize(*t);
        if(strcmp(res, *(t+1))) {
            printf("normalize(\"%s\") should be \"%s\" but is \"%s\"\n",
                    *t, *(t+1), res);
            err++;
        }
        free(res);
    }
    return err;
}

double dt(clock_t start, clock_t end)
{
    return (double)(end-start) / CLOCKS_PER_SEC;
}

void perftest() {
    const char * inp = "/foo/bar/../baz//pi/../ro/./sigma/";
    int inplen = strlen(inp) + 1;
    unsigned long cycles = 1000000000;

    clock_t t0 = clock();
    for(int i = 0; i < cycles; i++) {
        char * res = normalize(inp);
        free(res);
    }
    time_t t1 = clock();
    char * volatile a = mallocfree(cycles, inplen);
    time_t t2 = clock();

    printf("Perftest time: %.3fs, %.3fGbytes/sec\n",
            dt(t0, t1),
            (double)cycles * inplen / dt(t0, t1) / 1024.0 / 1024.0 / 1024.0);
    printf("malloc/free time: %.3fs, %.1f%%\n", dt(t1, t2), dt(t1, t2) / dt(t0, t1) * 100.0);
}

int main(int argc, const char ** argv)
{
    test();
    perftest();
    return 0;
}

