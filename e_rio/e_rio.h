#ifndef E_RIO_E_RIO_H
#define E_RIO_E_RIO_H

#include <unistd.h>
#include <string.h>
#include <sys/mman.h>
#include <errno.h>

/* $begin rio_t */
#define RIO_BUFSIZE 8192
typedef struct {
    int rio_fd;                /* Descriptor for this internal buf */
    int rio_cnt;               /* Unread bytes in internal buf */
    char *rio_bufptr;          /* Next unread byte in internal buf */
    char rio_buf[RIO_BUFSIZE]; /* Internal buffer */
} rio_t;
/* $end rio_t */

ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
ssize_t	e_rio_readnb(int fd, void *usrbuf, size_t n);
ssize_t	e_rio_readlineb(int fd, void *usrbuf, size_t maxlen);

#endif //E_RIO_E_RIO_H
