/* 
 * TOM - these functions are Richard Stevens
 *
 * Socket wrapper functions.
 * These could all go into separate files, so only the ones needed cause
 * the corresponding function to be added to the executable.  If sockets
 * are a library (SVR4) this might make a difference (?), but if sockets
 * are in the kernel (BSD) it doesn't matter.
 *
 * These wrapper functions also use the same prototypes as POSIX.1g,
 * which might differ from many implementations (i.e., POSIX.1g specifies
 * the fourth argument to getsockopt() as "void *", not "char *").
 *
 * If your system's headers are not correct [i.e., the Solaris 2.5
 * <sys/socket.h> omits the "const" from the second argument to both
 * bind() and connect()], you'll get warnings of the form:
 *warning: passing arg 2 of `bind' discards `const' from pointer target type
 *warning: passing arg 2 of `connect' discards `const' from pointer target type
 */

#include "wrapper.h"
#include "error.h"


int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int		n;

	if ( (n = accept(fd, sa, salenptr)) < 0) {
			errorSysRet("accept error");
	}
	return(n);
}

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (bind(fd, sa, salen) < 0)
		errorSysRet("bind error");
}

void
Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		errorSysRet("connect error");
}

void
Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getpeername(fd, sa, salenptr) < 0)
		errorSysRet("getpeername error");
}

void
Getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	if (getsockname(fd, sa, salenptr) < 0)
		errorSysRet("getsockname error");
}

void
Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr)
{
	if (getsockopt(fd, level, optname, optval, optlenptr) < 0)
		errorSysRet("getsockopt error");
}

int
Isfdtype(int fd, int fdtype)
{
	int		n;

	if ( (n = isfdtype(fd, fdtype)) < 0)
		errorSysRet("isfdtype error");
	return(n);
}

/* include Listen */
void
Listen(int fd, int backlog)
{
	char	*ptr;

	if (listen(fd, backlog) < 0)
		errorSysRet("listen error");
}
/* end Listen */

#ifdef	HAVE_POLL
int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout)
{
	int		n;

	if ( (n = poll(fdarray, nfds, timeout)) < 0)
		errorSysRet("poll error");

	return(n);
}
#endif

ssize_t
Recv(int fd, void *ptr, size_t nbytes, int flags)
{
	ssize_t		n;

	if ( (n = recv(fd, ptr, nbytes, flags)) < 0)
		errorSysRet("recv error");
	return(n);
}

ssize_t
Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
		 struct sockaddr *sa, socklen_t *salenptr)
{
	ssize_t		n;

	if ( (n = recvfrom(fd, ptr, nbytes, flags, sa, salenptr)) < 0)
		errorSysRet("recvfrom error");
	return(n);
}

ssize_t
Recvmsg(int fd, struct msghdr *msg, int flags)
{
	ssize_t		n;

	if ( (n = recvmsg(fd, msg, flags)) < 0)
		errorSysRet("recvmsg error");
	return(n);
}

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout)
{
	int		n;

	if ( (n = select(nfds, readfds, writefds, exceptfds, timeout)) < 0)
		errorSysRet("select error");
	return(n);		/* can return 0 on timeout */
}

void
Send(int fd, const void *ptr, size_t nbytes, int flags)
{
	if (send(fd, ptr, nbytes, flags) != nbytes)
		errorSysRet("send error");
}

void
Sendto(int fd, const void *ptr, size_t nbytes, int flags,
	   const struct sockaddr *sa, socklen_t salen)
{
	if (sendto(fd, ptr, nbytes, flags, sa, salen) != nbytes)
		errorSysRet("sendto error");
}

/*
void
Sendmsg(int fd, const struct msghdr *msg, int flags)
{
	int			i;
	ssize_t		nbytes;

	nbytes = 0;
	for (i = 0; i < msg->msg_iovlen; i++)
		nbytes += msg->msg_iov[i].iov_len;

	if (sendmsg(fd, msg, flags) != nbytes)
		errorSysRet("sendmsg error");
}
*/
void
Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen)
{
	if (setsockopt(fd, level, optname, optval, optlen) < 0)
		errorSysRet("setsockopt error");
}

void
Shutdown(int fd, int how)
{
	if (shutdown(fd, how) < 0)
		errorSysRet("shutdown error");
}

/*
int
Sockatmark(int fd)
{
	int		n;

	if ( (n = sockatmark(fd)) < 0)
		errorSysRet("sockatmark error");
	return(n);
}
*/

/* include Socket */
int
Socket(int family, int type, int protocol)
{
	int		n;

	if ( (n = socket(family, type, protocol)) < 0)
		errorSysRet("socket error");
	return(n);
}
/* end Socket */

void
Socketpair(int family, int type, int protocol, int *fd)
{
	int		n;

	if ( (n = socketpair(family, type, protocol, fd)) < 0)
		errorSysRet("socketpair error");
}



void *
Calloc(size_t n, size_t size)
{
	void	*ptr;

	if ( (ptr = calloc(n, size)) == NULL)
		errorSysRet("calloc error");
	return(ptr);
}

void
Close(int fd)
{
	if (close(fd) == -1)
		errorSysRet("close error");
}

void
Dup2(int fd1, int fd2)
{
	if (dup2(fd1, fd2) == -1)
		errorSysRet("dup2 error");
}

int
Fcntl(int fd, int cmd, int arg)
{
	int	n;

	if ( (n = fcntl(fd, cmd, arg)) == -1)
		errorSysRet("fcntl error");
	return(n);
}

void
Gettimeofday(struct timeval *tv, struct timezone *tz)
{
	if (gettimeofday(tv, tz) == -1)
		errorSysRet("gettimeofday error");
	return;
}

int
Ioctl(int fd, int request, void *arg)
{
	int		n;

	if ( (n = ioctl(fd, request, arg)) == -1)
		errorSysRet("ioctl error");
	return(n);	/* streamio of I_LIST returns value */
}

pid_t
Fork(void)
{
	pid_t	pid;

	if ( (pid = fork()) == -1)
		errorSysRet("fork error");
	return(pid);
}

void *
Malloc(size_t size)
{
	void	*ptr;

	if ( (ptr = malloc(size)) == NULL)
		errorSysRet("malloc error");
	return(ptr);
}

void
Mktemp(char *tmplate)
{
	if (mktemp(tmplate) == NULL || tmplate[0] == 0)
		errorSysExit("mktemp error");
}

#include	<sys/mman.h>

/*
void *
Mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	void	*ptr;

	if ( (ptr = mmap(addr, len, prot, flags, fd, offset)) == ((void *) -1))
		errorSysRet("mmap error");
	return(ptr);
}
*/

int
Open(const char *pathname, int oflag, mode_t mode)
{
	int		fd;

	if ( (fd = open(pathname, oflag, mode)) == -1)
		errorSysRet("open error for %s", pathname);
	return(fd);
}

void
Pipe(int *fds)
{
	if (pipe(fds) < 0)
		errorSysRet("pipe error");
}

ssize_t
Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		errorSysRet("read error");
	return(n);
}

void
Sigaddset(sigset_t *set, int signo)
{
	if (sigaddset(set, signo) == -1)
		errorSysRet("sigaddset error");
}

void
Sigdelset(sigset_t *set, int signo)
{
	if (sigdelset(set, signo) == -1)
		errorSysRet("sigdelset error");
}

void
Sigemptyset(sigset_t *set)
{
	if (sigemptyset(set) == -1)
		errorSysRet("sigemptyset error");
}

void
Sigfillset(sigset_t *set)
{
	if (sigfillset(set) == -1)
		errorSysRet("sigfillset error");
}

int
Sigismember(const sigset_t *set, int signo)
{
	int		n;

	if ( (n = sigismember(set, signo)) == -1)
		errorSysRet("sigismember error");
	return(n);
}

void
Sigpending(sigset_t *set)
{
	if (sigpending(set) == -1)
		errorSysRet("sigpending error");
}

void
Sigprocmask(int how, const sigset_t *set, sigset_t *oset)
{
	if (sigprocmask(how, set, oset) == -1)
		errorSysRet("sigprocmask error");
}

char *
Strdup(const char *str)
{
	char	*ptr;

	if ( (ptr = strdup(str)) == NULL)
		errorSysRet("strdup error");
	return(ptr);
}

long
Sysconf(int name)
{
	long	val;

	errno = 0;		/* in case sysconf() does not change this */
	if ( (val = sysconf(name)) == -1)
		errorSysRet("sysconf error");
	return(val);
}

#ifdef	HAVE_SYS_SYSCTL_H
void
Sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp,
	   void *newp, size_t newlen)
{
	if (sysctl(name, namelen, oldp, oldlenp, newp, newlen) == -1)
		errorSysRet("sysctl error");
}
#endif

void
Unlink(const char *pathname)
{
	if (unlink(pathname) == -1)
		errorSysRet("unlink error for %s", pathname);
}

pid_t
Wait(int *iptr)
{
	pid_t	pid;

	if ( (pid = wait(iptr)) == -1)
		errorSysRet("wait error");
	return(pid);
}

pid_t
Waitpid(pid_t pid, int *iptr, int options)
{
	pid_t	retpid;

	if ( (retpid = waitpid(pid, iptr, options)) == -1)
		errorSysRet("waitpid error");
	return(retpid);
}

void
Write(int fd, void *ptr, size_t nbytes)
{
	if (write(fd, ptr, nbytes) != nbytes)
		errorSysRet("write error");
}
