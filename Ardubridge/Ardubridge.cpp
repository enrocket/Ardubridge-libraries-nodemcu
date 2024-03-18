#include "Ardubridge.h"
#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

Ardubridge::Ardubridge() {
  // set default values, if -500.0 is sent, the value is not sent to the server
  ab_temp = -500.0;
  ab_rh = -500.0;
  ab_mbar = -500.0;
  ab_winddir = -500.0;
  ab_wind_avg = -500.0;
  ab_wind_min = -500.0;
  ab_wind_max = -500.0;
  ab_precip = -500.0;
  ab_uv = -500.0;
  ab_dewpoint = -500.0;
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

void Ardubridge::setWindSpeed(float windSpeedAvg) {
  ab_wind_avg = windSpeedAvg;
}

void Ardubridge::setWindSpeedMin(float windSpeedMin) {
  ab_wind_min = windSpeedMin;
}

void Ardubridge::setWindSpeedMax(float windSpeedMax) {
  ab_wind_max = windSpeedMax;
}

void Ardubridge::setPrecipitation(float precipitation) {
  ab_precip = precipitation;
}

void Ardubridge::setUV(float uv) {
  ab_uv = uv;
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
    String httpRequestData = "{";
    httpRequestData += "\"station_id\":\"" + String(ab_station_id) + "\",";
    httpRequestData += "\"station_pass\":\"" + String(ab_station_pass) + "\",";

    if (ab_temp != -500.0) {
      httpRequestData += "\"temp\":" + String(ab_temp) + ",";
    }

    if (ab_rh != -500.0) {
      httpRequestData += "\"rh\":" + String(ab_rh) + ",";
    }

    if (ab_mbar != -500.0) {
      httpRequestData += "\"mbar\":" + String(ab_mbar) + ",";
    }

    if (ab_winddir != -500.0) {
      httpRequestData += "\"winddir\":" + String(ab_winddir) + ",";
    }

    if (ab_wind_avg != -500.0) {
      httpRequestData += "\"wind_avg\":" + String(ab_wind_avg) + ",";
    }

    if (ab_wind_min != -500.0) {
      httpRequestData += "\"wind_min\":" + String(ab_wind_min) + ",";
    }

    if (ab_wind_max != -500.0) {
      httpRequestData += "\"wind_max\":" + String(ab_wind_max) + ",";
    }

    if (ab_precip != -500.0) {
      httpRequestData += "\"precip\":" + String(ab_precip) + ",";
    }

    if (ab_uv != -500.0) {
      httpRequestData += "\"uv\":" + String(ab_uv) + ",";
    }

    if (ab_dewpoint != -500.0) {
      httpRequestData += "\"dewpoint\":" + String(ab_dewpoint) + ",";
    }

    // comprobamos si el string termina con una coma
    if (httpRequestData.endsWith(",")) {
      httpRequestData = httpRequestData.substring(0, httpRequestData.length() - 1);
    }
    // cerramos el json
    httpRequestData += "}";

      WiFiClient client;
  http.begin(client, url);
    http.addHeader("Content-Type", "application/json");
    Serial.println("Sending data to Ardubridge");
    int httpResponseCode = http.POST(httpRequestData);
    String payload = http.getString();
    Serial.print("Response code: ");
    Serial.println(httpResponseCode);
    Serial.print("Ardubridge response: ");
    Serial.println(payload);
    Serial.println();
    http.end();
  } else {
    Serial.println("WiFi not connected.");
  }
}