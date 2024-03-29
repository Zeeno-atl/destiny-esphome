#pragma once

#include "esphome.h"
#include <Wire.h>
#include <array>

class PCF8591 : public PollingComponent {
  const constexpr static int POLLING_INTERVAL_MS = 1000;
  const constexpr static int PCF8591_ADDRESS = (0x90 >> 1);

public:
  std::array<Sensor *, 4> adc = {new Sensor(), new Sensor(), new Sensor(), new Sensor()};

  PCF8591() : PollingComponent(POLLING_INTERVAL_MS) {}

  void setup() override { Wire.begin(); }

  void update() override {
    Wire.beginTransmission(PCF8591_ADDRESS);
    Wire.write(0x04); // read ADC0, then auto-increment 
    Wire.endTransmission();

    Wire.requestFrom(PCF8591_ADDRESS, 5);
    Wire.read();  // dummy read of the previous value
    
    for (auto *sensor : adc) {
      sensor->publish_state(Wire.read());
    }
  }
};
