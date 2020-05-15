#include <assert.h>
#include <errno.h>
#include <reent.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/types.h>
#include <sys/unistd.h>

#define SYS_exit "0x0F"
#define UNUSED( VAR ) ( (void) VAR )

/**
 * \brief Dummy environment variable list.
 */
char * __env[1] = { '\0' };
/**
 * \brief Pointer to list of environmental variables.
 *
 * In our case can just be emoty.
 */
char ** environ = __env;

/**
 * \brief Exit a program without cleaning up files
 *
 * If your system doesn't provide this, it is best to avoid linking with subroutines that
 * require it (exit, system).
 *
 * \param code The exit code, 0 for success, other value for error.
 */
void __attribute__((weak))
_exit(int code)
{
    UNUSED(code);
    __asm__ __volatile__(".set noreorder       \n\t \
                          syscall " SYS_exit " \n\t \
                          nop                  \n\t \
                          .set reorder");
    while (1);
}

/*
 * Group of functions below implements reentrant syscalls required by newlib.
 * ccproc defines REENTRANT_SYSCALLS_PROVIDED
 */

int __attribute__((weak))
_close_r(
    struct _reent * reent,
    int fd
)
{
    assert(NULL != reent);
    UNUSED(fd);
    reent->_errno = ENOTSUP;
    return -1;
}

int __attribute__((weak))
_execve_r(
    struct _reent * reent,
    const char * name,
    char * const * argv,
    char * const * env
)
{
    assert(NULL != reent);
    UNUSED(name);
    UNUSED(argv);
    UNUSED(env);
    reent->_errno = ENOMEM;
    return -1;
}

int __attribute__((weak))
_fcntl_r(
    struct _reent * reent,
    int fd,
    int cmd,
    int arg
)
{
    assert(NULL != reent);
    UNUSED(fd);
    UNUSED(cmd);
    UNUSED(arg);
    reent->_errno = ENOTSUP;
    return -1;
}

int __attribute__((weak))
_fork_r(
    struct _reent * reent
)
{
    assert(NULL != reent);
    reent->_errno = EAGAIN;
    return -1;
}

int __attribute__((weak))
_fstat_r(
    struct _reent * reent,
    int fd,
    struct stat * st
)
{
    assert(NULL != reent);
    UNUSED(fd);
    /* do not change errno */
    st->st_mode = S_IFCHR;
    return 0;
}

int __attribute__((weak))
_getpid_r(
    struct _reent * reent
)
{
    assert(NULL != reent);
    /* do not change errno */
    return 1;
}

int __attribute__((weak))
_isatty_r(
    struct _reent * reent,
    int fd
)
{
    assert(NULL != reent);
    /* do not change errno */
    int const result = (fd == STDOUT_FILENO || fd == STDERR_FILENO) ? 1 : 0;
    return result;
}

int __attribute__((weak))
_kill_r(
    struct _reent * reent,
    int pid,
    int sig
)
{
    assert(NULL != reent);
    UNUSED(pid);
    UNUSED(sig);
    reent->_errno = EPERM;
    return -1;
}

int __attribute__((weak))
_link_r(
    struct _reent * reent,
    const char * old,
    const char * new
)
{
    assert(NULL != reent);
    UNUSED(old);
    UNUSED(new);
    reent->_errno = EMLINK;
    return -1;
}

off_t __attribute__((weak))
_lseek_r(
    struct _reent * reent,
    int fd,
    off_t offset,
    int whence
)
{
    assert(NULL != reent);
    UNUSED(fd);
    UNUSED(offset);
    UNUSED(whence);
    reent->_errno = EBADF;
    return -1;
}

int __attribute__((weak))
_mkdir_r(
    struct _reent * reent,
    const char * name,
    int mode
)
{
    assert(NULL != reent);
    UNUSED(name);
    UNUSED(mode);
    reent->_errno = EPERM;
    return -1;
}

int __attribute__((weak))
_open_r(
    struct _reent * reent,
    const char * name,
    int flags,
    int mode
)
{
    assert(NULL != reent);
    UNUSED(name);
    UNUSED(flags);
    UNUSED(mode);
    reent->_errno = ENXIO;
    return -1;
}

ssize_t __attribute__((weak))
_read_r(
    struct _reent * reent,
    int fd,
    void * ptr,
    size_t len
)
{
    assert(NULL != reent);
    UNUSED(ptr);
    UNUSED(len);
    ssize_t result = 0;
    if (STDIN_FILENO != fd) {
        reent->_errno = EBADF;
        result = -1;
    }
    /* do not change errno */
    return result; /* nothing was read */
}

int __attribute__((weak))
_rename_r(
    struct _reent * reent,
    const char * old,
    const char * new
)
{
    assert(NULL != reent);
    UNUSED(old);
    UNUSED(new);
    reent->_errno = EMLINK;
    return -1;
}

void * __attribute__((weak))
_sbrk_r
(
  struct _reent * reent,
  ptrdiff_t incr
)
{
    assert(NULL != reent);
    UNUSED(incr);
    reent->_errno = ENOMEM;
    return ((void *)-1);
}

int __attribute__((weak))
_stat_r(
    struct _reent * reent,
    const char * name,
    struct stat * st
)
{
    assert(NULL != reent);
    UNUSED(name);
    /* do not change errno */
    st->st_mode = S_IFCHR;
    return 0;
}

clock_t __attribute__((weak))
_times_r(
    struct _reent * reent,
    struct tms * buf
)
{
    assert(NULL != reent);
    UNUSED(buf);
    reent->_errno = ENOTSUP;
    return -1;
}

int __attribute__((weak))
_unlink_r(
    struct _reent * reent,
    const char * name
)
{
    assert(NULL != reent);
    UNUSED(name);
    reent->_errno = ENOENT;
    return -1;
}

int __attribute__((weak))
_wait_r(
    struct _reent * reent,
    int * status
)
{
    assert(NULL != reent);
    UNUSED(status);
    reent->_errno = ECHILD;
    return -1;
}

ssize_t __attribute__((weak))
_write_r(
    struct _reent * reent,
    int fd,
    const void * ptr,
    size_t len
)
{
    assert(NULL != reent);
    UNUSED(ptr);
    UNUSED(len);
    ssize_t result = 0;
    if (
      (STDIN_FILENO != fd)
      && (STDERR_FILENO != fd)
    ) {
        reent->_errno = EBADF;
        result = -1;
    }
    reent->_errno = EINVAL;
    result = -1;
    return result;
}

int __attribute__((weak))
_gettimeofday_r(
    struct _reent * reent,
    struct timeval *__tp,
    void * __tzp
)
{
    assert(NULL != reent);
    UNUSED(__tp);
    UNUSED(__tzp);
    reent->_errno = EINVAL;
    return -1;
}

