#include "stm8s.h"
#include <stdint.h>

#define TIM4_PERIOD 124

#define LCD_GPIO_PORT_C (GPIOC)
#define LCD_GPIO_PINS_C (GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | GPIO_PIN_4)

#define LCD_GPIO_PORT_G (GPIOG)
#define LCD_GPIO_PINS_G (GPIO_PIN_0)

#define LCD_GPIO_PORT_D (GPIOD)
#define LCD_GPIO_PINS_D (GPIO_PIN_3)

volatile uint32_t TimingDelay = 0;

static void CLK_Config(void);
static void TIM4_Config(void);

static void CLK_Config(void) { CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1); }

static void TIM4_Config(void) {
  TIM4_TimeBaseInit(TIM4_PRESCALER_128, TIM4_PERIOD);
  TIM4_ClearFlag(TIM4_FLAG_UPDATE);
  TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
  enableInterrupts();
  TIM4_Cmd(ENABLE);
}

void delay_ms(volatile uint32_t ms) {
  TimingDelay = ms;
  while (TimingDelay != 0) {
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
  delay_ms(5);
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

void LCD_Send_String(char *str) {
  while (*str) {
    LCD_Send_Char(*str++);
  }
}

void LCD_Init(void) {
  delay_ms(16);
  GPIO_WriteLow(LCD_GPIO_PORT_D, GPIO_PIN_3);
  LCD_Write_Nibble(0x03);
  delay_ms(5);
  LCD_Write_Nibble(0x03);
  delay_ms(1);
  LCD_Write_Nibble(0x03);
  delay_ms(1);
  LCD_Write_Nibble(0x02);
  delay_ms(1);
  LCD_Send_Command(0x28);
  delay_ms(1);
  LCD_Send_Command(0x08);
  delay_ms(1);
  LCD_Send_Command(0x01);
  delay_ms(1);
  LCD_Send_Command(0x06);
  delay_ms(1);
  LCD_Send_Command(0x0C);
}

void main(void) {

  CLK_Config();
  TIM4_Config();

  GPIO_Init(LCD_GPIO_PORT_C, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_C,
            GPIO_MODE_OUT_PP_LOW_FAST);

  GPIO_Init(LCD_GPIO_PORT_G, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_G,
            GPIO_MODE_OUT_PP_LOW_FAST);

  GPIO_Init(LCD_GPIO_PORT_D, (GPIO_Pin_TypeDef)LCD_GPIO_PINS_D,
            GPIO_MODE_OUT_PP_LOW_FAST);

  LCD_Init();
  LCD_Send_String("Hello, World!");

  while (1) {
  }
}

#ifdef USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line) {
  while (1) {
  }
}
#endif
