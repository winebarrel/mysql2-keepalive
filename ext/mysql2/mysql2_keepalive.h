#ifndef _MYSQL2_KEEPALIVEL_H_
#define _MYSQL2_KEEPALIVEL_H_

#include <ruby.h>

#ifndef RSTRING_PTR
#define RSTRING_PTR(s) (RSTRING(s)->ptr)
#endif

#ifndef RSTRING_LEN
#define RSTRING_LEN(s) (RSTRING(s)->len)
#endif

#endif // _MYSQL2_KEEPALIVEL_H_
