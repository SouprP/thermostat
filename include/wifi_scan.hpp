#ifndef WIFI_SCAN_HPP_
#define WIFI_SCAN_HPP_

#include <pico/cyw43_arch.h>

// char ssid[] = "INEA-2962_2.4GHz";
// char pass[] = "";

static int scan_result(void *env, const cyw43_ev_scan_result_t *result) {
    if (result) {
        printf("ssid: %-32s rssi: %4d chan: %3d mac: %02x:%02x:%02x:%02x:%02x:%02x sec: %u\n",
            result->ssid, result->rssi, result->channel,
            result->bssid[0], result->bssid[1], result->bssid[2], result->bssid[3], result->bssid[4], result->bssid[5],
            result->auth_mode);
    }
    return 0;
}

void scan_wifi(){
    while(true){
        cyw43_wifi_scan_options_t options = {0}; 
        int err = cyw43_wifi_scan(&cyw43_state, &options, NULL, scan_result);

        if(!cyw43_wifi_scan_active(&cyw43_state))
            break;
    }    
}


#endif