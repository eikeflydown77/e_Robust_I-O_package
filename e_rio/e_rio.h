#ifndef E_RIO_E_RIO_H
#define E_RIO_E_RIO_H

#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>


#define RIO_BUFSIZE 8192

/*宏：包装后的读接口所需要定义的宏*/
#define e_rio_readinit(fd) rio_readinitb(fd, &r_buffer);
#define e_rio_seekbuffd() rio_seekbuffd(&r_buffer);
#define e_rio_seekbufcnt() rio_seekbufcnt(&r_buffer);
#define e_rio_getbufptr() rio_getbufptr(&r_buffer);

typedef struct {
    int rio_fd;                /* Descriptor for this internal buf 指示符*/
    int rio_cnt;               /* Unread bytes in internal buf 缓冲区中未读数数量*/
    char *rio_bufptr;          /* Next unread byte in internal buf 缓冲区指针*/
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer 缓冲区数据区*/
} rio_t;
/* $end rio_t */

static rio_t r_buffer;

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
void rio_readinitb(int fd, rio_t *rp);
int rio_seekbuffd(const rio_t *rp);
int rio_seekbufcnt(const rio_t *rp);
char *rio_getbufptr(rio_t *rp);
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);
ssize_t	rio_vreadnb(int fd, void *usrbuf, size_t n);
ssize_t rio_vreadlineb(int fd, void *usrbuf, size_t n);

#endif //E_RIO_E_RIO_H
