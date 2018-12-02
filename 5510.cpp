#include "5510.hpp"
#include "font.h"

void Nokia5110::set_gpio_output(N5110_pin_TypeDef gpio)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = gpio.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(gpio.port,&GPIO_InitStruct);
}
	
void Nokia5110::set_gpio_input(N5110_pin_TypeDef gpio)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = gpio.pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(gpio.port,&GPIO_InitStruct);
}

Nokia5110::Nokia5110()
{
	return;
}

void Nokia5110::WriteByte(uint8_t dat, uint8_t mode){
	uint8_t i;
	N5110_CS_LOW;

	if (mode == N5110_DATA) N5110_DC_HIGH; else N5110_DC_LOW;

	for (i = 0; i < 8; i++) {
		if(dat & 0x80) N5110_DIN_HIGH; else N5110_DIN_LOW;
		dat = dat << 1;
		N5110_SCK_LOW;
		N5110_SCK_HIGH;
	}
	N5110_CS_HIGH;
}

void Nokia5110::Init(N5110_TypeDef* _lcd) {

	DWT_Delay_Init();
	
	lcd = _lcd;
	
	set_gpio_output(lcd->CS);
	
	set_gpio_output(lcd->DC);
	
	set_gpio_output(lcd->DIN);
	
	set_gpio_output(lcd->RST);
	
	set_gpio_output(lcd->SCK);

	
	
	N5110_RST_LOW;
	N5110_DC_HIGH;
	N5110_DIN_HIGH;
	N5110_SCK_HIGH;
	N5110_CS_HIGH;
	
	

	HAL_Delay(10);

	N5110_RST_HIGH;
	
	

	//Configure LCD module
	WriteByte(0x21, N5110_COMMAND); //Extended instruction set selected
	WriteByte(0xB7, N5110_COMMAND); //Set LCD voltage (defined by experimentation...)
	WriteByte(0x14, N5110_COMMAND); //Set Bias for 1/48
	WriteByte(0x06, N5110_COMMAND); //Set temperature control (TC2)
	WriteByte(0x20, N5110_COMMAND); //Revert to standard instruction set
	Clear(); //Clear display (still off)
	WriteByte(0x0c, N5110_COMMAND); //Set display on in "normal" mode (not inversed)

}

void Nokia5110::WriteChar(uint8_t x, uint8_t y, uint8_t c,bool Inv) {
	uint8_t line;
	uint8_t ch = 0, i;
	c = c - 32;
	for (line = 0; line < 6; line++) {
		ch = fontE6_8[c][line];
		for (i=0; i<8; i++){
			if (ch&0x01)
				if(Inv) ClearPixel(x+line, y+i); else DrawPixel(x+line, y+i);
			else
				if(!Inv) ClearPixel(x+line, y+i); else DrawPixel(x+line, y+i);
			ch >>= 1;
			
		}
	}
	HAL_IWDG_Refresh(&hiwdg);
}

void Nokia5110::WriteCharInv(uint8_t x, uint8_t y, uint8_t c) {
	uint8_t line;
	uint8_t ch = 0;

	c = c - 32;

	for (line = 0; line < 6; line++) {
		ch = ~fontE6_8[c][line];
		WriteByte(ch, N5110_DATA);
		
	}
}

void Nokia5110::WriteStringF(uint8_t x, uint8_t y, char *s,bool Inv,bool WordWrap){
	uint8_t ch;
	(void)ch;
}

void Nokia5110::WriteStringE(uint8_t x, uint8_t y, char *s,bool Inv,bool WordWrap) {
	uint8_t ch;
	while (*s != '\0') {
		ch = *s;
		if(ch == '\r') 
		{
			//nothing;
		}
		else if(ch == '\n')
		{
			for(;x<84;x++)
				for(int line=0;line<8;line++)
				{
					ClearPixel(x,y + line);
				}
			x = 84;
		}
		else
		{
			WriteChar(x, y, ch, Inv);
		}
		
		if(ch >= 32)
			x+=6;
		
		if(WordWrap && x>= 84)
		{
			y+=8;
			x=0;
		}
		else if(!WordWrap && x>=84)
		{
			break;
		}
		
		s++;
	}
}


void Nokia5110::Clear() {
	uint8_t i, j;
	for (i = 0; i < 6; i++)
		for (j = 0; j < 84; j++){
			bmp[j][i] = 0;
		}
	HAL_IWDG_Refresh(&hiwdg);
	for(j=0;j<6;j++){
		for (i=0;i<84;i++){
			WriteByte(bmp[i][j], N5110_DATA);
		}
	}
}

void Nokia5110::GotoXY(uint8_t X, uint8_t Y) {
	WriteByte(0x40 | Y, N5110_COMMAND);
	WriteByte(0x80 | X, N5110_COMMAND);
}

//decimal for 2dec
void Nokia5110::WriteDec_2(uint8_t x, uint8_t y, uint16_t b) {

	uint8_t datas[3];

	datas[0] = b / 100;
	b = b - datas[0] * 100;	
	datas[1] = b / 10;
	b = b - datas[1] * 10;	
	datas[2] = b;

	datas[0] += 48;
	datas[1] += 48;
	datas[2] += 48;

	WriteChar(x,    y, datas[0]); //
	WriteChar(x+6,  y, datas[1]); //6
	WriteChar(x+12, y, datas[2]); //12
}



//decimal for 3dec
void Nokia5110::WriteDec_3(uint8_t x, uint8_t y, uint16_t b) {

	uint8_t datas[4];

	datas[0] = b / 1000;
	b = b - datas[0] * 1000;	
	datas[1] = b / 100;
	b = b - datas[1] * 100;	
	datas[2] = b / 10;
	b = b - datas[2] * 10;
	datas[3] = b;

	datas[0] += 48;
	datas[1] += 48;
	datas[2] += 48;
	datas[3] += 48;

	WriteChar(x,    y, datas[0]); //
	WriteChar(x+6,  y, datas[1]); //6
	WriteChar(x+12, y, datas[2]); //12
	WriteChar(x+18, y, datas[3]); //18
}


//decimal for 4dec
void Nokia5110::WriteDec_4(uint8_t x, uint8_t y, uint16_t b) {

	uint8_t datas[5];
	
	datas[0] = b / 10000;
	b = b - datas[0] * 10000;
	datas[1] = b / 1000;
	b = b - datas[1] * 1000;
	datas[2] = b / 100;
	b = b - datas[2] * 100;
	datas[3] = b/10;
	b = b - datas[3] * 10;
	datas[4] = b;

	datas[0] += 48;
	datas[1] += 48;
	datas[2] += 48;
	datas[3] += 48;
	datas[4] += 48;

	WriteChar(x,    y, datas[0]); //
	WriteChar(x+6,  y, datas[1]); //6
	WriteChar(x+12, y, datas[2]); //12
	WriteChar(x+18, y, datas[3]); //18
	WriteChar(x+24, y, datas[4]); //24
}

//decimal for 5dec
void Nokia5110::WriteDec_5(uint8_t x, uint8_t y, uint16_t b) {

	uint8_t datas[6];
	datas[0] = b / 100000;
	b = b - datas[0] * 100000;	
	datas[1] = b / 10000;
	b = b - datas[1] * 10000;	
	datas[2] = b / 1000;
	b = b - datas[2] * 1000;	
	datas[3] = b/100;
	b = b - datas[3] * 100;	
	datas[4] = b/10;
	b = b - datas[4] * 10;	
	datas[5] = b;

	datas[0] += 48;
	datas[1] += 48;
	datas[2] += 48;
	datas[3] += 48;
	datas[4] += 48;
	datas[5] += 48;

	WriteChar(x,    y, datas[0]); //
	WriteChar(x+6,  y, datas[1]); //6
	WriteChar(x+12, y, datas[2]); //12
	WriteChar(x+18, y, datas[3]); //18
	WriteChar(x+24, y, datas[4]); //24
	WriteChar(x+30, y, datas[5]); //30	
}

void Nokia5110::Refresh(){
	uint8_t i, j;
	for(j=0;j<6;j++){
		for (i=0;i<84;i++){
			WriteByte(bmp[i][j], N5110_DATA);
		}
	}
	HAL_IWDG_Refresh(&hiwdg);
}

void Nokia5110::DrawPixel(uint8_t X, uint8_t Y){
	GotoXY(X, Y>>3);
	bmp[X][Y>>3] |= 1<<(Y & 0x7);
	WriteByte(bmp[X][Y>>3], N5110_DATA);
}

void Nokia5110::ClearPixel(uint8_t X, uint8_t Y){
	GotoXY(X, Y>>3);
	bmp[X][Y>>3] &= ~(1<<(Y & 0x7));
	WriteByte(bmp[X][Y>>3], N5110_DATA);
}

void Nokia5110::DrawLine(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2){
	uint8_t dx, dy, i, j;
	int8_t e =0;

	dx = (X2>X1) ? (X2-X1) : (X1-X2);
	dy = (Y2>Y1) ? (Y2-Y1) : (Y1-Y2);

	if (dx >= dy){
		j = Y1;
		if (X1 < X2){
			for (i=X1; i<X2; i++){
				DrawPixel(i, j);
				
				e = e + dy;
				if (2*e >= dx){
					j += (Y2>Y1)? 1 : -1;
					e = e - dx;
				}
			}
		} else {
			for (i=X1; i>X2; i--){
				DrawPixel(i, j);
				
				e = e + dy;
				if (2*e >= dx){
					j += (Y2>Y1)? 1 : -1;
					e = e - dx;
				}
			}
		}
	} else {
		i = X1;
		if (Y1<Y2){
			for (j=Y1; j<Y2; j++){
				DrawPixel(i, j);
				
				e = e + dx;
				if (2*e >= dy){
					i += (X2>X1)? 1 : -1;
					e = e - dy;
				}
			}
		} else {
			for (j=Y1; j>Y2; j--){
				DrawPixel(i, j);
				
				e = e + dx;
				if (2*e >= dy){
					i += (X2>X1)? 1 : -1;
					e = e - dy;
				}
			}
		}

	}
}

void Nokia5110::DrawCircle(uint8_t X, uint8_t Y, uint8_t R){
	uint8_t x=0, y=R;
	int8_t d, e=0;

	while(y > 0){
		
		DrawPixel(X + x, Y + y);
		DrawPixel(X + x, Y - y);
		
		DrawPixel(X - x, Y + y);
		DrawPixel(X - x, Y - y);
		

		e = 2 * (d + y) - 1;
		if ((d<0)&&(e<=0)){
			d += 2*(++x) + 1;
			continue;
		}
		e = 2 * (d-x) - 1;
		if ((d>0)&&(e>0)){
			d += 1 - 2* (--y);
			continue;
		}
		x++;
		d += 2*(x-y);
		y--;
	}
	DrawPixel(X + x, Y);
	DrawPixel(X - x, Y);
	
}

void Nokia5110::DrawRect(uint8_t X1, uint8_t Y1, uint8_t X2, uint8_t Y2){
	DrawLine(X1, Y1, X2, Y1);
	
	DrawLine(X1, Y2, X2, Y2);
	
	DrawLine(X1, Y1, X1, Y2);
	
	DrawLine(X2, Y1, X2, Y2);
	
}

void Nokia5110::DrawClock(uint8_t X, uint8_t Y, uint8_t r, uint8_t ang){
	int8_t x,y;
	if (ang>45){
		x = -r * sin[60-ang];
		y = -r * sin[ang-45];
	} else if(ang >= 31){
		x = -r * sin[ang-30];
		y = r * sin[45-ang];
	} else if(ang >= 16){
		x = r * sin[30-ang];
		y = r * sin[ang-15];
	} else {
		x = r * sin[ang];
		y = -r * sin[15-ang];
	}
	DrawLine(X, Y, X + x, Y + y);
}
