#include <stdio.h>
#include "pico/stdlib.h"
#include <pico/rand.h>
#include <hardware/i2c.h>
#include <math.h>
#include <wifi_scan.hpp>
#include <wifi.hpp>
#include <wifi_post.h>
// #include <pico/cyw43_arch.h>

#include <i2c_scan.hpp>
#include <sensors/ds18b20.h>
#include <sensors/bme680.h>
#include <peltier_driver.h>

#include <lwip/apps/httpd.h>
#include <pico/multicore.h>

#define LCD_ADDR        0x27
#define BMP_ADDR        0x76
#define BME680_ADDR     0x77

#define SDA_PIN         4
#define SCL_PIN         5

// #define LED_PIN_1       12
// #define LED_PIN_2       13

#define HEAT_PIN        22
#define COLD_PIN        16

#define DS18B20         9

// #define SSID        "pico"
// #define PASSWORD    "laptop123"

// int main(){
//     stdio_init_all();

//     // if(cyw43_arch_init())
//     //     return 1;

//     // cyw43_arch_enable_sta_mode();
//     // scan_wifi();

//     // return 0;
// }

float outdoorTemp = 25.5;
float thermostatTemp = 22.0;
float humidity = 60.5;
float pressure = 1013.25;
float airQuality = 75.0;

PeltierDriver* peltier;

// Function to handle HTTP requests
static err_t http_server_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err) {
    if (err != ERR_OK || p == NULL) {
        tcp_close(pcb);
        pbuf_free(p);
        return ERR_OK;
    }

    // Buffer for response
    char response[512];
    bool led_state = gpio_get(LED_PIN);

    // Parse HTTP request
    char *data =  (char *) p->payload;
    //printf(data);
    //printf("\n \n");

    if (strstr(data, "POST /set?")) {
        // Look for the query string
        char *query_start = strstr(data, "/set?");
        if (query_start) {
            // Extract the query parameters
            query_start += 5; // Move past "/set?"
            char *query_end = strstr(query_start, " ");
            if (query_end) {
                // Null-terminate the query string temporarily
                *query_end = '\0';

                // Look for "value=" in the query string
                char *value_start = strstr(query_start, "value=");
                if (value_start) {
                    value_start += 6; // Move past "value="
                    float received_value = atof(value_start); // Convert to float

                    peltier->set_target_temp(received_value);
                }
                // Restore the original request string
                *query_end = ' ';
            }
        }

        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n\r\n");
    }


    if (strstr(data, "GET /")) {
        // LED turning OFF / ON
        if (strstr(data, "GET /on")) {
            gpio_put(LED_PIN, 0); // Turn on LED
        } else if (strstr(data, "GET /off")) {
            gpio_put(LED_PIN, 1); // Turn off LED
        }
    } else if (strstr(data, "POST /")) {
        // Handle POST request (simplistic example, no body parsing)
        if (strstr(data, "state=on")) {
            gpio_put(LED_PIN, true);
        } else if (strstr(data, "state=off")) {
            gpio_put(LED_PIN, false);
        }
        snprintf(response, sizeof(response), "HTTP/1.1 200 OK\r\n\r\n");
    } else {
        // Unknown request
        snprintf(response, sizeof(response), "HTTP/1.1 404 Not Found\r\n\r\n");
    }

    // Send response
    err_t write_err = tcp_write(pcb, response, strlen(response), TCP_WRITE_FLAG_COPY);
    if (write_err != ERR_OK) {
        printf("Error writing response: %d\n", write_err);
    }

    // Ensure the data is flushed
    err_t output_err = tcp_output(pcb);
    if (output_err != ERR_OK) {
        printf("Error flushing response: %d\n", output_err);
    }

    // Free pbuf and close connection
    pbuf_free(p);
    tcp_close(pcb);
    return ERR_OK;
}

static err_t http_server_accept(void *arg, struct tcp_pcb *pcb, err_t err) {
    tcp_recv(pcb, http_server_recv);
    return ERR_OK;
}

void core1_task(){
    // while(true){
    //     peltier->drive();

    //     sleep_ms(500);
    // }
       uint8_t mode = 0x01;
   uint8_t press_os = 0x01;
   uint8_t temp_os = 0x01;
   uint8_t hum_os = 0x01;
   uint8_t iir_filter = 0x02;
   uint8_t set_point_index = 0x02;
   uint8_t heater_set_point = 0x01;
   uint8_t base_time = 0x02;
   uint8_t multiplier = 0x01;
   uint8_t gas_wait_x = GAS_WAIT_0 + heater_set_point;
   uint8_t target_temp = 200;
   double amb_temp = 25;
   uint8_t res_heat_addr = RES_HEAT_0 + heater_set_point;

   //Initialize I2C connection 
   bme680_init(i2c0,SDA_PIN,SCL_PIN);

   //Set oversampling for T, P and H
   set_oversampling_settings(i2c0,press_os,temp_os,hum_os);

   //Set IIR filter for the temperature sensor
   set_iir_filter_settings(i2c0,iir_filter);

   //Enable gas conversion
   enable_gas_conversion(i2c0);

   //Set index of heater set point
   set_heater_set_point(i2c0,set_point_index);

   //Define heater-on time
   set_gas_wait_time(i2c0,heater_set_point,base_time,multiplier);

   //Set heater temperature
   set_heater_temperature(i2c0,heater_set_point,target_temp,amb_temp);

   //Set mode to forced mode
   set_mode_settings(i2c0,mode);

   //Register values To be used as parameters for the function test_print_reg_values() 
   /*
   uint8_t ctrl_meas_settings = get_reg_val(i2c0,CTRL_MEAS);
   uint8_t ctrl_hum_settings = get_reg_val(i2c0,CTRL_HUM);
   uint8_t iir_filter_settings = get_reg_val(i2c0,IIR_FILTER_CONFIG);
   uint8_t ctrl_gas_1_val = get_reg_val(i2c0,CTRL_GAS_1);
   uint8_t res_heat_0_val = get_reg_val(i2c0,RES_HEAT_0);
   uint8_t par_g1_val = get_reg_val(i2c0,PAR_G1);
   uint8_t par_g2_lsb_val = get_reg_val(i2c0,PAR_G2_LSB);
   uint8_t par_g2_msb_val = get_reg_val(i2c0,PAR_G2_MSB);
   uint8_t par_g3_val = get_reg_val(i2c0,PAR_G3);
   uint8_t res_heat_range_val = get_reg_val(i2c0,RES_HEAT_RANGE);
   uint8_t res_heat_val_val = get_reg_val(i2c0,RES_HEAT_VAL);
   */

    while(1) {
        //LED Blink
        // printf("Blinking\n");
        //test_print_reg_values(ctrl_meas_settings, ctrl_hum_settings, iir_filter_settings, ctrl_gas_1_val, res_heat_0_val, par_g1_val, par_g2_lsb_val, par_g2_msb_val, par_g3_val, 
        //                            res_heat_range_val, res_heat_val_val, gas_wait_x, res_heat_addr);

        //Read calibrated TPHG data
        double temperature = get_calib_temp_data(i2c0);
        double pressure = get_calib_press_data(i2c0);
        double humidity = get_calib_hum_data(i2c0);
        double gas_resistance = get_calib_gas_res_data(i2c0);

        //Print calibrated TPHG data
        printf("Temperature (*C): %.2f\n",(float)temperature);
        printf("Pascal (hPa): %.2f\n",(float)(pressure));
        printf("Humidity (%%RH): %.2f\n",(float)humidity);
        printf("Gas Resistance (KOhms): %f\n",(float)(gas_resistance/1000));
        printf("Quality: %.2f\n", log(gas_resistance) + 0.04 * humidity);
        printf("\n");
        
        
        // gpio_put(LED_PIN,0);
        // sleep_ms(500);
        // gpio_put(LED_PIN, 1);
        // sleep_ms(500);
        
        while(!isNewDataAvailable(i2c0)) {
            // printf("Waiting for new data!\n");
            set_mode_settings(i2c0,mode);
        }

        sleep_ms(15000);
    }
}

int main() {
    // Initialize standard IO and the CYW43 WiFi library
    stdio_init_all();
    if (cyw43_arch_init()) {
        printf("Failed to initialize CYW43.\n");
        return 0;
    }

    // Connect to WiFi using static IP
    wifi_connect();
    print_ip_address();

    printf("Setting up Peltier\n");
    peltier = new PeltierDriver();
    peltier->init(HEAT_PIN, COLD_PIN, nullptr);

    multicore_launch_core1(core1_task);

    // lwip_init();
    // http_server_init();

    // Example: Send a POST request to Spring Boot server

    // const char *json_payload = "{\"outdoorTemp\": \"1.0\", \"thermostatTemp\": \"2.0\", \"humidity\": \"3.0\", \"pressure\": \"4.0\", \"airQuality\": \"5.0\"}";
    // // Main loop
    // while (1) {
    //     sleep_ms(10000);
    //     // Handle lwIP tasks
    //     send_post_request("192.168.29.181", 8080, "/api/upload", json_payload);
    // }
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_put(LED_PIN, 1);

    // Initialize lwIP stack
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Error creating TCP PCB\n");
        return 1;
    }

    // Bind to port 80 (HTTP)
    err_t err = tcp_bind(pcb, IP_ADDR_ANY, 80);
    if (err != ERR_OK) {
        printf("Error binding to port 80: %d\n", err);
        return 1;
    }

    // Start listening for incoming connections
    pcb = tcp_listen(pcb);
    tcp_accept(pcb, http_server_accept);

    printf("HTTP server running on port 80\n");

    // Run the main loop
    peltier->drive();

    return 0;
}


// int main() {
//     stdio_init_all();
//     // DS18B20 *sensor = new DS18B20(2); 
//     // BME680 bme680;


//     // PeltierDriver peltier;
//     // peltier.init(HEAT_PIN, COLD_PIN, nullptr);
//     // peltier.drive();

//     i2c_inst_t* i2c = i2c0;

//     // Initialize I2C once
//     // i2c_init(i2c, 100 * 1000);
//     // gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
//     // gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
//     // gpio_pull_up(SDA_PIN);
//     // gpio_pull_up(SCL_PIN); 

//     // gpio_init(LED_PIN);
//     // gpio_set_dir(LED_PIN, GPIO_OUT);

//     BME680 bme680;

//     // Initialize the sensor
//     printf("Initializing BME680 sensor...\n");
//     bme680.init(i2c, BME680_ADDR, SDA_PIN, SCL_PIN);

//     // Configure oversampling settings
//     printf("Configuring oversampling settings...\n");
//     bme680.set_oversampling(osr_8x, osr_8x, osr_8x);

//     // Set IIR filter coefficient
//     printf("Setting IIR filter coefficient...\n");
//     bme680.set_irf_filter_coefficient(iir_size_7);
//     bme680.enable_gas_conversion();
//     bme680.set_heater_set_point(0x02);
//     bme680.set_gas_wait_time(0x01, 0x02, 0x01);
//     bme680.set_heater_temperature(0x01, 200, 25);

//     // Start measurements in forced mode
//     printf("Starting measurements...\n");
//     bme680.force_start();

//     // Main loop to read and print sensor data
//     while (true) {
//         printf("Reading sensor data...\n");

//         // Read temperature
//         float temperature = bme680.read_temperature();
//         printf("Temperature: %.2f°C\n", temperature);

//         // Read pressure
//         float pressure = bme680.read_pressure();
//         printf("Pressure: %.2f hPa\n", pressure);

//         // Read humidity
//         float humidity = bme680.read_humidity();
//         printf("Humidity: %.2f%%\n", humidity);

//         // Read gas resistance
//         float gas_resistance = bme680.read_gas_resistance();
//         printf("Gas Resistance: %.2f Ohms\n", gas_resistance);

//         // Sleep for a while before the next read
//         sleep_ms(1000);
//     }

//     return 0;
// }


// int main() {

// //    bi_decl(bi_program_description("This is a test binary."));
// //    bi_decl(bi_1pin_with_name(LED_PIN, "On-board LED"));

//     //initialize stdio
//    stdio_init_all();

//    //Set up LED Pin
//    gpio_init(LED_PIN);
//    gpio_set_dir(LED_PIN, GPIO_OUT);

   
//    uint8_t mode = 0x01;
//    uint8_t press_os = 0x01;
//    uint8_t temp_os = 0x01;
//    uint8_t hum_os = 0x01;
//    uint8_t iir_filter = 0x02;
//    uint8_t set_point_index = 0x02;
//    uint8_t heater_set_point = 0x01;
//    uint8_t base_time = 0x02;
//    uint8_t multiplier = 0x01;
//    uint8_t gas_wait_x = GAS_WAIT_0 + heater_set_point;
//    uint8_t target_temp = 200;
//    double amb_temp = 25;
//    uint8_t res_heat_addr = RES_HEAT_0 + heater_set_point;

//    //Initialize I2C connection 
//    bme680_init(i2c0,SDA_PIN,SCL_PIN);

//    //Set oversampling for T, P and H
//    set_oversampling_settings(i2c0,press_os,temp_os,hum_os);

//    //Set IIR filter for the temperature sensor
//    set_iir_filter_settings(i2c0,iir_filter);

//    //Enable gas conversion
//    enable_gas_conversion(i2c0);

//    //Set index of heater set point
//    set_heater_set_point(i2c0,set_point_index);

//    //Define heater-on time
//    set_gas_wait_time(i2c0,heater_set_point,base_time,multiplier);

//    //Set heater temperature
//    set_heater_temperature(i2c0,heater_set_point,target_temp,amb_temp);

//    //Set mode to forced mode
//    set_mode_settings(i2c0,mode);

//    //Register values To be used as parameters for the function test_print_reg_values() 
//    /*
//    uint8_t ctrl_meas_settings = get_reg_val(i2c0,CTRL_MEAS);
//    uint8_t ctrl_hum_settings = get_reg_val(i2c0,CTRL_HUM);
//    uint8_t iir_filter_settings = get_reg_val(i2c0,IIR_FILTER_CONFIG);
//    uint8_t ctrl_gas_1_val = get_reg_val(i2c0,CTRL_GAS_1);
//    uint8_t res_heat_0_val = get_reg_val(i2c0,RES_HEAT_0);
//    uint8_t par_g1_val = get_reg_val(i2c0,PAR_G1);
//    uint8_t par_g2_lsb_val = get_reg_val(i2c0,PAR_G2_LSB);
//    uint8_t par_g2_msb_val = get_reg_val(i2c0,PAR_G2_MSB);
//    uint8_t par_g3_val = get_reg_val(i2c0,PAR_G3);
//    uint8_t res_heat_range_val = get_reg_val(i2c0,RES_HEAT_RANGE);
//    uint8_t res_heat_val_val = get_reg_val(i2c0,RES_HEAT_VAL);
//    */

//     while(1) {
//         //LED Blink
//         // printf("Blinking\n");
//         //test_print_reg_values(ctrl_meas_settings, ctrl_hum_settings, iir_filter_settings, ctrl_gas_1_val, res_heat_0_val, par_g1_val, par_g2_lsb_val, par_g2_msb_val, par_g3_val, 
//         //                            res_heat_range_val, res_heat_val_val, gas_wait_x, res_heat_addr);

//         //Read calibrated TPHG data
//         double temperature = get_calib_temp_data(i2c0);
//         double pressure = get_calib_press_data(i2c0);
//         double humidity = get_calib_hum_data(i2c0);
//         double gas_resistance = get_calib_gas_res_data(i2c0);

//         //Print calibrated TPHG data
//         printf("Temperature (*C): %.2f\n",(float)temperature);
//         printf("Pascal (hPa): %.2f\n",(float)(pressure));
//         printf("Humidity (%%RH): %.2f\n",(float)humidity);
//         printf("Gas Resistance (KOhms): %f\n",(float)(gas_resistance/1000));
//         printf("Quality: %.2f\n", log(gas_resistance) + 0.04 * humidity);
//         printf("\n");
        
        
//         gpio_put(LED_PIN,0);
//         sleep_ms(500);
//         gpio_put(LED_PIN, 1);
//         sleep_ms(500);
        
//         while(!isNewDataAvailable(i2c0)) {
//             // printf("Waiting for new data!\n");
//             set_mode_settings(i2c0,mode);
//         }
//     }
//     return 0;
// }