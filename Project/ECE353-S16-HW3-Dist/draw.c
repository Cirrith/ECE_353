#include "draw.h"

void drawMenu(void) {
	int i, j;
	char buffer[10];
	
  lcd_clear();
	
	for(i = 0; i < 7; i++)
  {
    lcd_set_page(i);
    for(j=0; j<56; j++)
    {
      lcd_set_column(j+8);
      lcd_write_data(motionW[i*56 + j]);
    }
  }
	
	for(i = 0; i < eggHeightPages; i++)
  {
    lcd_set_page(i+1);
    for(j=0; j<eggWidthPixels; j++)
    {
      lcd_set_column(j+70);
      lcd_write_data(egg0Bitmap[i*eggWidthPixels + j]);
    }
  }
	
	sprintf(buffer, "PressStart");
	lcd_writeString(3, buffer);
}

void drawBulba(uint8_t frame) {
	const uint8_t *ptrFrame;
	uint8_t i, j, width, height;
	
	switch (frame)
	{
		case 0:
		{
			ptrFrame = bulba0Bitmap;
			width = bulba0WidthPixels;
			height = bulba0HeightPages;
			break;
		}
		case 1:
		{
			ptrFrame = bulba1Bitmap;
			width = bulba1WidthPixels;
			height = bulba1HeightPages;
			break;
		}
		case 2:
		{
			ptrFrame = bulba2Bitmap;
			width = bulba2WidthPixels;
			height = bulba2HeightPages;
			break;
		}
	}
	
	for(i = 0; i < height; i++)
  {
    lcd_set_page(i+1);
    for(j=0; j<width; j++)
    {
      lcd_set_column(j+30);
      lcd_write_data(ptrFrame[i*width + j]);
    }
  }
}

void drawChar(uint8_t frame) {
	const uint8_t *ptrFrame;
	uint8_t i, j, width, height;
	
	switch (frame)
	{
		case 0:
		{
			ptrFrame = char0Bitmap;
			width = char0WidthPixels;
			height = char0HeightPages;
			break;
		}
		case 1:
		{
			ptrFrame = char1Bitmap;
			width = char1WidthPixels;
			height = char1HeightPages;
			break;
		}
		case 2:
		{
			ptrFrame = char2Bitmap;
			width = char2WidthPixels;
			height = char2HeightPages;
			break;
		}
	}
	
	for(i = 0; i < height; i++)
  {
    lcd_set_page(i+1);
    for(j=0; j<width; j++)
    {
      lcd_set_column(j+30);
      lcd_write_data(ptrFrame[i*width + j]);
    }
  }
}

void drawSquirt(uint8_t frame) {
	const uint8_t *ptrFrame;
	uint8_t i, j, width, height;
	
	switch (frame)
	{
		case 0:
		{
			ptrFrame = squirt0Bitmap;
			width = squirt0WidthPixels;
			height = squirt0HeightPages;
			break;
		}
		case 1:
		{
			ptrFrame = squirt1Bitmap;
			width = squirt1WidthPixels;
			height = squirt1HeightPages;
			break;
		}
		case 2:
		{
			ptrFrame = squirt2Bitmap;
			width = squirt2WidthPixels;
			height = squirt2HeightPages;
			break;
		}
	}
	
	for(i = 0; i < height; i++)
  {
    lcd_set_page(i+1);
    for(j=0; j<width; j++)
    {
      lcd_set_column(j+30);
      lcd_write_data(ptrFrame[i*width + j]);
    }
  }
}

void drawPika(uint8_t frame) {
	const uint8_t *ptrFrame;
	uint8_t i, j, width, height;
	
	switch (frame)
	{
		case 0:
		{
			ptrFrame = pika0Bitmap;
			width = pika0WidthPixels;
			height = pika0HeightPages;
			break;
		}
		case 1:
		{
			ptrFrame = pika1Bitmap;
			width = pika1WidthPixels;
			height = pika1HeightPages;
			break;
		}
		case 2:
		{
			ptrFrame = pika2Bitmap;
			width = pika2WidthPixels;
			height = pika2HeightPages;
			break;
		}
	}
	
	for(i = 0; i < height; i++)
  {
    lcd_set_page(i+1);
    for(j=0; j<width; j++)
    {
      lcd_set_column(j+30);
      lcd_write_data(ptrFrame[i*width + j]);
    }
  }
}

void drawEgg(uint8_t cracks, uint8_t frame) {
	const uint8_t *ptrFrame;
	uint8_t i, j, width, height;
	
	switch (cracks)
	{
		case 0:
		{
			if (frame == 0) {
				ptrFrame = egg10Bitmap;
				width = egg10WidthPixels;
				height = egg10HeightPages;
			}
			else if (frame == 1 || frame == 3) {
				ptrFrame = egg11Bitmap;
				width = egg11WidthPixels;
				height = egg11HeightPages;
			}
			else {
				ptrFrame = egg12Bitmap;
				width = egg12WidthPixels;
				height = egg12HeightPages;
			}
			break;
		}
		case 1:
		{
			if (frame == 0) {
				ptrFrame = egg30Bitmap;
				width = egg30WidthPixels;
				height = egg30HeightPages;
			}
			else if (frame == 1 || frame == 3) {
				ptrFrame = egg31Bitmap;
				width = egg31WidthPixels;
				height = egg31HeightPages;
			}
			else {
				ptrFrame = egg32Bitmap;
				width = egg32WidthPixels;
				height = egg32HeightPages;
			}
			break;
		}
		case 2:
		{
			if (frame == 0) {
				ptrFrame = egg60Bitmap;
				width = egg60WidthPixels;
				height = egg60HeightPages;
			}
			else if (frame == 1 || frame == 3) {
				ptrFrame = egg61Bitmap;
				width = egg61WidthPixels;
				height = egg61HeightPages;
			}
			else {
				ptrFrame = egg62Bitmap;
				width = egg62WidthPixels;
				height = egg62HeightPages;
			}
			break;
		}
	}
	
	for(i = 0; i < height; i++)
  {
    lcd_set_page(i+2);
    for(j=0; j<width; j++)
    {
      lcd_set_column(j+35);
      lcd_write_data(ptrFrame[i*width + j]);
    }
  }
}

