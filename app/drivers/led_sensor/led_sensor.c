#define DT_DRV_COMPAT my_vendor_my_led_sensor

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(my_led_sensor, LOG_LEVEL_INF);

/* ── Per-instance config (read-only, in flash) ─────────────────────────── */
struct led_sensor_config {
    struct gpio_dt_spec led;
};

/* ── Per-instance data (mutable, in RAM) ───────────────────────────────── */
struct led_sensor_data {
    bool led_state;
};

/* ── sample_fetch: turn LED ON ─────────────────────────────────────────── */
static int led_sensor_sample_fetch(const struct device *dev,
                                   enum sensor_channel chan)
{
    const struct led_sensor_config *cfg = dev->config;
    struct led_sensor_data       *data = dev->data;
    int ret;

    ret = gpio_pin_set_dt(&cfg->led, 1);
    if (ret < 0) {
        LOG_ERR("Failed to turn LED on: %d", ret);
        return ret;
    }

    data->led_state = true;
    LOG_INF("LED turned ON  (sample_fetch)");
    return 0;
}

/* ── channel_get: turn LED OFF, return dummy value ─────────────────────── */
static int led_sensor_channel_get(const struct device *dev,
                                  enum sensor_channel chan,
                                  struct sensor_value *val)
{
    const struct led_sensor_config *cfg  = dev->config;
    struct led_sensor_data         *data = dev->data;

    gpio_pin_set_dt(&cfg->led, 0);
    data->led_state = false;
    LOG_INF("LED turned OFF (channel_get)");

    /* Return a dummy sensor reading */
    val->val1 = 1;
    val->val2 = 0;
    return 0;
}

/* ── Sensor API vtable ──────────────────────────────────────────────────── */
static const struct sensor_driver_api led_sensor_api = {
    .sample_fetch = led_sensor_sample_fetch,
    .channel_get  = led_sensor_channel_get,
};

/* ── Initialisation ─────────────────────────────────────────────────────── */
static int led_sensor_init(const struct device *dev)
{
    const struct led_sensor_config *cfg = dev->config;
    int ret;

    if (!gpio_is_ready_dt(&cfg->led)) {
        LOG_ERR("GPIO device not ready");
        return -ENODEV;
    }

    ret = gpio_pin_configure_dt(&cfg->led, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        LOG_ERR("Failed to configure LED GPIO: %d", ret);
        return ret;
    }

    LOG_INF("My LED Sensor initialised");
    return 0;
}

/* ── Per-instance macro ─────────────────────────────────────────────────── */
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
