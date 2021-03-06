#include "Adafruit_ILI9341.h"
#ifndef _TD_GUI_
#define _TD_GUI_

#define BLACK       0x0000      /*   0,   0,   0 */
#define NAVY        0x000F      /*   0,   0, 128 */
#define DARKGREEN   0x03E0      /*   0, 128,   0 */
#define DARKCYAN    0x03EF      /*   0, 128, 128 */
#define MAROON      0x7800      /* 128,   0,   0 */
#define PURPLE      0x780F      /* 128,   0, 128 */
#define OLIVE       0x7BE0      /* 128, 128,   0 */
#define LIGHTGREY   0xC618      /* 192, 192, 192 */
#define DARKGREY    0x7BEF      /* 128, 128, 128 */
#define BLUE        0x001F      /*   0,   0, 255 */
#define GREEN       0x07E0      /*   0, 255,   0 */
#define CYAN        0x07FF      /*   0, 255, 255 */
#define RED         0xF800      /* 255,   0,   0 */
#define MAGENTA     0xF81F      /* 255,   0, 255 */
#define YELLOW      0xFFE0      /* 255, 255,   0 */
#define WHITE       0xFFFF      /* 255, 255, 255 */
#define ORANGE      0xFD20      /* 255, 165,   0 */
#define GREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define TRANSPARENT 0xF81F

#define ROTATE_NONE 0
#define ROTATE_90 1
#define ROTATE_180 2
#define ROTATE_270 3

#define BITMAP_HEADER 0x4D42
#define BITMAP_BUFFER 20
#define BITMAP_UNCOMPRESSED 0

class TD_Button {
public:
	TD_Button(int16_t x, int16_t y, int16_t width, int16_t height, int16_t border);

	void init(Adafruit_ILI9341 &gfx);
	void update(Adafruit_FT6206 &touch, Adafruit_ILI9341 &gfx);

	void (*onButtonPressed)();
	void (*onButtonReleased)();
	
	int16_t x, y, width, height, border;
	int16_t primaryColor, secondaryColor, clearColor;
	bool invisible;
private:
	bool isTouched, lastIsTouched;
	void draw(Adafruit_ILI9341 &gfx);
	void drawBox(Adafruit_ILI9341 &gfx);
	void fillBorder(Adafruit_ILI9341 &gfx);
	bool isPointInButton(TS_Point &point);
};

class TD_FileUtils {
public:
	static int readFile(char *filename, String &result);
	static uint16_t read16(File &file);
	static uint32_t read32(File &file);
};

class TD_BmpUtils {
public:
	static int drawBitmap(Adafruit_ILI9341 &gfx, char *filename,
		uint8_t x, uint16_t y);
	static int drawBitmap(Adafruit_ILI9341 &gfx, char *filename,
		uint8_t x, uint16_t y, int maskColor);
};


#endif
