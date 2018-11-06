/*
* @Author: Ramon Melo
* @Date:   2018-07-24
* @Last Modified by:   Ramon Melo
* @Last Modified time: 2018-08-20
*/

#include "SensorManager.h"

SensorManager::SensorManager(OneWire *ow) : manager(ow)
{

    Serial.println("Locating devices...");
    manager.begin();
    sensor_count = manager.getDeviceCount();

    Serial.print("Found ");
    Serial.print(String(sensor_count));
    Serial.println(" devices.");

    Serial.print("Parasite power is: ");
    if (manager.isParasitePowerMode())
    {
        Serial.println("ON");
    }
    else
    {
        Serial.println("OFF");
    }

    sensors = new DeviceAddress[sensor_count];

    for (int i = 0; i < sensor_count; ++i)
    {
        if (manager.getAddress(sensors[i], i))
        {
            Serial.println("Found Device");
            manager.setResolution(sensors[i], 9);
        }
        else
        {
            Serial.println("Unable to find address for Device");
        }
    }
}

void SensorManager::service()
{
    manager.requestTemperatures();

    float avg_temp = 0;

    for (int i = 0; i < sensor_count; ++i)
    {
        float current_temp = manager.getTempC(sensors[i]);

        avg_temp += current_temp;
        info[i] = current_temp;
    }

    avg_temp /= sensor_count;
    avg = avg_temp;
}

float SensorManager::total()
{
    return sensor_count;
}

float SensorManager::get(int i)
{
    return info[i];
}

float SensorManager::average()
{
    return avg;
}

String SensorManager::getAddr(int i)
{
    return getAddr( sensors[i] );
}

String SensorManager::getAddr(DeviceAddress deviceAddress)
{
    String addr = "";

    for (uint8_t i = 0; i < 8; i++)
    {
        if (deviceAddress[i] < 16)
        {
            addr = addr + "0";
            Serial.print("0");
        }

        addr = addr + String(deviceAddress[i], HEX);
    }

    return addr;
}