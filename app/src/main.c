#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include "led_sensor.h"

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    printk("=== Zephyr booted Task 2 ===\n");

    const struct device *sensor = DEVICE_DT_GET(DT_ALIAS(my_led_sensor));

    if (!device_is_ready(sensor)) {
        printk("ERROR: sensor not ready\n");
        return -ENODEV;
    }

    int val;

    LOG_INF("Round 1: default interval");
    led_sensor_fetch(sensor);
    led_sensor_get(sensor, &val);
    LOG_INF("Reported interval: %d ms", val);
    k_sleep(K_MSEC(500));

    led_sensor_set_interval(sensor, 200U);

    LOG_INF("Round 2: interval 200ms");
    led_sensor_fetch(sensor);
    led_sensor_get(sensor, &val);
    LOG_INF("Reported interval: %d ms", val);
    k_sleep(K_MSEC(500));

    led_sensor_set_interval(sensor, 1000U);

    LOG_INF("Round 3: interval 1000ms");
    led_sensor_fetch(sensor);
    led_sensor_get(sensor, &val);
    LOG_INF("Reported interval: %d ms", val);
    k_sleep(K_MSEC(500));

    LOG_INF("Task 2 complete");
    while (1) {
        k_sleep(K_MSEC(1000));
    }
    return 0;
}
