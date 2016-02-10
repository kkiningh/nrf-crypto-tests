#include "uECC.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "app_error.h"
#include "nrf_drv_rng.h"
#include "nrf_drv_rtc.h"
#include "nrf_drv_clock.h"
#include "nrf_gpio.h"
#include "sha256.h"
#include "SEGGER_RTT.h"

#define LOG(msg, ...) (SEGGER_RTT_printf(0, "%s:%d - " msg "\n", __FILE__, __LINE__, ##__VA_ARGS__))

uint8_t public_key[uECC_BYTES * 2];
uint8_t private_key[uECC_BYTES];
uint8_t hash[uECC_BYTES];
uint8_t signature[uECC_BYTES * 2];
uint8_t secret[uECC_BYTES];

uint8_t message[20];
uint8_t shahash[64];
sha256_context_t context;

const nrf_drv_rtc_t rtc = NRF_DRV_RTC_INSTANCE(0);

static void rtc_handler(nrf_drv_rtc_int_type_t int_type) {
    LOG("timer fired");
}

int main(void) {
    uint32_t err_code;
    uint32_t time0, time1;

    LOG("Starting LFCLK");
    err_code = nrf_drv_clock_init();
    APP_ERROR_CHECK(err_code);
    
    nrf_drv_clock_lfclk_request(NULL);

    LOG("Starting RTC");
    err_code = nrf_drv_rtc_init(&rtc, NULL, &rtc_handler);
    APP_ERROR_CHECK(err_code);

    nrf_drv_rtc_tick_enable(&rtc, false);
    nrf_drv_rtc_enable(&rtc);

    LOG("Starting RNG");
    err_code = nrf_drv_rng_init(NULL);
    APP_ERROR_CHECK(err_code);

    while (1) {
        /* get a random key */
        time0 = nrf_drv_rtc_counter_get(&rtc);
        uECC_make_key(public_key, private_key); 
        time1 = nrf_drv_rtc_counter_get(&rtc);
        
        LOG("Key time was %u ", time1 - time0);

        /* use the key to sign the hash */
        time0 = nrf_drv_rtc_counter_get(&rtc);
        uECC_sign(private_key, hash, signature);
        time1 = nrf_drv_rtc_counter_get(&rtc);

        LOG("Sig Time was %u ", time1 - time0);

        /* verify the signature */
        time0 = nrf_drv_rtc_counter_get(&rtc);
        uECC_verify(public_key, hash, signature);
        time1 = nrf_drv_rtc_counter_get(&rtc);

        LOG("Verify Time was %u ", time1 - time0);
        
        time0 = nrf_drv_rtc_counter_get(&rtc);
        uECC_shared_secret(public_key, private_key, secret);
        time1 = nrf_drv_rtc_counter_get(&rtc);

        LOG("SS Time was %u ", time1 - time0);

        time0 = nrf_drv_rtc_counter_get(&rtc);
        sha256_init(&context);
        sha256_update(&context, message, 20);
        sha256_final(&context, shahash);
        time1 = nrf_drv_rtc_counter_get(&rtc);

        LOG("SHA Time was %u ", time1 - time0);
    }

    return 0;
}
