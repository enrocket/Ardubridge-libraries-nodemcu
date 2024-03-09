#include "Ardubridge.h"

Ardubridge::Ardubridge() : _ssid(NULL), _password(NULL), _station_id(0), _station_pass(NULL), _send_interval(0), _last_send_time(0), 
                            _ab_temp(NAN), _ab_rh(NAN), _ab_mbar(NAN), _ab_winddir(NAN), _ab_wind_avg(NAN), _ab_wind_min(NAN), 
                            _ab_wind_max(NAN), _ab_precip(NAN), _ab_uv(NAN), _ab_dewpoint(NAN) {
}
void Ardubridge::setup(const char* ssid, const char* password, int station_id, const char* station_pass, unsigned long send_interval_minutes) {
  _ssid = ssid;
  _password = password;
  _station_id = station_id;
  _station_pass = station_pass;
  _send_interval = send_interval_minutes * 60 * 1000;  // Convert minutes to milliseconds

  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void Ardubridge::loop() {
  checkWiFiConnection();
  if (millis() - _last_send_time >= _send_interval) {
    sendData();
    _last_send_time = millis();
  }
}

void Ardubridge::setTemperature(float temperature) {
  _ab_temp = temperature;
}

void Ardubridge::setHumidity(float humidity) {
  _ab_rh = humidity;
}

void Ardubridge::setPressure(float pressure) {
  _ab_mbar = pressure;
}

void Ardubridge::setWindDirection(float windDirection) {
  _ab_winddir = windDirection;
}

void Ardubridge::setWindSpeed(float windSpeed) {
  _ab_wind_avg = windSpeed;
}

void Ardubridge::setWindSpeedMin(float windSpeedMin) {
  _ab_wind_min = windSpeedMin;
}

void Ardubridge::setWindSpeedMax(float windSpeedMax) {
  _ab_wind_max = windSpeedMax;
}

void Ardubridge::setRainfall(float rainfall) {
  _ab_precip = rainfall;
}

void Ardubridge::setUVIndex(float uvIndex) {
  _ab_uv = uvIndex;
}

void Ardubridge::setDewPoint(float dewPoint) {
  _ab_dewpoint = dewPoint;
}

void Ardubridge::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost WiFi connection. Attempting to reconnect...");
    WiFi.begin(_ssid, _password);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi reconnected.");
  }
}

void Ardubridge::sendData() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = "http://ardubridge.com:80/data/recibir.php";
    if (http.begin(_wifiClient, url)) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String httpRequestData = "station_id=" + String(_station_id) + "&station_pass=" + _station_pass + 
                         "&ab_temp=" + String(_temperature) + "&ab_rh=" + String(_humidity) + 
                         "&ab_mbar=" + String(_pressure) + "&ab_winddir=" + String(_windDirection) + 
                         "&ab_wind_avg=" + String(_windSpeed) + "&ab_wind_min=" + String(_windSpeedMin) + 
                         "&ab_wind_max=" + String(_windSpeedMax) + "&ab_precip=" + String(_rainfall) + 
                         "&ab_uv=" + String(_uvIndex) + "&ab_dewpoint=" + String(_dewPoint);
      int httpResponseCode = http.POST(httpRequestData);
      if (httpResponseCode == 200) {
        Serial.println("Data sent successfully.");
      } else {
        Serial.print("Unexpected error. HTTP response code: ");
        Serial.println(httpResponseCode);
      }
      http.end();
    } else {
      Serial.println("Unable to connect to the server.");
    }
  } else {
    Serial.println("WiFi not connected.");
  }
}