// LEDraw! Fast Drawing Library for FastLED by PalPalych v0.1b
// Набор быстрых графических функций для среды FastLED для матриц размером не более 128x128 светодиодов
// Концепция заключается в использовании знаковых величин int8_t для адрессации координат X и Y для быстрой проверки на условие выхода за пределы экрана
// функции с суффиксом _fast требуют только инкрементное указание координа и не соделжат проверок выхода за края матрицы
// функции без суффикса _fast содержат проверки, и выводят на экран только видимую часть объекта

#include <stdint.h>

// константы для быстрого пересчета графических функций
const int8_t  WIDTH_d         = WIDTH - 1U;                   // ширина в цифровом представлении
const int8_t  HEIGHT_d        = HEIGHT - 1U;                  // высота в цифровом представлении
const int8_t  WIDTH_odd       = WIDTH % 2U;                   // признак нечетной ширины
const int8_t  HEIGHT_odd      = HEIGHT % 2U;                  // признак нечетной высоты
const int8_t  SEMIWIDTH       = WIDTH / 2U;                   // половина ширины (минорное значение)
const int8_t  SEMIHEIGHT      = HEIGHT / 2U;                  // половина высоты (минорное значение)
const int8_t  CENTERX         = SEMIWIDTH + WIDTH_odd;        // середина по ширине (минорное значение)
const int8_t  CENTERY         = SEMIHEIGHT + HEIGHT_odd;      // середина по высоте (минорное значение)


// функция получения цвета пикселя по его номеру
uint32_t getPixelValue(uint16_t thisPixel) {
  CRGB color = leds[thisPixel];
  return (((uint32_t)color.r << 16) | ((uint32_t)color.g << 8 ) | (uint32_t)color.b);
}
uint32_t getPixelValue(uint8_t x, uint8_t y) {
  CRGB color = leds[getPixelNumber(x, y)];
  return (((uint32_t)color.r << 16) | ((uint32_t)color.g << 8 ) | (uint32_t)color.b);
}

// функция получения цвета пикселя в матрице по его координатам
CRGB getPixelColor(uint8_t x, uint8_t y) {
  return leds[getPixelNumber(x, y)];
}
