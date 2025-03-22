#include <ArduinoFake.h>
#include <unity.h>
#include "VCO.h"

using namespace fakeit;

// Use setUp() to reset ArduinoFake and stub delay so that it does nothing.
void setUp(void) {
    ArduinoFakeReset();
    // Stub the delay() method. Since delay returns void, use AlwaysDo with a lambda.
    When(Method(ArduinoFake(), delay)).AlwaysDo([](unsigned long ms) {
        // No operation performed for delay during tests.
    });
}

void tearDown(void) {}

// Test the calculateCurrent logic function.
// For an ADC reading of 2048, the voltage is ~1.65V so that the expected current is zero.
static void test_calculateCurrent_logic() {
    VCO sensor;
    float expectedCurrent = (sensor.calculateVoltage(2048) - 1.65) / 0.185;
    float current = sensor.calculateCurrent(2048, 1.65, 0.185);
    TEST_ASSERT_FLOAT_WITHIN(0.01, expectedCurrent, current);
}

// Test readCurrent() using ArduinoFake to simulate analogRead returning 2234 on pin 1.
static void test_readCurrent_with_fake_analogRead() {
    // Fake analogRead(pin 1) to return 2234.
    When(Method(ArduinoFake(), analogRead).Using(1)).Return(2234);

    // Create a subclass that overrides readCurrentADC() to use the faked analogRead.
    class TestVCO : public VCO {
    public:
        int readCurrentADC() override {
            return analogRead(1);
        }
    };
    TestVCO sensor;
    float voltage = 2234 * 3.3f / 4095.0f;
    float expectedCurrent = (voltage - 1.65) / 0.185;
    float current = sensor.readCurrent();
    TEST_ASSERT_FLOAT_WITHIN(0.01, expectedCurrent, current);
}

// Test readVoltageRMS() by overriding readVoltageADC() to always return 2048.
static void test_readVoltageRMS() {
    class TestVCO : public VCO {
    public:
        int16_t readVoltageADC() override {
            return 2048;
        }
    };
    TestVCO sensor;
    float expectedVoltage = sensor.calculateVoltage(2048);  // ~1.65V
    // With a constant reading, RMS should equal the constant voltage.
    float rms = sensor.readVoltageRMS(100, 0); 
    TEST_ASSERT_FLOAT_WITHIN(0.01, expectedVoltage, rms);
}

// Test readCurrentRMS() by overriding readCurrentADC() to always return 2234.
static void test_readCurrentRMS() {
    class TestVCO : public VCO {
    public:
        int readCurrentADC() override {
            return 2234;
        }
    };
    TestVCO sensor;
    float voltage = 2234 * 3.3f / 4095.0f;
    float expectedCurrent = (voltage - 1.65) / 0.185;
    float rms = sensor.readCurrentRMS(100, 0);
    TEST_ASSERT_FLOAT_WITHIN(0.01, expectedCurrent, rms);
}

// Test readResistance() by overriding readVoltageADC() to return 2048.
// For a voltage divider, if the measured voltage is ~1.65V (half of 3.3V),
// then the calculated resistance equals the known resistor.
static void test_readResistance() {
    class TestVCO : public VCO {
    public:
        int16_t readVoltageADC() override {
            return 2048;
        }
    };
    TestVCO sensor;
    float knownResistor = 1000.0; // ohms
    // Expected resistance: (1.65/(3.3-1.65))*1000 = 1000 ohm.
    float expectedResistance = 1000.0;
    float resistance = sensor.readResistance(knownResistor, 100, 0, 3.3);
    TEST_ASSERT_FLOAT_WITHIN(1.0, expectedResistance, resistance);
}

// Main function modeled after the PlatformIO googletest example structure.
int main(int argc, char **argv) {
    UNITY_BEGIN();
    RUN_TEST(test_calculateCurrent_logic);
    RUN_TEST(test_readCurrent_with_fake_analogRead);
    RUN_TEST(test_readVoltageRMS);
    RUN_TEST(test_readCurrentRMS);
    RUN_TEST(test_readResistance);
    return UNITY_END();
}
