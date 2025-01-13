#ifndef _LWIPOPTS_H
#define _LWIPOPTS_H

// // Generally you would define your own explicit list of lwIP options
// // (see https://www.nongnu.org/lwip/2_1_x/group__lwip__opts.html)
// //
// // This example uses a common include to avoid repetition
// // #include "lwipopts_examples_common.h"


// Enable threading support since you're using cyw43_arch_threadsafe_background
#include "lwipopts_examples_common.h"

#undef TCP_WND
#define TCP_WND  16384

#define LWIP_ALTCP 1

// If you don't want to use TLS (just a http request) you can avoid linking to mbedtls and remove the following
// #define LWIP_ALTCP_TLS           1
// #define LWIP_ALTCP_TLS_MBEDTLS   1

// The following is needed to test mDns
#define LWIP_MDNS_RESPONDER 1
#define LWIP_IGMP 1
#define LWIP_NUM_NETIF_CLIENT_DATA 1
#define MDNS_RESP_USENETIF_EXTCALLBACK  1
#define MEMP_NUM_SYS_TIMEOUT (LWIP_NUM_SYS_TIMEOUT_INTERNAL + 3)
#define MEMP_NUM_TCP_PCB 12

// Enable some httpd features
#define LWIP_HTTPD_CGI 1
#define LWIP_HTTPD_SSI 1
#define LWIP_HTTPD_SSI_MULTIPART 1
#define LWIP_HTTPD_SUPPORT_POST 1
#define LWIP_HTTPD_SSI_INCLUDE_TAG 0

#define HTTPD_POST_CGI_SUPPORT    1

#define LWIP_TCP 1                // Enable TCP support in lwIP
#define LWIP_NETCONN 0            // Disable netconn API (if using raw API)
#define LWIP_SOCKET 0             // Disable socket API (if not needed)
#define TCP_QUEUE_OOSEQ 1         // Enable out-of-order TCP queueing
#define TCP_OVERSIZE TCP_MSS      // Allow TCP oversize sending

// #define LWIP_ALTCP                1
// #define LWIP_ALTCP_TLS            1
// #define LWIP_ALTCP_TLS_MBEDTLS    1

// Generated file containing html data
// #define HTTPD_FSDATA_FILE "pico_fsdata.inc"

#endif