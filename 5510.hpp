#ifndef __N5110_H
#define __N5110_H

#include "stm32f1xx_hal.h"
#include "dwt_stm32_delay.h"

#define	N5110_COMMAND	0
#define N5110_DATA 1

#define N5110_CS_LOW			HAL_GPIO_WritePin(lcd->CS.port,lcd->CS.pin,GPIO_PIN_RESET)
#define N5110_CS_HIGH			HAL_GPIO_WritePin(lcd->CS.port,lcd->CS.pin,GPIO_PIN_SET)
#define N5110_DC_LOW			HAL_GPIO_WritePin(lcd->DC.port,lcd->DC.pin,GPIO_PIN_RESET)
#define N5110_DC_HIGH			HAL_GPIO_WritePin(lcd->DC.port,lcd->DC.pin,GPIO_PIN_SET)

#define N5110_DIN_LOW			HAL_GPIO_WritePin(lcd->DIN.port,lcd->DIN.pin,GPIO_PIN_RESET)
#define N5110_DIN_HIGH		HAL_GPIO_WritePin(lcd->DIN.port,lcd->DIN.pin,GPIO_PIN_SET)
#define N5110_RST_LOW			HAL_GPIO_WritePin(lcd->RST.port,lcd->RST.pin,GPIO_PIN_RESET)
#define N5110_RST_HIGH		HAL_GPIO_WritePin(lcd->RST.port,lcd->RST.pin,GPIO_PIN_SET)
#define N5110_SCK_LOW			HAL_GPIO_WritePin(lcd->SCK.port,lcd->SCK.pin,GPIO_PIN_RESET)
#define N5110_SCK_HIGH		HAL_GPIO_WritePin(lcd->SCK.port,lcd->SCK.pin,GPIO_PIN_SET)

extern IWDG_HandleTypeDef hiwdg;

static float sin[16] = {0.0,0.105,0.208,0.309,0.407,0.5,0.588,0.67,0.743,0.809,0.866,0.914,0.951,0.978,0.995,1.0};

//typedef void (*N5110_Delay_Fn)(uint32_t us);

typedef struct {
	GPIO_TypeDef* 	port;
	uint16_t 		pin;
} N5110_pin_TypeDef;

typedef struct {
	N5110_pin_TypeDef 	RST;
	N5110_pin_TypeDef 	CS;
	N5110_pin_TypeDef 	DC;
	N5110_pin_TypeDef 	DIN;
	N5110_pin_TypeDef 	SCK;
} N5110_TypeDef;


class Nokia5110
{
	public:
		Nokia5110();
		/* User API functions */
		void Init(N5110_TypeDef* _lcd);
		void WriteChar(uint8_t x, uint8_t y, uint8_t c,bool Inv=false);
		void WriteCharInv(uint8_t x, uint8_t y, uint8_t c);
		void Clear();
		void GotoXY(uint8_t X, uint8_t Y);
		void WriteStringE(uint8_t x, uint8_t y, char *s,bool Inv=false,bool WordWrap=false);
		void WriteStringF(uint8_t x, uint8_t y, char *s,bool Inv=false,bool WordWrap=false);

		void WriteDec_2(uint8_t x, uint8_t y, uint16_t b);
		void WriteDec_3(uint8_t x, uint8_t y, uint16_t b);
		void WriteDec_4(uint8_t x, uint8_t y, uint16_t b);
		void WriteDec_5(uint8_t x, uint8_t y, uint16_t b);//

		void Refresh();
		void DrawPixel(uint8_t X, uint8_t Y);
		void ClearPixel(uint8_t X, uint8_t Y);
		void DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
		void DrawCircle(uint8_t X, uint8_t Y, uint8_t R);
		void DrawRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
		void DrawClock(uint8_t X, uint8_t Y, uint8_t r, uint8_t ang);
	private:
		void WriteByte(uint8_t dat, uint8_t mode);
		void set_gpio_output(N5110_pin_TypeDef gpio);
		void set_gpio_input(N5110_pin_TypeDef gpio);
	
		N5110_TypeDef* lcd;
		uint8_t bmp[84][6];
	};
#endif
