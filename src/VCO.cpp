#include "VCO.h"
#include <Arduino.h>
#include <math.h>

VCO::VCO() {
    // Constructor (initialize hardware if needed)
}

float VCO::calculateCurrent(int adcRaw, float voltageOffset, float sensitivity) {
    // Convert ADC raw value (0 - 4095) to voltage (0 - 3.3V)
    float voltage = adcRaw * 3.3f / 4095.0f;
    // Calculate current: (voltage - offset) / sensitivity.
    return (voltage - voltageOffset) / sensitivity;
}

float VCO::calculateVoltage(int16_t adcRaw, float vRef, int resolution) {
    // Convert ADC raw value to voltage.
    return adcRaw * vRef / (float)resolution;
}

float VCO::calculateResistance(float vMeasured, float knownResistor, float vIn) {
    // Prevent division by zero: if vMeasured is 0 or >= supply, return INFINITY.
    if(vMeasured <= 0 || vMeasured >= vIn)
        return INFINITY;
    return (vMeasured / (vIn - vMeasured)) * knownResistor;
}

int VCO::readCurrentADC() {
    // Default: read from an analog pin dedicated to current measurement (e.g., pin 1)
    return analogRead(1);
}

int16_t VCO::readVoltageADC() {
    // Default: return a reading from the voltage measurement channel (e.g., ADS1115).
    // In a real implementation, you’d use an ADS1115 library here.
    return 0;
}

float VCO::readCurrent() {
    int adc = readCurrentADC();
    return calculateCurrent(adc);
}

float VCO::readVoltage() {
    int16_t adc = readVoltageADC();
    return calculateVoltage(adc);
}

float VCO::readVoltageRMS(unsigned int samples, unsigned int delayMs) {
    double sumSquares = 0;
    for (unsigned int i = 0; i < samples; i++) {
        int16_t adc = readVoltageADC();
        float voltage = calculateVoltage(adc);
        sumSquares += voltage * voltage;
        delay(delayMs);  // Delay between samples (can be 0 in test environment)
    }
    float meanSquares = sumSquares / samples;
    return sqrt(meanSquares);
}

float VCO::readCurrentRMS(unsigned int samples, unsigned int delayMs) {
    double sumSquares = 0;
    for (unsigned int i = 0; i < samples; i++) {
        int adc = readCurrentADC();
        float current = calculateCurrent(adc);
        sumSquares += current * current;
        delay(delayMs);
    }
    float meanSquares = sumSquares / samples;
    return sqrt(meanSquares);
}

float VCO::readResistance(float knownResistor, unsigned int samples, unsigned int delayMs, float vIn) {
    double sumVoltage = 0;
    for (unsigned int i = 0; i < samples; i++) {
        int16_t adc = readVoltageADC();
        float voltage = calculateVoltage(adc);
        sumVoltage += voltage;
        delay(delayMs);
    }
    float avgVoltage = sumVoltage / samples;
    return calculateResistance(avgVoltage, knownResistor, vIn);
}
