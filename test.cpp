#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <arduinoFFT.h>

#define ONE_WIRE_BUS 4  // DS18B20 数据线
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
arduinoFFT FFT;  

const int SAMPLES = 128;  
double vReal[SAMPLES];
double vImag[SAMPLES];

void setup() {
    Serial.begin(115200);
    sensors.begin();
    Wire.begin();
    accel.begin();
}

void loop() {
    // 读取温度
    sensors.requestTemperatures();
    float tempC = sensors.getTempCByIndex(0);
    Serial.print("Temperature: "); Serial.println(tempC);

    // 读取振动
    sensors_event_t event; 
    accel.getEvent(&event);
    Serial.print("X: "); Serial.print(event.acceleration.x);
    Serial.print(" Y: "); Serial.print(event.acceleration.y);
    Serial.print(" Z: "); Serial.println(event.acceleration.z);

    // FFT 处理（示例，假设采样 128 次）
    for (int i = 0; i < SAMPLES; i++) {
        vReal[i] = event.acceleration.x; // 这里只用 X 轴，实际可用多轴
        vImag[i] = 0;
    }
    FFT.Windowing(vReal, SAMPLES, FFT_WIN_TYP_HAMMING, FFT_FORWARD);
    FFT.Compute(vReal, vImag, SAMPLES, FFT_FORWARD);
    FFT.ComplexToMagnitude(vReal, vImag, SAMPLES);
    
    // 打印 FFT 结果（示例）
    for (int i = 0; i < SAMPLES / 2; i++) {
        Serial.print("Freq "); Serial.print(i);
        Serial.print(": "); Serial.println(vReal[i]);
    }

    delay(1000);
}
