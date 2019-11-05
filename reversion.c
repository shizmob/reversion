#include <stdio.h>
#include <string.h>
#include <dlfcn.h>
#include <rd_route.h>


static struct reversion {
    const char *original;
    const char *replacement;
} reversions[] = {
    { "libcrypto.dylib", "libcrypto.0.9.8.dylib" },
    { "libssl.dylib",    "libssl.0.9.8.dylib"    },
    { 0 }
};

void *(*orig_dlopen)(const char *path, int mode);

void *my_dlopen(const char* path, int mode) {
     if (path) {
         for (const struct reversion *r = reversions; r->original; r++) {
             if (!strcmp(path, r->original)) {
                 path = r->replacement;
                 break;
             }
         }
     }
     return orig_dlopen(path, mode);
}

__attribute__((constructor))
static void reverse(void)
{
    rd_route(dlopen, my_dlopen, (void **)&orig_dlopen);
}
