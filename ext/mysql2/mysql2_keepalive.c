#include "mysql2_keepalive.h"

static VALUE rb_eMysql2KeepaliveError;

static VALUE rb_mysql2_client_get(VALUE arg) {
  VALUE *args = (VALUE *) arg;
  return rb_const_get(args[0], args[1]);
}

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
}

