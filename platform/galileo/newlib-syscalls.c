#include <sys/stat.h>
#include <errno.h>

// Below there all needed syscall stubs so far needed by newlib.

// FIXME: I don't think this should be needed. There is something wrong with our newlib configure.
_fpos64_t __sseek64(struct _reent *ptr, void *t, _fpos64_t p, int s)
{
    return 0;
}

// FIXME: I don't think this should be needed. There is something wrong with our newlib configure.
int __swrite64(struct _reent *ptr, void *t, const char *b, int s)
{
    return 0;
}

int _close_r(struct _reent *ptr, int file)
{
    errno = ENOTSUP;
    return -1;
}

int _isatty_r(struct _reent *ptr, int file)
{
    errno = ENOTSUP;
    return 0;
}

int _read_r(struct _reent *ptr, int file, char *buf, int len)
{
    errno = ENOTSUP;
    return 0;
}

int _write_r(struct _reent *ptr, int file, const char *buf, int len)
{
    errno = ENOTSUP;
    return -1;
}

int _lseek_r(struct _reent *ptr, int file, int p, int dir)
{
    errno = ENOTSUP;
    return 0;
}

int _fstat_r(struct _reent *ptr, int file, struct stat *st)
{
    errno = ENOTSUP;
    return -1;
}

caddr_t _sbrk_r(struct _reent *ptr,int incr)
{
    errno = ENOTSUP;
    return -1;
}

void _kill_r(struct _reent *ptr)
{
    errno = ENOTSUP;
}

int _getpid_r(struct _reent *ptr)
{
    errno = ENOTSUP;
    return 1;
}

