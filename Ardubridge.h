#ifndef Ardubridge_h
#define Ardubridge_h

#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

class Ardubridge {
public:
  Ardubridge();
  void setup(const char* ssid, const char* password, int station_id, const char* station_pass, unsigned long send_interval_minutes);
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
  const char* _ssid;
  const char* _password;
  int _station_id;
  const char* _station_pass;
  unsigned long _send_interval;
  unsigned long _last_send_time;
  WiFiClient _wifiClient;
  float _ab_temp;
  float _ab_rh;
  float _ab_mbar;
  float _ab_winddir;
  float _ab_wind_avg;
  float _ab_wind_min;
  float _ab_wind_max;
  float _ab_precip;
  float _ab_uv;
  float _ab_dewpoint;

  void checkWiFiConnection();
  void sendData();
};

#endif