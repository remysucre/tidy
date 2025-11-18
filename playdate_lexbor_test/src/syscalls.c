//
//  syscalls.c
//  System call stubs for lexbor on Playdate
//

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

// Stub implementations for functions not available on Playdate
// Only provide stubs for functions that are truly missing

#if TARGET_PLAYDATE

// Process stubs
void _exit(int status) {
    (void)status;
    while(1) {} // Infinite loop as we can't actually exit
}

// Environment stubs
char* getenv(const char* name) {
    (void)name;
    return NULL;
}

// sbrk for memory allocation (required by newlib malloc)
extern char _end; // Defined by linker script
static char *heap_end = 0;

void* _sbrk(int incr) {
    char *prev_heap_end;

    if (heap_end == 0) {
        heap_end = &_end;
    }
    prev_heap_end = heap_end;
    heap_end += incr;
    return (void *)prev_heap_end;
}

// File operations - return failure
int _open(const char *name, int flags, int mode) {
    (void)name;
    (void)flags;
    (void)mode;
    errno = ENOENT;
    return -1;
}

int _close(int file) {
    (void)file;
    return -1;
}

int _read(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    (void)len;
    return 0;
}

int _write(int file, char *ptr, int len) {
    (void)file;
    (void)ptr;
    return len;
}

int _lseek(int file, int ptr, int dir) {
    (void)file;
    (void)ptr;
    (void)dir;
    return 0;
}

int _fstat(int file, struct stat *st) {
    (void)file;
    st->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file) {
    (void)file;
    return 1;
}

int _getpid(void) {
    return 1;
}

int _kill(int pid, int sig) {
    (void)pid;
    (void)sig;
    errno = EINVAL;
    return -1;
}

#endif // TARGET_PLAYDATE
