#ifndef Ardubridge_h
#define Ardubridge_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <Arduino.h>

class Ardubridge {
public:
  Ardubridge();
  void setup(const char* ssid, const char* password, const char* station_id, const char* station_pass, unsigned long send_interval_minutes);
  void loop();
  void setTemperature(float ab_temp);
  void setHumidity(float ab_rh);
  void setPressure(float ab_mbar);
  void setWindDirection(float ab_winddir);
  void setWindSpeed(float ab_wind_avg);
  void setWindSpeedMin(float ab_wind_min);
  void setWindSpeedMax(float ab_wind_max);
  void setRainfall(float ab_precip);
  void setUVIndex(float ab_uv);
  void setDewPoint(float ab_dewpoint);

private:
  const char* ab_ssid;
  const char* ab_password;
  const char* ab_station_id;
  const char* ab_station_pass;
  unsigned long ab_send_interval;
  unsigned long ab_last_send_time;
  WiFiClient ab_wifiClient;
  float ab_temp;
  float ab_rh;
  float ab_mbar;
  float ab_winddir;
  float ab_wind_avg;
  float ab_wind_min;
  float ab_wind_max;
  float ab_precip;
  float ab_uv;
  float ab_dewpoint;

  void checkWiFiConnection();
  void sendData();
};

#endif