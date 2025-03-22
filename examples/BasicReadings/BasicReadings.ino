#include <VCO.h>

VCO vco;

void setup() {
    Serial.begin(115200);
    vco.begin();
}

void loop() {
    float voltage = vco.readVoltage();
    float current = vco.readCurrent();
    float resistance = vco.readResistance(1000.0); // 1kΩ known resistor

    Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
    Serial.print("Current: "); Serial.print(current); Serial.println(" A");
    Serial.print("Resistance: "); Serial.print(resistance); Serial.println(" Ohm");

    delay(1000);
}
