#include <Ardubridge.h>

Ardubridge ardubridge;

void setup() {
  // Configure the WiFi connection and the Ardubridge station
  ardubridge.setup("wifi_name", "wifi_password", "your_station_id", "your_station_pass", 5);
}

void loop() {
  // Update the meteo variable values
  ardubridge.setTemperature(25.0);
  ardubridge.setHumidity(50.0);
  ardubridge.setPressure(1013.25);
  ardubridge.setWindDirection(180.0);
  ardubridge.setWindSpeed(5.0);
  ardubridge.setWindSpeedMin(2.0);
  ardubridge.setWindSpeedMax(8.0);
  ardubridge.setRainfall(0.0);
  ardubridge.setUVIndex(3.0);
  ardubridge.setDewPoint(15.0);

  // Call the Ardubridge loop method to send the data
  ardubridge.loop();
}