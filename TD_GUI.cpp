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

