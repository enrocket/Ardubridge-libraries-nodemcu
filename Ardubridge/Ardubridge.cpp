#include "Ardubridge.h"

Ardubridge::Ardubridge() : ab_ssid(NULL), ab_password(NULL), ab_station_id(0), ab_station_pass(NULL), ab_send_interval(0), ab_last_send_time(0), 
                            ab_temp(NAN), ab_rh(NAN), ab_mbar(NAN), ab_winddir(NAN), ab_wind_avg(NAN), ab_wind_min(NAN), 
                            ab_wind_max(NAN), ab_precip(NAN), ab_uv(NAN), ab_dewpoint(NAN) {
}

void Ardubridge::setup(const char* ssid, const char* password, const char* station_id, const char* station_pass, unsigned long send_interval_minutes) {
  ab_ssid = ssid;
  ab_password = password;
  ab_station_id = station_id;
  ab_station_pass = station_pass;
  ab_send_interval = send_interval_minutes * 60 * 1000;


  WiFi.begin(ab_ssid, ab_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected.");
}

void Ardubridge::loop() {
  checkWiFiConnection();
  if (millis() - ab_last_send_time >= ab_send_interval) {
    sendData();
    ab_last_send_time = millis();
  }
}

void Ardubridge::setTemperature(float temperature) {
  ab_temp = temperature;
}

void Ardubridge::setHumidity(float humidity) {
  ab_rh = humidity;
}

void Ardubridge::setPressure(float pressure) {
  ab_mbar = pressure;
}

void Ardubridge::setWindDirection(float windDirection) {
  ab_winddir = windDirection;
}

void Ardubridge::setWindSpeed(float windSpeed) {
  ab_wind_avg = windSpeed;
}

void Ardubridge::setWindSpeedMin(float windSpeedMin) {
  ab_wind_min = windSpeedMin;
}

void Ardubridge::setWindSpeedMax(float windSpeedMax) {
  ab_wind_max = windSpeedMax;
}

void Ardubridge::setRainfall(float rainfall) {
  ab_precip = rainfall;
}

void Ardubridge::setUVIndex(float uvIndex) {
  ab_uv = uvIndex;
}

void Ardubridge::setDewPoint(float dewPoint) {
  ab_dewpoint = dewPoint;
}


void Ardubridge::checkWiFiConnection() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Lost WiFi connection. Attempting to reconnect...");
    WiFi.begin(ab_ssid, ab_password);
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
    if (http.begin(ab_wifiClient, url)) {
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        String httpRequestData = "station_id=" + String(ab_station_id) + "&station_pass=" + ab_station_pass + 
                         "&temp=" + String(ab_temp) + "&rh=" + String(ab_rh) + 
                         "&mbar=" + String(ab_mbar) + "&winddir=" + String(ab_winddir) + 
                         "&wind_avg=" + String(ab_wind_avg) + "&wind_min=" + String(ab_wind_min) + 
                         "&wind_max=" + String(ab_wind_max) + "&precip=" + String(ab_precip) + 
                         "&uv=" + String(ab_uv) + "&dewpoint=" + String(ab_dewpoint);
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