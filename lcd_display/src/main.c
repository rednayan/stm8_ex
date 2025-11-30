#include "stm8s.h"
#include <stdint.h>

#define LCD_GPIO_PORT_C (GPIOC)
#define LCD_GPIO_PINS_C (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)

#define LCD_GPIO_PORT_G (GPIOG)
#define LCD_GPIO_PINS_G (GPIO_PIN_0)

#define LCD_GPIO_PORT_D (GPIOD)
#define LCD_GPIO_PINS_D (GPIO_PIN_3)

void delay(uint16_t nCount) {
  while (nCount != 0) {
    nCount--;
  }
}

void LCD_Write_Nibble(uint8_t val) {

  if ((val & 1)) {
    /* turn PC1 high */
    GPIO_WriteHigh(LCD_GPIO_PORT_C, GPIO_PIN_1);
  } else {
    /* turn PC1 low */
    GPIO_WriteLow(LCD_GPIO_PORT_C, GPIO_PIN_1);
  }
  if ((val & (1 << 1))) {
    /* turn PG0 high */
    GPIO_WriteHigh(LCD_GPIO_PORT_G, GPIO_PIN_0);
  } else {
    /* turn PG0 low */
    GPIO_WriteLow(LCD_GPIO_PORT_G, GPIO_PIN_0);
  }
  if ((val & (1 << 2))) {
    /* turn PC2 high */
    GPIO_WriteHigh(LCD_GPIO_PORT_C, GPIO_PIN_2);
  } else {
    /* turn PC2 low */
    GPIO_WriteLow(LCD_GPIO_PORT_C, GPIO_PIN_2);
  }
  if ((val & (1 << 3))) {
    /* turn PC3 high */
    GPIO_WriteHigh(LCD_GPIO_PORT_C, GPIO_PIN_3);
  } else {
    /* turn PC3 low */
    GPIO_WriteLow(LCD_GPIO_PORT_C, GPIO_PIN_3);
  }
  GPIO_WriteHigh(LCD_GPIO_PORT_C, GPIO_PIN_4);
  delay(500);
  GPIO_WriteLow(LCD_GPIO_PORT_C, GPIO_PIN_4);
}

void LCD_Send_Command(uint8_t cmd) {
  GPIO_WriteLow(LCD_GPIO_PORT_D, GPIO_PIN_3);
  LCD_Write_Nibble(cmd >> 4);
  LCD_Write_Nibble(cmd & 0xF);
}
void LCD_Send_Char(char data) {
  GPIO_WriteHigh(LCD_GPIO_PORT_D, GPIO_PIN_3);
  LCD_Write_Nibble(data >> 4);
  LCD_Write_Nibble(data & 0xF);
}

void main(void) {

  GPIO_Init(LCD_GPIO_PORT_C, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_C,
            GPIO_MODE_OUT_PP_LOW_FAST);

  GPIO_Init(LCD_GPIO_PORT_G, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_G,
            GPIO_MODE_OUT_PP_LOW_FAST);

  GPIO_Init(LCD_GPIO_PORT_D, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_D,
            GPIO_MODE_OUT_PP_LOW_FAST);

  while (1) {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {
  while (1) {
  }
}
#endif
