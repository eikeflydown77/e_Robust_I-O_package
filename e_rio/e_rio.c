#include "e_rio.h"
/**
 *健壮的无缓冲读函数：从目标文件中读入数据到用户内存
 * @param fd    目标文件
 * @param usrbuf    用户内存
 * @param n     数据长度
 * @return      若成功返回传送的字节数，否则则是-1
 */
ssize_t rio_readn(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;   /*数据剩余的字节*/
    ssize_t nread;      /*每次读入的字节*/
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = read(fd, bufp, nleft)) < 0) {
            if (errno == EINTR)     /*接收到中断信号*/
                nread = 0;          /*重启函数*/
            else
                return -1;          /*读出错*/
        } else if (nread == 0)      /*EOF*/
            break;
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);     /*Return >= 0*/
}

/**
 *健壮的无缓冲写函数：从用户内存写入数据到目标文件
 * @param fd    目标文件
 * @param usrbuf    用户内存
 * @param n     数据长度
 * @return      若成功返回传送的字节数，否则则是-1
 */
ssize_t rio_writen(int fd, void *usrbuf, size_t n) {
    size_t nleft = n;
    ssize_t nwritten;
    char *bufp = usrbuf;
    while (nleft > 0) {
        if ((nwritten = write(fd, bufp, nleft)) < 0) {
            if (errno == EINTR)     /*接收到中断信号*/
                nwritten = 0;          /*重启函数*/
            else
                return -1;          /*写出错*/
        }
        nleft -= nwritten;
        bufp += nwritten;
    }
    return n;
}

/**
 * 初始化内部缓存空间，用与rio中带缓存的输入输出函数
 * @param rp
 * @param fd
 */
void rio_readinitb(rio_t *rp, int fd) {
    rp->rio_fd = fd;
    rp->rio_cnt = 0;
    rp->rio_bufptr = rp->rio_buf;
}

/**
 *
 * @param rp
 * @param usrbuf
 * @param n
 * @return
 */
static ssize_t rio_read(rio_t *rp, char *usrbuf, size_t n) {
    int cnt;

    /*当缓冲区为空时从文件中读去数据填满缓冲区*/
    while (rp->rio_cnt <= 0) {
        rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
        if (rp->rio_cnt < 0) {
            if (errno != EINTR) return -1;  /*如果不是被中断的，就是发生了错误*/
        } else if (rp->rio_cnt == 0) {  /*文件没有数据*/
            return  0;
        } else {
            rp->rio_bufptr = rp->rio_buf;   /*初始化指针，指向缓冲区首*/
        }
    }

    /*从缓冲区中复制数据到内存位置*/
    cnt = n;
    /*该设计中会按照缓冲区的最大数据量去加载数据，故有可能会出现 rp->rio_cnt > n 的情况*/
    if (rp->rio_cnt < n) cnt = rp->rio_cnt; /*rp->rio_cnt > 0*/
    memcpy(usrbuf, rp->rio_bufptr, cnt);    /*复制数据*/
    rp->rio_bufptr += cnt;
    rp->rio_cnt -= cnt;
    return cnt;
}


ssize_t	e_rio_readnb(int fd, void *usrbuf, size_t n) {
    rio_t rp;
    rio_readinitb(&rp, fd);
    size_t nleft = n;
    ssize_t nread;
    char *bufp = usrbuf;

    while (nleft > 0) {
        if ((nread = rio_read(&rp, bufp, nleft)) < 0) return -1; /*只可能会出错，中断处理在rio_read中已经被处理*/
        else if (nread == 0) break; /*EOF*/
        nleft -= nread;
        bufp += nread;
    }
    return (n - nleft);
}

/**
 *
 * @param rp
 * @param usrbuf
 * @param maxlen    行长度
 * @return
 */
ssize_t	e_rio_readlineb(int fd, void *usrbuf, size_t maxlen) {
    rio_t rp;
    rio_readinitb(&rp, fd);
    int n, rc;
    char c, *bufp = usrbuf;

    for (n = 1; n < maxlen; n++) {  /*最多读取maxlen - 1个字符*/
        if ((rc = rio_read(&rp, &c, 1)) == 1) {  /*rio_read中预加载了缓冲区最大容量的数据，实际上每次读取都是从缓冲区中读取*/
            *bufp ++ = c;
            if (c == '\n') {        /*遭遇换行符*/
                n++;
                break;
            }
        } else if (rc == 0) {       /*EOF*/
            if (n == 1) return 0;   /*没读取到数据*/
            else break;             /*已读取到数据*/
        } else return -1;           /*错误*/
    }
    *bufp = NULL;                   /*截断超出行长度的字符*/
    return n - 1;
}