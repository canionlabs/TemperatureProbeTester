#define ONE_WIRE_BUS D5

#include <OneWire.h>
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <SensorManager.h>

OneWire oneWire(ONE_WIRE_BUS);

Adafruit_SSD1306 display;
SensorManager *sensor;

int current = 0;
long last_change = 0;

void write(int x, int y, int size, String text)
{
    display.setCursor(x, y);
    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.print(text);
}

void updateView(int current, int total, float value, float avg, String addr, int percent)
{
    display.clearDisplay();

    write(0, 0, 1, "# " + String(current) + "/" + String(total));
    // write(0, 10, 1, addr);
    write(60, 0, 1, "AVG: " + String(avg));
    write(38, 9, 3, String(value));

    display.drawLine(percent, display.height() - 1, display.width() - 1, display.height() - 1, WHITE);

    display.display();
}

void setup()
{
    Serial.begin(9600);

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    // Init Sensor
    sensor = new SensorManager(&oneWire);

    last_change = millis();
}

void loop()
{
    sensor->service();
    float current_temp = sensor->get(current);
    String addr = sensor->getAddr(current);

    int percent = (int) map(last_change + (millis() - last_change), last_change, last_change + 5000, 0, display.width() - 1);

    updateView(current + 1, sensor->total(), current_temp, sensor->average(), addr, percent);

    if (millis() - last_change > 5000)
    {
        last_change = millis();
        current += 1;

        if (current == sensor->total())
        {
            current = 0;
        }
    }

    delay(50);
}
