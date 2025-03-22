#ifndef VCO_H
#define VCO_H

#include <Arduino.h>

class VCO {
public:
    VCO();

    // Calculation functions:
    // Convert ADC raw value to current (using default parameters suited for a 3.3V, 12-bit ADC)
    // Default: voltage offset is 1.65 V (mid-scale) and sensitivity (e.g., ACS723) is 0.185 V/A.
    float calculateCurrent(int adcRaw, float voltageOffset = 1.65, float sensitivity = 0.185);

    // Convert ADC raw value to voltage.
    // For a 12-bit ADC with a 3.3V reference, resolution = 4095.
    float calculateVoltage(int16_t adcRaw, float vRef = 3.3, int resolution = 4095);

    // Calculate unknown resistance using the measured voltage across the resistor,
    // a known resistor, and the supply voltage (vIn). 
    // Formula: R_unknown = (V_measured / (vIn - V_measured)) * knownResistor.
    float calculateResistance(float vMeasured, float knownResistor, float vIn = 3.3);

    // Hardware-dependent functions.
    // Override or fake these in tests.
    virtual int readCurrentADC();       // For current sensor (e.g. ACS723 via analogRead)
    virtual int16_t readVoltageADC();     // For voltage measurement (e.g. from ADS1115)

    // High-level reading functions.
    float readCurrent();
    float readVoltage();

    // RMS measurement functions: sample multiple ADC readings and compute RMS.
    float readVoltageRMS(unsigned int samples = 100, unsigned int delayMs = 1);
    float readCurrentRMS(unsigned int samples = 100, unsigned int delayMs = 1);

    // Resistance measurement:
    // Samples the voltage reading over several measurements and calculates resistance.
    float readResistance(float knownResistor, unsigned int samples = 100, unsigned int delayMs = 1, float vIn = 3.3);
};

#endif
