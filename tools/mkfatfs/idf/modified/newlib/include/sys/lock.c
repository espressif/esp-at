#include "lock.h"

//MVA emulate lock system

#ifdef __cplusplus
extern "C" {
#endif

void _lock_init(_lock_t *lock) {}
void _lock_init_recursive(_lock_t *lock) {}
void _lock_close(_lock_t *lock) {}
void _lock_close_recursive(_lock_t *lock) {}
void _lock_acquire(_lock_t *lock) {}
void _lock_acquire_recursive(_lock_t *lock) {}
int _lock_try_acquire(_lock_t *lock) {return 0;}
int _lock_try_acquire_recursive(_lock_t *lock) {return 0;}
void _lock_release(_lock_t *lock) {}
void _lock_release_recursive(_lock_t *lock) {}

#ifdef __cplusplus
}
#endif
