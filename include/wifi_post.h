#ifndef WIFI_POST_H_
#define WIFI_POST_H_

#include "lwip/tcp.h"
#include "lwip/ip_addr.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    const char *path;
    const char *payload;
} post_request_data_t;

/**
 * Callback for handling TCP events after the connection is established.
 */
static err_t post_request_connected(void *arg, struct tcp_pcb *tpcb, err_t err) {
    if (err != ERR_OK) {
        printf("TCP connection failed with error: %d\n", err);
        tcp_close(tpcb);
        return err;
    }

    post_request_data_t *data = (post_request_data_t *)arg;

    // Prepare HTTP POST request
    char buffer[512];
    snprintf(buffer, sizeof(buffer),
             "POST %s HTTP/1.1\r\n"
             "Host: target\r\n"
             "Content-Type: application/json\r\n"
             "Content-Length: %zu\r\n"
             "\r\n"
             "%s",
             data->path, strlen(data->payload), data->payload);

    // Send POST request
    err_t write_err = tcp_write(tpcb, buffer, strlen(buffer), TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        printf("Failed to send POST request: %d\n", write_err);
        tcp_close(tpcb);
        return write_err;
    }

    tcp_output(tpcb);
    return ERR_OK;
}

/**
 * Callback for handling data sent completion.
 */
static err_t post_request_sent(void *arg, struct tcp_pcb *tpcb, u16_t len) {
    printf("POST request sent successfully.\n");
    tcp_close(tpcb);
    return ERR_OK;
}

static void send_post_request(const char *ip, uint16_t port, const char *path, const char *payload) {
    ip4_addr_t dest_ip;
    ip4addr_aton(ip, &dest_ip);

    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Failed to create TCP PCB for POST request.\n");
        return;
    }

    // Allocate memory for POST request data
    post_request_data_t *data = (post_request_data_t *)malloc(sizeof(post_request_data_t));
    if (!data) {
        printf("Failed to allocate memory for POST request data.\n");
        tcp_close(pcb);
        return;
    }

    data->path = path;
    data->payload = payload;

    // Set up connection
    err_t connect_err = tcp_connect(pcb, &dest_ip, port, post_request_connected);
    if (connect_err != ERR_OK) {
        printf("TCP connect call failed: %d\n", connect_err);
        tcp_close(pcb);
        free(data);
        return;
    }

    // Assign callbacks
    tcp_arg(pcb, data);
    tcp_sent(pcb, post_request_sent);
}

void format_json_payload(char *json_payload, size_t buffer_size, 
                         float outdoorTemp, float thermostatTemp, 
                         float humidity, float pressure, float airQuality) {
    snprintf(json_payload, buffer_size, 
             "{\"outdoorTemp\": \"%.1f\", \"thermostatTemp\": \"%.1f\", "
             "\"humidity\": \"%.1f\", \"pressure\": \"%.1f\", \"airQuality\": \"%.1f\"}", 
             outdoorTemp, thermostatTemp, humidity, pressure, airQuality);
                         }
#endif