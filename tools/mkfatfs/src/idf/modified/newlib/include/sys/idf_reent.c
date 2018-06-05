
#include "idf_reent.h"

//MVA emulate reent

static struct _idf_reent s_r;

struct _idf_reent* __idf_getreent() {
  return &s_r;
}


