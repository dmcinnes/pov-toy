#include <stdlib.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define DELAY_TIME 15
#define PATTERN_BREAK 0

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

const uint8_t checker[] = {
  0b101010,
  0b101010,
  0b010101,
  0b010101,
  0b101010,
  0b101010,
  0b010101,
  0b010101,
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

const uint8_t *patterns[] = { sine, checker, ellery };
const uint8_t patternLengths[] = {
  sizeof(sine),
  sizeof(checker),
  sizeof(ellery)
};

uint8_t buttonState = 0;
uint8_t selection = 2;

void setup() {
  // Set six pins to output
  DDRA = 0x3F;
  // Set PINB3 as input
  DDRB &= ~(1 << PINB3);
}

bool button() {
  // debounce state
  buttonState = buttonState << 1;
  if (PINB & (1 << PINB3)) {
    buttonState |= 1;
  }
  return (buttonState == 0x80);
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
  if (button()) {
    selection = (selection + 1) % sizeof(patternLengths);
  }
}

int main(void) {
  setup();

  while(1) {
    loop();
  }
}
