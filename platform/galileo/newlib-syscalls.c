#include <sys/stat.h>
#include <errno.h>

/* Below there all needed syscall stubs so far needed by newlib. */

int
_close_r(struct _reent *ptr, int file)
{
  ptr->_errno = ENOTSUP;
  return -1;
}
/*---------------------------------------------------------------------------*/
int
_isatty_r(struct _reent *ptr, int file)
{
  ptr->_errno = ENOTSUP;
  return 0;
}
/*---------------------------------------------------------------------------*/
int
_read_r(struct _reent *ptr, int file, char *buf, int len)
{
  ptr->_errno = ENOTSUP;
  return 0;
}
/*---------------------------------------------------------------------------*/
int
_write_r(struct _reent *ptr, int file, const char *buf, int len)
{
  ptr->_errno = ENOTSUP;
  return -1;
}
/*---------------------------------------------------------------------------*/
int
_lseek_r(struct _reent *ptr, int file, int p, int dir)
{
  ptr->_errno = ENOTSUP;
  return 0;
}
/*---------------------------------------------------------------------------*/
int
_fstat_r(struct _reent *ptr, int file, struct stat *st)
{
  ptr->_errno = ENOTSUP;
  return -1;
}
/*---------------------------------------------------------------------------*/
caddr_t
_sbrk_r(struct _reent *ptr, int incr)
{
  ptr->_errno = ENOTSUP;
  return NULL;
}
