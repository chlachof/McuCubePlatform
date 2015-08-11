#ifndef _M_ERRORCODE_
#define _M_ERRORCODE_

#define EC_OK 0 /* Success */
#define EC_PERM 1 /* Operation not permitted */
#define EC_NOENT 2 /* No such file or directory */
#define EC_SRCH 3 /* No such process */
#define EC_INTR 4 /* Interrupted system call */
#define EC_IO 5 /* I/O error */
#define EC_NXIO 6 /* No such device or address */
#define EC_2BIG 7 /* Arg list too long */
#define EC_NOEXEC 8 /* Exec format error */
#define EC_BADF 9 /* Bad file number */
#define EC_CHILD 10 /* No child processes */
#define EC_AGAIN 11 /* Try again */
#define EC_NOMEM 12 /* Out of memory */
#define EC_ACCES 13 /* Permission denied */
#define EC_FAULT 14 /* Bad address */
#define EC_NOTBLK 15 /* Block device required */
#define EC_BUSY 16 /* Device or resource busy */
#define EC_EXIST 17 /* File exists */
#define EC_XDEV 18 /* Cross-device link */
#define EC_NODEV 19 /* No such device */
#define EC_NOTDIR 20 /* Not a directory */
#define EC_ISDIR 21 /* Is a directory */
#define EC_INVAL 22 /* Invalid argument */
#define EC_NFILE 23 /* File table overflow */
#define EC_MFILE 24 /* Too many open files */
#define EC_NOTTY 25 /* Not a typewriter */
#define EC_TXTBSY 26 /* Text file busy */
#define EC_FBIG 27 /* File too large */
#define EC_NOSPC 28 /* No space left on device */
#define EC_SPIPE 29 /* Illegal seek */
#define EC_ROFS 30 /* Read-only file system */
#define EC_MLINK 31 /* Too many links */
#define EC_PIPE 32 /* Broken pipe */
#define EC_DOM 33 /* Math argument out of domain of func */
#define EC_RANGE 34 /* Math result not representable */
#define EC_DEADLK 35 /* Resource deadlock would occur */
#define EC_NAMETOOLONG 36 /* File name too long */
#define EC_NOLCK 37 /* No record locks available */
#define EC_NOSYS 38 /* Function not implemented */
#define EC_NOTEMPTY 39 /* Directory not empty */
#define EC_LOOP 40 /* Too many symbolic links encountered */
#define EC_WOULDBLOCK EC_AGAIN /* Operation would block */
#define EC_NOMSG 42 /* No message of desired type */
#define EC_IDRM 43 /* Identifier removed */
#define EC_CHRNG 44 /* Channel number out of range */
#define EC_L2NSYNC 45 /* Level 2 not synchronized */
#define EC_L3HLT 46 /* Level 3 halted */
#define EC_L3RST 47 /* Level 3 reset */
#define EC_LNRNG 48 /* Link number out of range */
#define EC_UNATCH 49 /* Protocol driver not attached */
#define EC_NOCSI 50 /* No CSI structure available */
#define EC_L2HLT 51 /* Level 2 halted */
#define EC_BADE 52 /* Invalid exchange */
#define EC_BADR 53 /* Invalid request descriptor */
#define EC_XFULL 54 /* Exchange full */
#define EC_NOANO 55 /* No anode */
#define EC_BADRQC 56 /* Invalid request code */
#define EC_BADSLT 57 /* Invalid slot */
#define EC_DEADLOCK EC_DEADLK
#define EC_BFONT 59 /* Bad font file format */
#define EC_NOSTR 60 /* Device not a stream */
#define EC_NODATA 61 /* No data available */
#define EC_TIME 62 /* Timer expired */
#define EC_NOSR 63 /* Out of streams resources */
#define EC_NONET 64 /* Machine is not on the network */
#define EC_NOPKG 65 /* Package not installed */
#define EC_REMOTE 66 /* Object is remote */
#define EC_NOLINK 67 /* Link has been severed */
#define EC_ADV 68 /* Advertise error */
#define EC_SRMNT 69 /* Srmount error */
#define EC_COMM 70 /* Communication error on send */
#define EC_PROTO 71 /* Protocol error */
#define EC_MULTIHOP 72 /* Multihop attempted */
#define EC_DOTDOT 73 /* RFS specific error */
#define EC_BADMSG 74 /* Not a data message */
#define EC_OVERFLOW 75 /* Value too large for defined data type */
#define EC_NOTUNIQ 76 /* Name not unique on network */
#define EC_BADFD 77 /* File descriptor in bad state */
#define EC_REMCHG 78 /* Remote address changed */
#define EC_LIBACC 79 /* Can not access a needed shared library */
#define EC_LIBBAD 80 /* Accessing a corrupted shared library */
#define EC_LIBSCN 81 /* .lib section in a.out corrupted */
#define EC_LIBMAX 82 /* Attempting to link in too many shared libraries */
#define EC_LIBEXEC 83 /* Cannot exec a shared library directly */
#define EC_ILSEQ 84 /* Illegal byte sequence */
#define EC_RESTART 85 /* Interrupted system call should be restarted */
#define EC_STRPIPE 86 /* Streams pipe error */
#define EC_USERS 87 /* Too many users */
#define EC_NOTSOCK 88 /* Socket operation on non-socket */
#define EC_DESTADDRREQ 89 /* Destination address required */
#define EC_MSGSIZE 90 /* Message too long */
#define EC_PROTOTYPE 91 /* Protocol wrong type for socket */
#define EC_NOPROTOOPT 92 /* Protocol not available */
#define EC_PROTONOSUPPORT 93 /* Protocol not supported */
#define EC_SOCKTNOSUPPORT 94 /* Socket type not supported */
#define EC_OPNOTSUPP 95 /* Operation not supported on transport endpoint */
#define EC_PFNOSUPPORT 96 /* Protocol family not supported */
#define EC_AFNOSUPPORT 97 /* Address family not supported by protocol */
#define EC_ADDRINUSE 98 /* Address already in use */
#define EC_ADDRNOTAVAIL 99 /* Cannot assign requested address */
#define EC_NETDOWN 100 /* Network is down */
#define EC_NETUNREACH 101 /* Network is unreachable */
#define EC_NETRESET 102 /* Network dropped connection because of reset */
#define EC_CONNABORTED 103 /* Software caused connection abort */
#define EC_CONNRESET 104 /* Connection reset by peer */
#define EC_NOBUFS 105 /* No buffer space available */
#define EC_ISCONN 106 /* Transport endpoint is already connected */
#define EC_NOTCONN 107 /* Transport endpoint is not connected */
#define EC_SHUTDOWN 108 /* Cannot send after transport endpoint shutdown */
#define EC_TOOMANYREFS 109 /* Too many references: cannot splice */
#define EC_TIMEDOUT 110 /* Connection timed out */
#define EC_CONNREFUSED 111 /* Connection refused */
#define EC_HOSTDOWN 112 /* Host is down */
#define EC_HOSTUNREACH 113 /* No route to host */
#define EC_ALREADY 114 /* Operation already in progress */
#define EC_INPROGRESS 115 /* Operation now in progress */
#define EC_STALE 116 /* Stale NFS file handle */
#define EC_UCLEAN 117 /* Structure needs cleaning */
#define EC_NOTNAM 118 /* Not a XENIX named type file */
#define EC_NAVAIL 119 /* No XENIX semaphores available */
#define EC_ISNAM 120 /* Is a named type file */
#define EC_REMOTEIO 121 /* Remote I/O error */
#define EC_DQUOT 122 /* Quota exceeded */
#define EC_NOMEDIUM 123 /* No medium found */
#define EC_MEDIUMTYPE 124 /* Wrong medium type */

#endif