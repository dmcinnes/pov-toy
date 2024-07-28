#include <stdlib.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY_TIME 7
#define PATTERN_BREAK 10

const uint8_t sine[] = {
  0b100000,
  0b100000,
  0b010000,
  0b010000,
  0b001000,
  0b000100,
  0b000010,
  0b000010,
  0b000001,
  0b000001,
  0b000001,
  0b000010,
  0b000010,
  0b000100,
  0b001000,
  0b010000,
  0b010000,
  0b100000,
};

const uint8_t ellery[] = {
  0b111111,
  0b100101,
  0b100101,
  0b000000,
  0b111111,
  0b100000,
  0b100000,
  0b000000,
  0b111111,
  0b100000,
  0b100000,
  0b000000,
  0b111111,
  0b100101,
  0b100101,
  0b000000,
  0b111111,
  0b001101,
  0b110111,
  0b000000,
  0b100111,
  0b100100,
  0b111111,
  0b000000,
  0b000000,
  0b000000,
  0b000000,
  0b000000,
};

const uint8_t *patterns[] = { sine, ellery };
const uint8_t patternLengths[] = { sizeof(sine), sizeof(ellery) };

volatile uint8_t selection = 1;

void setup() {
  // Set six pins to output
  DDRA = 0x3F;

  sei();
}

void display(uint8_t byte) {
  PORTA = byte;
  _delay_ms(DELAY_TIME);
}

void loop() {
  const uint8_t *pattern;
  pattern = patterns[selection];
  for (uint8_t i = 0; i < patternLengths[selection]; i++) {
    display(pattern[i]);
    PORTA = 0;
    _delay_ms(PATTERN_BREAK);
  }
}

int main(void) {
  setup();

  while(1) {
    loop();
  }
}
