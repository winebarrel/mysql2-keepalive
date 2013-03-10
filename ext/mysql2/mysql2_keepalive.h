#ifndef _MYSQL2_KEEPALIVEL_H_
#define _MYSQL2_KEEPALIVEL_H_

#ifdef HAVE_MYSQL_H
#include <mysql.h>
#else
#include <mysql/mysql.h>
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <errno.h>

#include <ruby.h>

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

// see https://github.com/brianmario/mysql2/blob/master/ext/mysql2/client.h
typedef struct {
  VALUE encoding;
  int active;
  int reconnect_enabled;
  int closed;
  MYSQL *client;
} mysql_client_wrapper;

socklen_t vio_fd(Vio* vio);

#endif // _MYSQL2_KEEPALIVEL_H_
