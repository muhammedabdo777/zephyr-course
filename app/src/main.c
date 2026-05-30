#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    const struct device *sensor = DEVICE_DT_GET(DT_ALIAS(my_led_sensor));

    if (!device_is_ready(sensor)) {
        LOG_ERR("LED sensor device not ready");
        return -ENODEV;
    }

    LOG_INF("LED Sensor Task 1 demo starting");

    while (1) {
        /* sample_fetch → LED ON */
        sensor_sample_fetch(sensor);
        k_sleep(K_MSEC(500));

        /* channel_get → LED OFF, read dummy value */
        struct sensor_value val;
        sensor_channel_get(sensor, SENSOR_CHAN_ALL, &val);
        LOG_INF("Sensor value: %d", val.val1);
        k_sleep(K_MSEC(500));
    }

    return 0;
}
