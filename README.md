## e_Robust_I/O_package

### 简介
e_Robust_I/O_package(后简称eRio)是基于Unix提供的I/O接口包装和改进的健壮的I/O包，它改编自CSAPP第10章中的RIO_package。我为使用者提供了：
- 更加简单但健壮的输入输出接口。
- 公用的使用缓冲区与查看其状态的接口。



### 您什么时候可以使用它
1. eRio仅仅包装了Unix包装中的`read`和`write`功能，所以当您需要从文件中读写的时候都可以使用eRio，当然，我指的是广义上的文件，也就是您打开任何I/O设备（包括硬盘，键鼠与网络适配器等）的映射，您可以在Unix中查看它的文件描述符（File Descriter）。为了保证eRio良好的复用性，eRio使用的文件参数均是文件描述符（File Descriter）。
&nbsp;            
2. 它与Unix I/O、C语言提供的标准I/O函数的区别：
- 它比Unix I/O更加健壮，eRio提供的函数接口解决了Unix I/O返回不足值的问题，并且提供了一系列提供缓冲区的输入方法，减少了内存与I/O设备传输数据的次数，而每次传输都是要陷入内核的。
- 它比C语言提供的标准I/O函数更通用，标准I/O函数仅提供了对普通文本文件（狭义上的文件），当您使用标准I/O函数读写二进制文件或套接字文件时，会出现很多令人讨厌的问题。eRio为您提供了更加通用的接口。

### 使用说明

> 1. 下载e_Robust_I/O_package，并将它置入您的代码文件中。 
> 2. 添加头文件`include e_rio.h`



#### 无缓冲的输入输出函数
这些提供最基本的输入输出函数接口，这个函数直接在内存与文件之间传送数据。我尤其建议您使用它们来从网络中读写二进制数据。
```
ssize_t rio_readn(int fd, void *usrbuf, size_t n);
ssize_t rio_writen(int fd, void *usrbuf, size_t n);
```

#### 带缓冲的输入输出函数
这些函数允许您高效地从文件中读取文本行和二进制数据，这些文件的内容缓存在应用级缓冲区内。
1. 通用缓冲区的函数

我为您建立了一个通用的缓冲区，大小为8192字节，您只需要像使用无缓冲的输入输出函数一样使用它即可。
```
ssize_t	rio_vreadnb(int fd, void *usrbuf, size_t n);
ssize_t rio_vreadlineb(int fd, void *usrbuf, size_t n);
```
2. 自行初始化缓冲区的函数

您可以根据需求自行建立缓冲区，这允许是多个的，这代表着当您多线程使用时每个线程可以自行拥有一个缓冲区而不用担心发生线程安全的问题。


建立缓冲区的函数方法：
```
rio_t buffer; /*您可以自行定义变量缓冲区的名字*/
void rio_readinitb(int fd, rio_t *rp);
```
输入输出函数：
```
ssize_t	rio_readnb(rio_t *rp, void *usrbuf, size_t n);
ssize_t	rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen);  /*按行读取*/
```
特别注意：不要使用`rio_readlineb`读取二进制文件，它只对文本文件有效。

3. 查看缓冲区情况的函数

我为您提供了查看缓冲区当前情况的几个函数：
```
int rio_seekbuffd(const rio_t *rp);     /*查看缓冲区当前匹配的文件*/
int rio_seekbufcnt(const rio_t *rp);    /*查看缓冲区当前剩余数据数量*/
char *rio_getbufptr(rio_t *rp);         /*获取缓冲区当前的指针*/
```
同时也为您提供了查看通用缓冲区当前情况的几个函数：
```
int e_rio_seekbuffd();
int e_rio_seekbufcnt();
char e_rio_getbufptr();
```
我想我有时间的时候会继续更新这个包使得它更加地通用与强大 :)
