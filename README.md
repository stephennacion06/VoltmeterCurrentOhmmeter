# VoltmeterCurrentOhmmeter

## Description
The VoltmeterCurrentOhmmeter project is an Arduino library designed to measure voltage, current, and resistance in electrical circuits. It provides accurate readings and is suitable for both hobbyists and professionals.

## Features
- Measure voltage, current, and resistance.
- Easy integration into Arduino projects.
- High precision and reliability.

## Installation
1. Clone the repository:
   ```bash
   git clone https://github.com/stephennacion06/VoltmeterCurrentOhmmeter.git
   ```
2. Navigate to the project directory:
   ```bash
   cd VoltmeterCurrentOhmmeter
   ```
3. Copy the library folder into your Arduino libraries directory:
   ```bash
   cp -r VoltmeterCurrentOhmmeter /path/to/Arduino/libraries/
   ```

## Usage
1. Include the library in your Arduino sketch:
   ```cpp
   #include <VoltmeterCurrentOhmmeter.h>
   ```
2. Use the provided functions to perform measurements:
   ```cpp
   float voltage = measureVoltage();
   float current = measureCurrent();
   float resistance = measureResistance();
   Serial.print("Voltage: ");
   Serial.print(voltage);
   Serial.println(" V");
   Serial.print("Current: ");
   Serial.print(current);
   Serial.println(" A");
   Serial.print("Resistance: ");
   Serial.print(resistance);
   Serial.println(" Ω");
   ```

## Testing
This project uses PlatformIO for testing. To run the tests:
1. Install PlatformIO: [https://platformio.org/](https://platformio.org/)
2. Navigate to the project directory.
3. Run the tests:
   ```bash
   pio test
   ```

## License
This project is licensed under the MIT License. See the LICENSE file for details.
