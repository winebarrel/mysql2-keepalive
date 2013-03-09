#include "mysql2_keepalive.h"

static VALUE rb_eMysql2KeepaliveError;


/* get SO_KEEPALIVE */
static VALUE rb_mysql2_keepalive_get_keepalive(VALUE self) {
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;
  socklen_t optlen;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);

  optlen = sizeof(optval);
  rv = getsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &optval, &optlen);

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return(optval ? Qtrue : Qfalse);
}

/* set SO_KEEPALIVE */
static VALUE rb_mysql2_keepalive_set_keepalive(VALUE self, VALUE vOptval) {
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;
  const char *classname;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);

  switch (TYPE(vOptval)) {
  case T_TRUE:
    optval = 1;
    break;
  case T_FALSE:
    optval = 0;
    break;
  default:
    classname = rb_class2name(CLASS_OF(vOptval));
    rb_raise(rb_eTypeError, "wrong argument type %s (expected TrueClass or FalseClass)", classname);
    break;
  }

  rv = setsockopt(sockfd, SOL_SOCKET, SO_KEEPALIVE, (void*) &optval, sizeof(optval));

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return Qnil;
}

/* get TCP_KEEPIDLE */
static VALUE rb_mysql2_keepalive_get_keepidle(VALUE self) {
#ifdef TCP_KEEPIDLE
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;
  socklen_t optlen;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);

  optlen = sizeof(optval);
  rv = getsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void*) &optval, &optlen);

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return INT2NUM(optval);
#else
  return Qnil;
#endif
}
/* set TCP_KEEPIDLE */
static VALUE rb_mysql2_keepalive_set_keepidle(VALUE self, VALUE vOptval) {
#ifdef TCP_KEEPIDLE
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);
  optval = NUM2INT(vOptval);

  rv = setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPIDLE, (void*) &optval, sizeof(optval));

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }
#endif

  return Qnil;
}

/* get TCP_KEEPINTVL */
static VALUE rb_mysql2_client_get_keepintvl(VALUE self) {
#ifdef TCP_KEEPINTVL
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;
  socklen_t optlen;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);

  optlen = sizeof(optval);
  rv = getsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, (void*) &optval, &optlen);

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return INT2NUM(optval);
#else
  return Qnil;
#endif
}

/* set TCP_KEEPINTVL */
static VALUE rb_mysql2_client_set_keepintvl(VALUE self, VALUE vOptval) {
#ifdef TCP_KEEPINTVL
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);
  optval = NUM2INT(vOptval);

  rv = setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPINTVL, (void*) &optval, sizeof(optval));

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }
#endif

  return Qnil;
}

/* get TCP_KEEPCNT */
static VALUE rb_mysql2_client_get_keepcnt(VALUE self) {
#ifdef TCP_KEEPCNT
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;
  socklen_t optlen;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);

  optlen = sizeof(optval);
  rv = getsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void*) &optval, &optlen);

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return INT2NUM(optval);
#else
  return Qnil;
#endif
}

/* set TCP_KEEPCNT */
static VALUE rb_mysql2_client_set_keepcnt(VALUE self, VALUE vOptval) {
#ifdef TCP_KEEPCNT
  mysql_client_wrapper *wrapper;
  int sockfd, optval, rv;

  Data_Get_Struct(self, mysql_client_wrapper, wrapper);
  sockfd = vio_fd(wrapper->client->net.vio);
  optval = NUM2INT(vOptval);

  rv = setsockopt(sockfd, IPPROTO_TCP, TCP_KEEPCNT, (void*) &optval, sizeof(optval));

  if (rv != 0) { 
    rb_raise(rb_eMysql2KeepaliveError, "%s", strerror(errno));
  }

  return Qnil;
#endif
}

static VALUE rb_mysql2_client_get(VALUE arg) {
  VALUE *args = (VALUE *) arg;
  return rb_const_get(args[0], args[1]);
}

void Init_keepalive() {
  VALUE rb_cMysql2, rb_cMysql2Client;
  VALUE args[2];
  int status;

  args[0] = rb_cObject;
  args[1] = rb_intern("Mysql2");

  rb_cMysql2 = rb_protect(rb_mysql2_client_get, (VALUE) &args[0], &status);

  if (status != 0) {
    rb_jump_tag(status);
  }

  args[0] = rb_cMysql2;
  args[1] = rb_intern("Client");

  rb_cMysql2Client = rb_protect(rb_mysql2_client_get, (VALUE) &args[0], &status);

  if (status != 0) {
    rb_jump_tag(status);
  }

  VALUE rb_mMysql2Keepalive = rb_define_module("Keepalive");
  rb_eMysql2KeepaliveError = rb_define_class_under(rb_mMysql2Keepalive, "Error", rb_eStandardError);

  // SO_KEEPALIVE
  rb_define_module_function(rb_cMysql2Client, "keepalive", rb_mysql2_keepalive_get_keepalive, 0);
  rb_define_module_function(rb_cMysql2Client, "keepalive=", rb_mysql2_keepalive_set_keepalive, 1);

  // TCP_KEEPIDLE
  rb_define_module_function(rb_cMysql2Client, "keepidle", rb_mysql2_keepalive_get_keepidle, 0);
  rb_define_module_function(rb_cMysql2Client, "keepidle=", rb_mysql2_keepalive_set_keepidle, 1);

  // TCP_KEEPINTVL
  rb_define_module_function(rb_cMysql2Client, "keepintvl", rb_mysql2_client_get_keepintvl, 0);
  rb_define_module_function(rb_cMysql2Client, "keepintvl=", rb_mysql2_client_set_keepintvl, 1);

  // TCP_KEEPCNT
  rb_define_module_function(rb_cMysql2Client, "keepcnt", rb_mysql2_client_get_keepcnt, 0);
  rb_define_module_function(rb_cMysql2Client, "keepcnt=", rb_mysql2_client_set_keepcnt, 1);
}
