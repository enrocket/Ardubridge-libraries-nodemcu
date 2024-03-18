#include <Ardubridge.h>

Ardubridge ardubridge;

void setup() {
  Serial.begin(9600);
  // Configure the WiFi connection and the Ardubridge station
  ardubridge.setup("wifi_name", "wifi_password", "your_station_id", "your_station_pass", 3);
}

void loop() {
  // Update the meteo variable values
  // (change fix values for real sensors values, remove rows if you don't have the sensor)
  ardubridge.setTemperature(5.0);
  ardubridge.setHumidity(55);
  ardubridge.setPressure(5.55);
  ardubridge.setWindDirection(50);
  ardubridge.setWindSpeed(5.0);
  ardubridge.setWindSpeedMin(5.0);
  ardubridge.setWindSpeedMax(5);
  ardubridge.setPrecipitation(5.0);
  ardubridge.setUV(5.0);
  ardubridge.setDewPoint(5.0);

  // Call loop method to send the data
  ardubridge.loop();
}