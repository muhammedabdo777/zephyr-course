#define DT_DRV_COMPAT my_vendor_my_led_sensor

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include "led_sensor.h"

LOG_MODULE_REGISTER(my_led_sensor, LOG_LEVEL_INF);

struct led_sensor_config {
    struct gpio_dt_spec led;
};

struct led_sensor_data {
    bool     led_state;
    uint32_t interval_ms;
};

static int led_fetch_impl(const struct device *dev)
{
    const struct led_sensor_config *cfg  = dev->config;
    struct led_sensor_data         *data = dev->data;
    int ret;

    ret = gpio_pin_set_dt(&cfg->led, 1);
    if (ret < 0) {
        LOG_ERR("Failed to turn LED on: %d", ret);
        return ret;
    }

    data->led_state = true;
    LOG_INF("LED turned ON  (fetch) interval=%u ms", data->interval_ms);
    k_sleep(K_MSEC(data->interval_ms));
    return 0;
}

static int led_get_impl(const struct device *dev, int *val)
{
    const struct led_sensor_config *cfg  = dev->config;
    struct led_sensor_data         *data = dev->data;

    gpio_pin_set_dt(&cfg->led, 0);
    data->led_state = false;
    LOG_INF("LED turned OFF (get)");

    *val = (int)data->interval_ms;
    return 0;
}

static int led_set_interval_impl(const struct device *dev, uint32_t interval_ms)
{
    struct led_sensor_data *data = dev->data;
    data->interval_ms = interval_ms;
    LOG_INF("interval_ms updated to %u", interval_ms);
    return 0;
}

static const struct led_sensor_driver_api led_sensor_api = {
    .fetch        = led_fetch_impl,
    .get          = led_get_impl,
    .set_interval = led_set_interval_impl,
};

static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *cfg  = dev->config;
    struct led_sensor_data         *data = dev->data;
    int ret;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("GPIO not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure GPIO: %d", ret);
        return ret;
    }

    data->interval_ms = 500U;
    LOG_INF("My LED Sensor initialised default interval=%u ms", data->interval_ms);
    return 0;
}

#define LED_SENSOR_INST(n)                                                  \
    static struct led_sensor_data led_sensor_data_##n;                      \
                                                                            \
    static const struct led_sensor_config led_sensor_config_##n = {        \
        .led = GPIO_DT_SPEC_INST_GET(n, led_gpios),                         \
    };                                                                      \
                                                                            \
    DEVICE_DT_INST_DEFINE(n,                                                \
                          led_sensor_init,                                  \
                          NULL,                                             \
                          &led_sensor_data_##n,                             \
                          &led_sensor_config_##n,                           \
                          POST_KERNEL,                                      \
                          CONFIG_MY_LED_SENSOR_INIT_PRIORITY,               \
                          &led_sensor_api);

DT_INST_FOREACH_STATUS_OKAY(LED_SENSOR_INST)
