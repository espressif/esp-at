/* ocsp.h
 *
 * Copyright (C) 2006-2019 wolfSSL Inc.  All rights reserved.
 *
 * This file is part of wolfSSL.
 *
 * Contact licensing@wolfssl.com with any questions or comments.
 *
 * https://www.wolfssl.com
 */

/* ocsp.h for libcurl */

#ifndef WOLFSSL_OCSP_H_
#define WOLFSSL_OCSP_H_

#ifdef HAVE_OCSP
#include <wolfssl/ocsp.h>

#define OCSP_REQUEST              OcspRequest
#define OCSP_RESPONSE             OcspResponse
#define OCSP_BASICRESP            WOLFSSL_OCSP_BASICRESP
#define OCSP_CERTID               WOLFSSL_OCSP_CERTID
#define OCSP_ONEREQ               WOLFSSL_OCSP_ONEREQ

#define OCSP_REVOKED_STATUS_NOSTATUS     -1


#define OCSP_RESPONSE_STATUS_SUCCESSFUL  0
#define OCSP_RESPONSE_STATUS_TRYLATER    3

#define V_OCSP_CERTSTATUS_GOOD           0
#define V_OCSP_CERTSTATUS_REVOKED        1
#define V_OCSP_CERTSTATUS_UNKNOWN        2

#define OCSP_resp_find_status     wolfSSL_OCSP_resp_find_status
#define OCSP_cert_status_str      wolfSSL_OCSP_cert_status_str
#define OCSP_check_validity       wolfSSL_OCSP_check_validity

#define OCSP_CERTID_free          wolfSSL_OCSP_CERTID_free
#define OCSP_cert_to_id           wolfSSL_OCSP_cert_to_id

#define OCSP_BASICRESP_free       wolfSSL_OCSP_BASICRESP_free
#define OCSP_basic_verify         wolfSSL_OCSP_basic_verify

#define OCSP_RESPONSE_free        wolfSSL_OCSP_RESPONSE_free
#define d2i_OCSP_RESPONSE_bio     wolfSSL_d2i_OCSP_RESPONSE_bio
#define d2i_OCSP_RESPONSE         wolfSSL_d2i_OCSP_RESPONSE
#define i2d_OCSP_RESPONSE         wolfSSL_i2d_OCSP_RESPONSE
#define OCSP_response_status      wolfSSL_OCSP_response_status
#define OCSP_response_status_str  wolfSSL_OCSP_response_status_str
#define OCSP_response_get1_basic  wolfSSL_OCSP_response_get1_basic
#define OCSP_response_create      wolfSSL_OCSP_response_create

#define OCSP_REQUEST_new          wolfSSL_OCSP_REQUEST_new
#define OCSP_REQUEST_free         wolfSSL_OCSP_REQUEST_free
#define i2d_OCSP_REQUEST          wolfSSL_i2d_OCSP_REQUEST
#define OCSP_request_add0_id      wolfSSL_OCSP_request_add0_id
#define OCSP_request_add1_nonce   wolfSSL_OCSP_request_add1_nonce
#define OCSP_check_nonce          wolfSSL_OCSP_check_nonce
#define OCSP_id_get0_info         wolfSSL_OCSP_id_get0_info
#define OCSP_crl_reason_str       wolfSSL_OCSP_crl_reason_str
#define OCSP_REQUEST_add_ext      wolfSSL_OCSP_REQUEST_add_ext

#define OCSP_CERTID_dup           wolfSSL_OCSP_CERTID_dup

#define i2d_OCSP_REQUEST_bio      wolfSSL_i2d_OCSP_REQUEST_bio

#endif /* HAVE_OCSP */

#endif /* WOLFSSL_OCSP_H_ */

