#ifndef LED_SENSOR_H
#define LED_SENSOR_H

#include <zephyr/device.h>

struct led_sensor_driver_api {
    int (*fetch)(const struct device *dev);
    int (*get)(const struct device *dev, int *val);
    int (*set_interval)(const struct device *dev, uint32_t interval_ms);
};

static inline int led_sensor_fetch(const struct device *dev)
{
    const struct led_sensor_driver_api *api = dev->api;
    return api->fetch(dev);
}

static inline int led_sensor_get(const struct device *dev, int *val)
{
    const struct led_sensor_driver_api *api = dev->api;
    return api->get(dev, val);
}

static inline int led_sensor_set_interval(const struct device *dev,
                                          uint32_t interval_ms)
{
    const struct led_sensor_driver_api *api = dev->api;
    return api->set_interval(dev, interval_ms);
}

#endif
