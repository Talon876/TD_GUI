#include <SD.h>
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "Adafruit_FT6206.h"
#include "TD_GUI.h"

TD_Button::TD_Button(int16_t x0, int16_t y0, int16_t width0, int16_t height0,
	int16_t border0) {
	x = x0;
	y = y0;
	width = width0;
	height = height0;
	border = border0;
	primaryColor = CYAN;
	secondaryColor = DARKCYAN;
	clearColor = BLACK;
}

void TD_Button::init(Adafruit_ILI9341 &gfx) {
	draw(gfx);
}

void TD_Button::update(Adafruit_FT6206 &touch, Adafruit_ILI9341 &gfx) {
	if(touch.touched()) {
		TS_Point point = touch.getPoint();
		int tempX = point.x;
		point.x = map(point.y, 0, gfx.width(), gfx.width(), 0);
		point.y = map(tempX, gfx.height(), 0, gfx.height(), 0);
		isTouched = isPointInButton(point);
	} else {
		isTouched = false;
	}

	if(lastIsTouched != isTouched) {
		draw(gfx);
		if(isTouched && onButtonPressed != NULL) {
			(*onButtonPressed)();
		} else if(!isTouched && onButtonReleased != NULL) {
			(*onButtonReleased)();
		}
	}

	lastIsTouched = isTouched;
}

void TD_Button::draw(Adafruit_ILI9341 &gfx) {
	if(!invisible) {
		drawBox(gfx);
		fillBorder(gfx);
	}
}

bool TD_Button::isPointInButton(TS_Point &point) {
	return point.x >= x &&
		   point.y >= y &&
		   point.x <= x + width &&
		   point.y <= y + height;
}

void TD_Button::drawBox(Adafruit_ILI9341 &gfx) {
	gfx.drawRect(x, y, width, height, secondaryColor);
	gfx.drawRect(x + border, y + border,
		width - border*2, height - border*2, primaryColor);
}

void TD_Button::fillBorder(Adafruit_ILI9341 &gfx) {
	if (border > 1 && isTouched) {
		for(int16_t i = border - 1; i > 0; i--) {
			gfx.drawRect(x + i, y + i, width - i*2,
				height - i*2, primaryColor);
		}
	} else if(border > 1 && !isTouched) {
		for(int16_t i = border - 1; i > 0; i--) {
			gfx.drawRect(x + i, y + i, width - i*2,
				height - i*2, clearColor);
		}
	}
}

int TD_FileUtils::readFile(char *filename, String &result) {
	File textFile;
	if((textFile = SD.open(filename)) == NULL) {
		result = "error";
		return -1;
	}
	result = "";
	while(textFile.available()) {
		result.concat((char)textFile.read());
	}
	textFile.close();
	return 0;
}

uint16_t TD_FileUtils::read16(File &file) {
	uint16_t result;
	((uint8_t *)&result)[0] = file.read();
	((uint8_t *)&result)[1] = file.read();
	return result;
}

uint32_t TD_FileUtils::read32(File &file) {
	uint32_t result;
	((uint8_t *)&result)[0] = file.read();
	((uint8_t *)&result)[1] = file.read();
	((uint8_t *)&result)[2] = file.read();
	((uint8_t *)&result)[3] = file.read();
	return result;
}

int TD_BmpUtils::drawBitmap(Adafruit_ILI9341 &gfx, char *filename,
		uint8_t x, uint16_t y) {
	return TD_BmpUtils::drawBitmap(gfx, filename, x, y, 0);
}

int TD_BmpUtils::drawBitmap(Adafruit_ILI9341 &gfx, char *filename,
		uint8_t x, uint16_t y, int maskColor) {
	File bmpFile;
	int bmpWidth, bmpHeight;
	uint8_t bmpDepth;
	uint32_t bmpImageoffset;
	uint32_t rowSize;
	uint8_t sdbuffer[3*BITMAP_BUFFER];
	uint8_t buffidx = sizeof(sdbuffer);
	bool goodBmp = false;
	bool flip = true;
	int w, h, row, col;
	uint8_t r, g, b;
	uint32_t pos = 0;

	if((x >= gfx.width()) || (y >= gfx.height())) {
		return 1;
	}	
	if((bmpFile = SD.open(filename)) == NULL) {
		return 2;
	}
	if(TD_FileUtils::read16(bmpFile) == BITMAP_HEADER) {
		//read file size
		(void)TD_FileUtils::read32(bmpFile);
		//read creator
		(void)TD_FileUtils::read32(bmpFile);
		//read and store image offset
		bmpImageoffset = TD_FileUtils::read32(bmpFile);
		//read dib header
		(void)TD_FileUtils::read32(bmpFile);
		//read image dimensions
		bmpWidth = TD_FileUtils::read32(bmpFile);
		bmpHeight = TD_FileUtils::read32(bmpFile);

		if(TD_FileUtils::read16(bmpFile) == 1 /* Planes = 1 */) {
			bmpDepth = TD_FileUtils::read16(bmpFile);
			if((bmpDepth == 24) &&
				(TD_FileUtils::read32(bmpFile) == BITMAP_UNCOMPRESSED)) {
				goodBmp = true;
				rowSize = (bmpWidth * 3 + 3) & ~3;
				if(bmpHeight < 0) {
					bmpHeight = -bmpHeight;
					flip = false;
				}

				w = bmpWidth;
				h = bmpHeight;
				if((x+w-1) >= gfx.width()) {
					w = gfx.width() - x;
				}
				if((y+h-1) >= gfx.height()) {
					h = gfx.height() - y;
				}

				if(maskColor == BLACK) {
					gfx.setAddrWindow(x, y, x+w-1, y+h-1);
				}

				for(row = 0; row < h; row++) {
					if(flip) {
						pos = bmpImageoffset + (bmpHeight - 1 - row) * rowSize;
					} else {
						pos = bmpImageoffset + row * rowSize;
					}
					if(bmpFile.position() != pos) {
						bmpFile.seek(pos);
						buffidx = sizeof(sdbuffer);
					}

					for(col = 0; col < w; col++) {
						if (buffidx >= sizeof(sdbuffer)) { 
							bmpFile.read(sdbuffer, sizeof(sdbuffer));
							buffidx = 0;
						}
						b = sdbuffer[buffidx++];
						g = sdbuffer[buffidx++];
						r = sdbuffer[buffidx++];
						uint16_t color = gfx.color565(r,g,b);
						if(maskColor != BLACK && color != maskColor) {
							gfx.drawPixel(x+col, y+row, color);
						} else {
							gfx.pushColor(color);
						}
					}
				}
			}
		}
	} else {
		return 3;
	}

	bmpFile.close();
	return 0;
}

