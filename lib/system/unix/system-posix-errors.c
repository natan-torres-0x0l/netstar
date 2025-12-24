#include "errors.h"


const char *
system_error_reason(system_error_t error) {
  if (!error)
    return "success";

  switch (error) {
    case EPERM:
      return "operation not permitted";

    case ENOENT:
      return "no such file or directory";

    case ESRCH:
      return "no such process";

    case EINTR:
      return "interrupted system call";

    case EIO:
      return "i/o error";

    case ENXIO:
      return "no such device or address";

    case E2BIG:
      return "argument list too long";

    case ENOEXEC:
      return "exec format error";

    case EBADF:
      return "bad file descriptor";

    case ECHILD:
      return "no child processes";

    case EAGAIN:
      return "try again";

    case ENOMEM:
      return "out of memory";

    case EACCES:
      return "permission denied";

    case EFAULT:
      return "bad address";

    case ENOTBLK:
      return "block device required";

    case EBUSY:
      return "device or resource busy";

    case EEXIST:
      return "file exists";

    case EXDEV:
      return "cross-device link";

    case ENODEV:
      return "no such device";

    case ENOTDIR:
      return "not a directory";

    case EISDIR:
      return "is a directory";

    case EINVAL:
      return "invalid argument";

    case ENFILE:
      return "file table overflow";

    case EMFILE:
      return "too many open files";

    case ENOTTY:
      return "not a typewriter";

    case ETXTBSY:
      return "text file busy";

    case EFBIG:
      return "file too large";

    case ENOSPC:
      return "no space left on device";

    case ESPIPE:
      return "illegal seek";

    case EROFS:
      return "read-only file system";

    case EMLINK:
      return "too many links";

    case EPIPE:
      return "broken pipe";

    case EDOM:
      return "math argument out of domain of func";

    case ERANGE:
      return "math result not representable";

    case EDEADLK:
      return "resource deadlock would occur";

    case ENAMETOOLONG:
      return "file name too long";

    case ENOLCK:
      return "no record locks available";

    case ENOSYS:
      return "function not implemented";

    case ENOTEMPTY:
      return "directory not empty";

    case ELOOP:
      return "too many symbolic links encountered";

    case ENOMSG:
      return "no message of desired type";

    case EIDRM:
      return "identifier removed";

    case ECHRNG:
      return "channel number out of range";

    case EL2NSYNC:
      return "level 2 not synchronized";

    case EL3HLT:
      return "level 3 halted";

    case EL3RST:
      return "level 3 reset";

    case ELNRNG:
      return "link number out of range";

    case EUNATCH:
      return "protocol driver not attached";

    case ENOCSI:
      return "no CSI structure available";

    case EL2HLT:
      return "level 2 halted";

    case EBADE:
      return "invalid exchange";

    case EBADR:
      return "invalid request descriptor";

    case EXFULL:
      return "exchange full";

    case ENOANO:
      return "no anode";

    case EBADRQC:
      return "invalid request code";

    case EBADSLT:
      return "invalid slot";

    case EBFONT:
      return "bad font file format";

    case ENOSTR:
      return "device not a stream";

    case ENODATA:
      return "no data available";

    case ETIME:
      return "timer expired";

    case ENOSR:
      return "out of streams resources";

    case ENONET:
      return "machine is not on the network";

    case ENOPKG:
      return "package not installed";

    case EREMOTE:
      return "object is remote";

    case ENOLINK:
      return "link has been severed";

    case EADV:
      return "advertise error";

    case ESRMNT:
      return "srmount error";

    case ECOMM:
      return "communication error on send";

    case EPROTO:
      return "protocol error";

    case EMULTIHOP:
      return "multihop attempted";

    case EDOTDOT:
      return "RFS specific error";

    case EBADMSG:
      return "not a data message";

    case EOVERFLOW:
      return "value too large for defined data type";

    case ENOTUNIQ:
      return "name not unique on network";

    case EBADFD:
      return "file descriptor in bad state";

    case EREMCHG:
      return "remote address changed";

    case ELIBACC:
      return "can not access a needed shared library";

    case ELIBBAD:
      return "accessing a corrupted shared library";

    case ELIBSCN:
      return ".lib section in a.out corrupted";

    case ELIBMAX:
      return "attempting to link in too many shared libraries";

    case ELIBEXEC:
      return "cannot exec a shared library directly";

    case EILSEQ:
      return "illegal byte sequence";

    case ERESTART:
      return "interrupted system call should be restarted";

    case ESTRPIPE:
      return "streams pipe error";

    case EUSERS:
      return "too many users";

    case ENOTSOCK:
      return "socket operation on non-socket";

    case EDESTADDRREQ:
      return "destination address required";

    case EMSGSIZE:
      return "message too long";

    case EPROTOTYPE:
      return "protocol wrong type for socket";

    case ENOPROTOOPT:
      return "protocol not available";

    case EPROTONOSUPPORT:
      return "protocol not supported";

    case ESOCKTNOSUPPORT:
      return "socket type not supported";

    case EOPNOTSUPP:
      return "operation not supported on transport endpoint";

    case EPFNOSUPPORT:
      return "protocol family not supported";

    case EAFNOSUPPORT:
      return "address family not supported by protocol";

    case EADDRINUSE:
      return "address already in use";

    case EADDRNOTAVAIL:
      return "cannot assign requested address";

    case ENETDOWN:
      return "network is down";

    case ENETUNREACH:
      return "network is unreachable";

    case ENETRESET:
      return "network dropped connection because of reset";

    case ECONNABORTED:
      return "software caused connection abort";

    case ECONNRESET:
      return "connection reset by peer";

    case ENOBUFS:
      return "no buffer space available";

    case EISCONN:
      return "transport endpoint is already connected";

    case ENOTCONN:
      return "transport endpoint is not connected";

    case ESHUTDOWN:
      return "cannot send after transport endpoint shutdown";

    case ETOOMANYREFS:
      return "too many references: cannot splice";

    case ETIMEDOUT:
      return "connection timed out";

    case ECONNREFUSED:
      return "connection refused";

    case EHOSTDOWN:
      return "host is down";

    case EHOSTUNREACH:
      return "no route to host";

    case EALREADY:
      return "operation already in progress";

    case EINPROGRESS:
      return "operation now in progress";

    case ESTALE:
      return "stale NFS file handle";

    case EUCLEAN:
      return "structure needs cleaning";

    case ENOTNAM:
      return "not a XENIX named type file";

    case ENAVAIL:
      return "no XENIX semaphores available";

    case EISNAM:
      return "is a named type file";

    case EREMOTEIO:
      return "remote i/o error";

    case EDQUOT:
      return "quota exceeded";

    case ENOMEDIUM:
      return "no medium found";

    case EMEDIUMTYPE:
      return "wrong medium type";

    case ECANCELED:
      return "operation Canceled";

    case ENOKEY:
      return "required key not available";

    case EKEYEXPIRED:
      return "key has expired";

    case EKEYREVOKED:
      return "key has been revoked";

    case EKEYREJECTED:
      return "key was rejected by service";

    case EOWNERDEAD:
      return "owner died";

    case ENOTRECOVERABLE:
      return "state not recoverable";

    default:
      return "unknown error";
  }
}
