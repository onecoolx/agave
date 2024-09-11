
#ifndef _DIST_BUILD_H_
#define _DIST_BUILD_H_

#define OS "multi-platform"

#define BUILDING_LIBCURL 1
#define CURL_STATICLIB 1

#define USE_POLARSSL 1
#define USE_SSL 1
#define HAVE_UNISTD_H 1
#define HAVE_ZLIB 1
#define HAVE_LIBZ 1
#define HAVE_ZLIB_H 1
#define HAVE_MATH_H 1
#define HAVE_CTYPE_H 1
#define HAVE_FLOAT_H 1
#define HAVE_SYS_TIME_H 1
#define HAVE_STRUCT_TIMEVAL 1
#define HAVE_STDBOOL_H 1
#define HAVE_SYS_POLL_H 1
#define HAVE_POLL_H 1
#define HAVE_BOOL_T 1
#define HAVE_SEND 1
#define HAVE_SELECT 1
#define HAVE_POLL_FINE 1
#define HAVE_ERRNO_H 1
#define HAVE_SYS_SOCKET_H 1
#define HAVE_ARPA_INET_H 1
#define HAVE_NETDB_H 1
#define HAVE_SYS_TYPES_H 1
#define HAVE_SYS_STAT_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FCNTL_O_NONBLOCK 1
#define HAVE_IOCTL_FIONBIO 1
#define HAVE_SOCKET 1
#define HAVE_VA_COPY 1
#define HAVE_GETTIMEOFDAY 1

#define SEND_QUAL_ARG2 const
#define SEND_TYPE_ARG1 int
#define SEND_TYPE_ARG2 char*
#define SEND_TYPE_ARG3 int
#define SEND_TYPE_ARG4 int
#define SEND_TYPE_RETV int
#define HAVE_RECV 1
#define RECV_TYPE_ARG1 int
#define RECV_TYPE_ARG2 char*
#define RECV_TYPE_ARG3 size_t
#define RECV_TYPE_ARG4 int
#define RECV_TYPE_RETV int

#define SIZEOF_INT 4
#define SIZEOF_SHORT 2

#define CURL_DISABLE_TFTP 1
#define CURL_DISABLE_TELNET 1
#define CURL_DISABLE_DICT 1
#define CURL_DISABLE_LDAP 1
#define CURL_DISABLE_LDAPS 1
#define CURL_DISABLE_RTSP 1
#define CURL_DISABLE_POP3 1
#define CURL_DISABLE_IMAP 1
#define CURL_DISABLE_SMTP 1
#define CURL_DISABLE_TYPECHECK 1

#endif
