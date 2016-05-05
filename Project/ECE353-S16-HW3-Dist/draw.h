#include <stdint.h>

#include "lcd.h"
#include "img.h"

void drawMenu(void);
void drawBattle(uint8_t owned, uint8_t otherowned);
void drawBulba(uint8_t frame);
void drawChar(uint8_t frame);
void drawSquirt(uint8_t frame);
void drawPika(uint8_t frame);

void drawEgg(uint8_t cracks, uint8_t frame);
