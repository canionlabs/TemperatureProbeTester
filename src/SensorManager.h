#pragma once

#include "Arduino.h"
#include <DallasTemperature.h>
#include <map>

enum SensorStatus
{
    INITILIZING,
    READY
};

class SensorManager
{
  public:
    SensorManager(OneWire *ow);
    void service();
    float total();
    float get(int i);
    String getAddr(int i);
    float average();

  private:
    String getAddr(DeviceAddress deviceAddress);

    DallasTemperature manager;
    DeviceAddress *sensors;
    int sensor_count;
    float avg;

    std::map<int, float> info;

    SensorStatus status;
};