#pragma once


//MVA emulate reent

struct _idf_reent {
  int _errno;
};

struct _idf_reent* __idf_getreent();


