#ifndef KEYPAD_H
#define KEYPAD_H

#include "gpio.h"

#define COLUMN_PORT GPIOA
#define ROW_PORT GPIOA


typedef enum
{
    KEYPAD_KEY_NONE = 0,
    KEYPAD_KEY_ASTERISK = 0b000100001,
    KEYPAD_KEY_UP = 0b000100010,
    KEYPAD_KEY_DOWN = 0b000100100,
    KEYPAD_KEY_ESC = 0b000101000,
    KEYPAD_KEY_ENT = 0b000110000,
    KEYPAD_KEY_HASH = 0b001000001,
    KEYPAD_KEY_THREE = 0b001000010,
    KEYPAD_KEY_SIX = 0b001000100,
    KEYPAD_KEY_NINE = 0b001001000,
    KEYPAD_KEY_RIGHT = 0b001010000,
    KEYPAD_KEY_F2 = 0b010000001,
    KEYPAD_KEY_TWO = 0b010000010,
    KEYPAD_KEY_FIVE = 0b010000100,
    KEYPAD_KEY_EIGHT = 0b010001000,
    KEYPAD_KEY_ZERO = 0b010010000,
    KEYPAD_KEY_F1 = 0b100000001,
    KEYPAD_KEY_ONE = 0b100000010,
    KEYPAD_KEY_FOUR = 0b100000100,
    KEYPAD_KEY_SEVEN = 0b100001000,
    KEYPAD_KEY_LEFT = 0b100010000
} Keypad_key;

void Keypad_init();
Keypad_key Keypad_read();

#endif