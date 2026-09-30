#include "extern.hpp"
#include "intern.hpp"

Core::HaptFlelt haptflelt;

void setup() {
  haptflelt.initSystem();
}

void loop() {
  haptflelt.process();
}

