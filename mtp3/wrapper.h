/*
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

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/ioctl.h>

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <string.h>

#include "error.h"


extern int errno;

int
Accept(int fd, struct sockaddr *sa, socklen_t *salenptr);

void
Bind(int fd, const struct sockaddr *sa, socklen_t salen);

void
Connect(int fd, const struct sockaddr *sa, socklen_t salen);

void
Getpeername(int fd, struct sockaddr *sa, socklen_t *salenptr);

void
Getsockname(int fd, struct sockaddr *sa, socklen_t *salenptr);

void
Getsockopt(int fd, int level, int optname, void *optval, socklen_t *optlenptr);

int
Isfdtype(int fd, int fdtype);

/* include Listen */
void
Listen(int fd, int backlog);
/* end Listen */

#ifdef	HAVE_POLL
int
Poll(struct pollfd *fdarray, unsigned long nfds, int timeout);
#endif

ssize_t
Recv(int fd, void *ptr, size_t nbytes, int flags);

ssize_t
Recvfrom(int fd, void *ptr, size_t nbytes, int flags,
		 struct sockaddr *sa, socklen_t *salenptr);

ssize_t
Recvmsg(int fd, struct msghdr *msg, int flags);

int
Select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
       struct timeval *timeout);

void
Send(int fd, const void *ptr, size_t nbytes, int flags);

void
Sendto(int fd, const void *ptr, size_t nbytes, int flags,
	   const struct sockaddr *sa, socklen_t salen);

/*
void
Sendmsg(int fd, const struct msghdr *msg, int flags);
*/

void
Setsockopt(int fd, int level, int optname, const void *optval, socklen_t optlen);

void
Shutdown(int fd, int how);

/*
int
Sockatmark(int fd);
*/

/* include Socket */
int
Socket(int family, int type, int protocol);
/* end Socket */

void
Socketpair(int family, int type, int protocol, int *fd);


void *
Calloc(size_t n, size_t size);

void
Close(int fd);

void
Dup2(int fd1, int fd2);

int
Fcntl(int fd, int cmd, int arg);

void
Gettimeofday(struct timeval *tv, void *foo);

int
Ioctl(int fd, int request, void *arg);

pid_t
Fork(void);

void *
Malloc(size_t size);

void
Mktemp(char *tmplate);

int
Open(const char *pathname, int oflag, mode_t mode);

void
Pipe(int *fds);

ssize_t
Read(int fd, void *ptr, size_t nbytes);

void
Sigaddset(sigset_t *set, int signo);

void
Sigdelset(sigset_t *set, int signo);

void
Sigemptyset(sigset_t *set);

void
Sigfillset(sigset_t *set);

int
Sigismember(const sigset_t *set, int signo);

void
Sigpending(sigset_t *set);

void
Sigprocmask(int how, const sigset_t *set, sigset_t *oset);

char *
Strdup(const char *str);

long
Sysconf(int name);

#ifdef	HAVE_SYS_SYSCTL_H
void
Sysctl(int *name, u_int namelen, void *oldp, size_t *oldlenp,
	   void *newp, size_t newlen);
#endif

void
Unlink(const char *pathname);

pid_t
Wait(int *iptr);

pid_t
Waitpid(pid_t pid, int *iptr, int options);

void
Write(int fd, void *ptr, size_t nbytes);
