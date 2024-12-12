/******************************************************************************************************************************************
  Created by Roytech Prototypes (Shawon Roy).
  Till now tested on SSD1306 OLED display.
  Date: 12/12/24
  Version: v1.1
  GitHub: https://github.com/superguine 
******************************************************************************************************************************************/

#ifndef ELLIPSE_FUNCTIONS_H
#define ELLIPSE_FUNCTIONS_H

#include <stdint.h>        // For standard integer types
#include "Adafruit_SSD1306.h"  // Included the actual display library header file 

extern Adafruit_SSD1306 display; // Declare the display object (ensure it's initialized elsewhere)


// Function to fill an ellipse with a given color
inline void fillEllipse(int x0, int y0, int w, int h, uint16_t color) {
    int rx = w / 2, ry = h / 2;
    
    // Loop through every pixel in the bounding box and check if it's inside the ellipse
    for (int x = -rx; x <= rx; x++) {
        for (int y = -ry; y <= ry; y++) {
            // Ellipse equation: (x^2 / rx^2) + (y^2 / ry^2) <= 1
            if ((x * x * ry * ry + y * y * rx * rx) <= (rx * rx * ry * ry)) {
                display.drawPixel(x0 + x, y0 + y, color);  // Draw pixel
            }
        }
    }
    display.display();  // Update the display with the filled ellipse
}


// Function to draw the outline of an ellipse
inline void drawEllipse(int x0, int y0, int w, int h, uint16_t color) {
    int rx = w / 2, ry = h / 2;
    int mxa=x0-rx, mxb=x0+rx;
    int rxSq = rx * rx, rySq = ry * ry;

    int x = 0, y = ry;
    int px = 0, py = 2 * rxSq * y;

    // Region 1 (top half of the ellipse)
    int p1 = rySq - (rxSq * ry) + (0.25 * rxSq);
    while (px < py) {
        // Draw pixels in the four quadrants of the ellipse
        display.drawPixel(x0 + x, y0 + y, color);
        display.drawPixel(x0 - x, y0 + y, color);
        display.drawPixel(x0 + x, y0 - y, color);
        display.drawPixel(x0 - x, y0 - y, color);

        x++;  // Move along the x-axis
        px += 2 * rySq;
        if (p1 < 0) {
            p1 += rySq + px;
        } else {
            y--;  // Move along the y-axis
            py -= 2 * rxSq;
            p1 += rySq + px - py;
        }
    }
	display.drawPixel(mxa,y0,color); display.drawPixel(mxb,y0,color); // fill the gap between 2 regions
	// Region 2 (bottom half of the ellipse)
    int p2 = (rySq * (x + 0.5) * (x + 0.5)) + (rxSq * (y - 1) * (y - 1)) - (rxSq * rySq);
    while (y > 0) {
        // Draw pixels in the four quadrants of the ellipse
        display.drawPixel(x0 + x, y0 + y, color);
        display.drawPixel(x0 - x, y0 + y, color);
        display.drawPixel(x0 + x, y0 - y, color);
        display.drawPixel(x0 - x, y0 - y, color);

        y--;  // Move along the y-axis
        py -= 2 * rxSq;
        if (p2 > 0) {
            p2 += rxSq - py;
        } else {
            x++;  // Move along the x-axis
            px += 2 * rySq;
            p2 += rxSq - py + px;
        }
    }

    display.display();  // Update the display with the drawn ellipse
}

#endif // ELLIPSE_FUNCTIONS_H
