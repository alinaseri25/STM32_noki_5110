# STM32_noki_5110

to use this lib you should be enable C99 Mode and Add --cpp to misc controls (in options for target --> C/C++ tab)

then you can add library to your project and use it

  #include "dwt_stm32_delay.h"
  #include "5510.hpp"


  char data[50];


  N5110_TypeDef LCD;
  Nokia5110 NLCD;


  DWT_Delay_Init();

	
  // Nokia 5110 LCD Pins Configuration for example:
  LCD.RST.pin = GPIO_PIN_4;
	LCD.RST.port = GPIOA;
	LCD.CS.pin = GPIO_PIN_3;
	LCD.CS.port = GPIOA;
	LCD.DC.pin = GPIO_PIN_2;
	LCD.DC.port = GPIOA;
	LCD.DIN.pin = GPIO_PIN_1;
	LCD.DIN.port = GPIOA;
	LCD.SCK.pin = GPIO_PIN_0;
	LCD.SCK.port = GPIOA;
  
  
  NLCD.WriteStringE(0,0,data,false,true); // void WriteStringE(uint8_t x, uint8_t y, char *s,bool Inv=false,bool WordWrap=false);
  
	void Clear();
	void GotoXY(uint8_t X, uint8_t Y);
  void WriteChar(uint8_t x, uint8_t y, uint8_t c,bool Inv=false);
	void WriteCharInv(uint8_t x, uint8_t y, uint8_t c);
  
  void Refresh();
  void DrawPixel(uint8_t X, uint8_t Y);
  void ClearPixel(uint8_t X, uint8_t Y);
  void DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
  void DrawCircle(uint8_t X, uint8_t Y, uint8_t R);
  void DrawRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2);
  void DrawClock(uint8_t X, uint8_t Y, uint8_t r, uint8_t ang);
