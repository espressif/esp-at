/* wolfevent.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

#ifndef _WOLF_EVENT_H_
#define _WOLF_EVENT_H_

#ifdef __cplusplus
    extern "C" {
#endif

#ifndef SINGLE_THREADED
    #include <wolfssl/wolfcrypt/wc_port.h>
#endif

typedef struct WOLF_EVENT WOLF_EVENT;
typedef unsigned short WOLF_EVENT_FLAG;

typedef enum WOLF_EVENT_TYPE {
    WOLF_EVENT_TYPE_NONE,
#ifdef WOLFSSL_ASYNC_CRYPT
    WOLF_EVENT_TYPE_ASYNC_WOLFSSL,    /* context is WOLFSSL* */
    WOLF_EVENT_TYPE_ASYNC_WOLFCRYPT,  /* context is WC_ASYNC_DEV */
    WOLF_EVENT_TYPE_ASYNC_FIRST = WOLF_EVENT_TYPE_ASYNC_WOLFSSL,
    WOLF_EVENT_TYPE_ASYNC_LAST = WOLF_EVENT_TYPE_ASYNC_WOLFCRYPT,
#endif /* WOLFSSL_ASYNC_CRYPT */
} WOLF_EVENT_TYPE;

typedef enum WOLF_EVENT_STATE {
    WOLF_EVENT_STATE_READY,
    WOLF_EVENT_STATE_PENDING,
    WOLF_EVENT_STATE_DONE,
} WOLF_EVENT_STATE;

struct WOLF_EVENT {
    /* double linked list */
    WOLF_EVENT*         next;
    WOLF_EVENT*         prev;

    void*               context;
    union {
        void* ptr;
#ifdef WOLFSSL_ASYNC_CRYPT
        struct WC_ASYNC_DEV* async;
#endif
    } dev;
#ifdef HAVE_CAVIUM
    word64              reqId;
    #ifdef WOLFSSL_NITROX_DEBUG
    word32              pendCount;
    #endif
#endif
#ifndef WC_NO_ASYNC_THREADING
    pthread_t           threadId;
#endif
    int                 ret;    /* Async return code */
    unsigned int        flags;
    WOLF_EVENT_TYPE     type;
    WOLF_EVENT_STATE    state;
};

enum WOLF_POLL_FLAGS {
    WOLF_POLL_FLAG_CHECK_HW = 0x01,
};

typedef struct {
    WOLF_EVENT*         head;     /* head of queue */
    WOLF_EVENT*         tail;     /* tail of queue */
#ifndef SINGLE_THREADED
    wolfSSL_Mutex       lock;     /* queue lock */
#endif
    int                 count;
} WOLF_EVENT_QUEUE;


#ifdef HAVE_WOLF_EVENT

/* Event */
WOLFSSL_API int wolfEvent_Init(WOLF_EVENT* event, WOLF_EVENT_TYPE type, void* context);
WOLFSSL_API int wolfEvent_Poll(WOLF_EVENT* event, WOLF_EVENT_FLAG flags);

/* Event Queue */
WOLFSSL_API int wolfEventQueue_Init(WOLF_EVENT_QUEUE* queue);
WOLFSSL_API int wolfEventQueue_Push(WOLF_EVENT_QUEUE* queue, WOLF_EVENT* event);
WOLFSSL_API int wolfEventQueue_Pop(WOLF_EVENT_QUEUE* queue, WOLF_EVENT** event);
WOLFSSL_API int wolfEventQueue_Poll(WOLF_EVENT_QUEUE* queue, void* context_filter,
    WOLF_EVENT** events, int maxEvents, WOLF_EVENT_FLAG flags, int* eventCount);
WOLFSSL_API int wolfEventQueue_Count(WOLF_EVENT_QUEUE* queue);
WOLFSSL_API void wolfEventQueue_Free(WOLF_EVENT_QUEUE* queue);

/* the queue mutex must be locked prior to calling these */
WOLFSSL_API int wolfEventQueue_Add(WOLF_EVENT_QUEUE* queue, WOLF_EVENT* event);
WOLFSSL_API int wolfEventQueue_Remove(WOLF_EVENT_QUEUE* queue, WOLF_EVENT* event);


#endif /* HAVE_WOLF_EVENT */


#ifdef __cplusplus
    }   /* extern "C" */
#endif

#endif /* _WOLF_EVENT_H_ */
