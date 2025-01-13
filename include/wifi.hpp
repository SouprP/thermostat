#ifndef WIFI_SERVER_H_
#define WIFI_SERVER_H_

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/cyw43_arch.h"
#include "hardware/gpio.h"
#include <lwip/apps/httpd.h>
// #include "hardware/adc.h"

#include "lwip/tcp.h"
#include "lwip/init.h"
#include "lwip/ip4_addr.h"

#include <peltier_driver.h>

#define SSID        "pico"
#define PASSWORD    "laptop123"

static bool led_on = false;

#define LED_PIN 12

// class WebServer{
//     private:
//         const char* ssid;
//         const char* password;

//         PeltierDriver& peltier;

//         static inline err_t http_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err);
//     public:
//         static inline err_t http_server_accept(void *arg, struct tcp_pcb *pcb, err_t err);
//         void wifi_connect_static();
//         void wifi_connect();
//         void print_ip_address();
// };
// WiFi and Spring Boot configuration
// const char *ssid = "pico";
// const char *password = "laptop123";
// const char *spring_host = "192.168.29.181";
// const int spring_port = 8080;
// const char *spring_path = "/api/upload";

// // Function to handle HTTP requests
// static err_t http_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
//     if (err != ERR_OK || p == NULL) {
//         tcp_close(pcb);
//         pbuf_free(p);
//         return ERR_OK;
//     }

//     // Buffer for response
//     char response[512];
//     bool led_state = gpio_get(LED_PIN);

//     // Parse HTTP request
//     char *data =  (char *) p->payload;
//     printf(data);
//     printf("\n \n");

//     if (strstr(data, "POST /set?")) {
//         // Look for the query string
//         char *query_start = strstr(data, "/set?");
//         if (query_start) {
//             // Extract the query parameters
//             query_start += 5; // Move past "/set?"
//             char *query_end = strstr(query_start, " ");
//             if (query_end) {
//                 // Null-terminate the query string temporarily
//                 *query_end = '\0';

//                 // Look for "value=" in the query string
//                 char *value_start = strstr(query_start, "value=");
//                 if (value_start) {
//                     value_start += 6; // Move past "value="
//                     float received_value = atof(value_start); // Convert to float
//                     printf("Received float value: %f\n", received_value);

//                     // Perform an action based on the value
//                     if (received_value > 0.5) {
//                         gpio_put(LED_PIN, true);  // Example: Turn on LED if value > 0.5
//                     } else {
//                         gpio_put(LED_PIN, false); // Turn off LED otherwise
//                     }
//                 }
//                 // Restore the original request string
//                 *query_end = ' ';
//             }
//         }

//         snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n\r\n");
//     }


//     if (strstr(data, "GET /")) {
//         // LED turning OFF / ON
//         if (strstr(data, "GET /on")) {
//             gpio_put(LED_PIN, 0); // Turn on LED
//         } else if (strstr(data, "GET /off")) {
//             gpio_put(LED_PIN, 1); // Turn off LED
//         }
//     } else if (strstr(data, "POST /")) {
//         // Handle POST request (simplistic example, no body parsing)
//         if (strstr(data, "state=on")) {
//             gpio_put(LED_PIN, true);
//         } else if (strstr(data, "state=off")) {
//             gpio_put(LED_PIN, false);
//         }
//         snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n\r\n");
//     } else {
//         // Unknown request
//         snprintf(response, sizeof(response), "HTTP/1.1 404 Not Found\r\n\r\n");
//     }

//     // Send response
//     err_t write_err = tcp_write(pcb, response, strlen(response), TCP_WRITE_FLAG_COPY);
//     if (write_err != ERR_OK) {
//         printf("Error writing response: %d\n", write_err);
//     }

//     // Ensure the data is flushed
//     err_t output_err = tcp_output(pcb);
//     if (output_err != ERR_OK) {
//         printf("Error flushing response: %d\n", output_err);
//     }

//     // Free pbuf and close connection
//     pbuf_free(p);
//     tcp_close(pcb);
//     return ERR_OK;
// }

// static err_t http_server_accept(void *arg, struct tcp_pcb *pcb, err_t err) {
//     tcp_recv(pcb, http_server_recv);
//     return ERR_OK;
// }

// WiFi connection with static IP
void wifi_connect_static() {
    cyw43_arch_enable_sta_mode();

    if (cyw43_arch_wifi_connect_blocking(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK)) {
        printf("WiFi connection failed.\n");
        return;
    }

    struct netif *netif = &cyw43_state.netif[0];
    netif_set_down(netif);

    ip4_addr_t ipaddr, netmask, gateway;
    IP4_ADDR(&ipaddr, 192, 168, 29, 15);   // Static IP
    IP4_ADDR(&netmask, 255, 255, 255, 0);  // Subnet mask
    IP4_ADDR(&gateway, 192, 168, 29, 1);   // Gateway

    netif_set_addr(netif, &ipaddr, &netmask, &gateway);
    netif_set_up(netif);

    printf("Static IP configured: %s\n", ip4addr_ntoa(&ipaddr));
}

void wifi_connect() {
    cyw43_arch_enable_sta_mode();
    if (cyw43_arch_wifi_connect_blocking(SSID, PASSWORD, CYW43_AUTH_WPA2_AES_PSK)) {
        printf("WiFi connection failed.\n");
    } else {
        printf("WiFi connected.\n");
    }
}
void print_ip_address() {
    struct netif *netif = &cyw43_state.netif[0];
    if (!netif_is_up(netif)) {
        printf("Network interface is down.\n");
        return;
    }

    const ip4_addr_t *ip = netif_ip4_addr(netif);
    if (ip) {
        printf("IP Address: %s\n", ip4addr_ntoa(ip));
    } else {
        printf("Failed to retrieve IP address.\n");
    }
}

#endif