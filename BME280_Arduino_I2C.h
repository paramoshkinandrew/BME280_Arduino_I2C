#ifndef BME280_ARDUINO_I2C_H
#define BME280_ARDUINO_I2C_H

#include <Arduino.h>
#include <Wire.h>

// BME280 default address
#define BME280_DEFAULT_ADDRESS 0x76

typedef long signed int BME280_S32_t;
typedef long unsigned int BME280_U32_t;
typedef long long signed int BME280_S64_t;

struct BME280Data {
    float temperature;
    float pressure;
    uint8_t humidity;
};

class BME280_Arduino_I2C {
   public:
    BME280_Arduino_I2C(int bmeAddress = BME280_DEFAULT_ADDRESS, TwoWire* customTwoWire = &Wire);
    ~BME280_Arduino_I2C();
    uint8_t begin();
    BME280Data* read();

   private:
    unsigned long lastRead = 0;
    bool initialized = false;
    int address;
    TwoWire* wire;
    BME280Data* data = new BME280Data();
    // Registries
    uint8_t BME280_REG_ID = 0xD0;           // chip_id[7:0], should be 0xB6. Available as soon as power-reset cycle is over
    uint8_t BME280_REG_CTRL_HUM = 0xF2;     // Humidity data aquisition options. Applies only after CTRL_MEAS. Oversampling settings.
    uint8_t BME280_REG_CTRL_MEAS = 0xF4;    // Pressure and temp data configs/oversampling. Applies hummidity as well.
    uint8_t BME280_REG_CONFIG = 0xF5;       // Rate, filter and interface options.
    uint8_t BME280_REG_DATA = 0xF7;         // Pressure most significant bit output. 0xF8, 0xF9 are less and xless bits
    uint8_t BME280_REG_CALIB_T1_P9 = 0x88;  // Temperature and pressure of the calibration registers, 24 bytes, grouped by 2
    uint8_t BME280_REG_CALIB_H1 = 0xA1;     // Single char for humidity calibration
    uint8_t BME280_REG_CALIB_H2_H6 = 0xE1;  // Main humidity calibration sequence register, contains 7 bytes
    // Calibration values
    struct {
        unsigned short T1, P1;
        short T2, T3, P2, P3, P4, P5, P6, P7, P8, P9, H2, H4, H5;
        unsigned char H1, H3;
        char H6;
    } calibrationData;
    BME280_S32_t getFineResolutionTemperature(BME280_S32_t rawTemp);
    float getPressurePA(BME280_S32_t t_fine, BME280_S32_t rawPressure);
    uint8_t getHumidity(BME280_S32_t t_fine, BME280_S32_t rawHumidity);
    void writeRegister(uint8_t reg, uint8_t value);
};

#endif