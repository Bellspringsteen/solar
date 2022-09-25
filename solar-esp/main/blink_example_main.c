/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "server_calls.h"
#include "sdkconfig.h"
#include "wifi.h"
#include "esp_sleep.h"
#include <stdint.h>

#define uS_TO_S_FACTOR 1000000LL  /* Conversion factor for micro seconds to seconds */

#define TASK_PRI   4
#define TASK_STACK 8192 // 2048


static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
#define BLINK_GPIO 8

static uint8_t s_led_state = 0;
static bool wifi_task_created = false;

// #ifdef CONFIG_BLINK_LED_RMT
static led_strip_t *pStrip_a;

static void blink_led(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        if (wifi_task_created){
            pStrip_a->set_pixel(pStrip_a, 0, 16, 0, 0);

        }else{
            pStrip_a->set_pixel(pStrip_a, 0, 0, 0, 16);
        }
        /* Refresh the strip to send data */
        pStrip_a->refresh(pStrip_a, 100);
    } else {
        /* Set all LED off to clear all pixels */
        pStrip_a->clear(pStrip_a, 50);
    }
}

static void configure_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, BLINK_GPIO, 1);
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 50);
}

// // #elif 

// static void blink_led(void)
// {
//     /* Set the GPIO level according to the state (LOW or HIGH)*/
//     gpio_set_level(BLINK_GPIO, s_led_state);
// }

// static void configure_led(void)
// {
//     ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
//     gpio_reset_pin(BLINK_GPIO);
//     /* Set the GPIO as a push/pull output */
//     gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);
// }

//#endif

void app_main(void)
{

    /* Configure the peripheral according to the LED type */
    //xTaskCreate(wifi_init_sta_task, "Wifi Task", TASK_STACK, NULL, TASK_PRI, NULL);

    configure_led();
    //int seconds = 0;

    while (1) {
        //ESP_LOGI(TAG, "Turning the LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        //post_server_call(44);
        //vTaskDelay(1000 / portTICK_PERIOD_MS);
        

        xTaskCreate(wifi_init_sta_task, "Wifi Task", TASK_STACK, NULL, TASK_PRI, NULL);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        post_server_call(44);

        static uint64_t time_to_sleep_seconds = 60*60;
        esp_sleep_enable_timer_wakeup(time_to_sleep_seconds * uS_TO_S_FACTOR);
        //esp_light_sleep_start();
        //vTaskDelay(1000 / portTICK_PERIOD_MS);

        esp_deep_sleep_start(); //swap to deep sleep, sleep for 10 mins and then start from scratch
        
        //seconds++;

        //if ((seconds > 1*60) && (wifi_task_created == false)){
        //    xTaskCreate(wifi_init_sta_task, "Wifi Task", TASK_STACK, NULL, TASK_PRI, NULL);
        //    wifi_task_created = true; 
        //    vTaskDelay(1000 / portTICK_PERIOD_MS);
        //    disable_wifi();   
        //}
        //else if ((seconds > 2*60) && ((seconds%(60*2))==0))
        //{
        // enable_wifi();
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // post_server_call(44);
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        // disable_wifi();   
        //}

    }
}
