/*
Copyright (c) 2019 Brian Lough . All right reserved.

Arduino Printing Animation

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef PrintingAnimation_h
#define PrintingAnimation_h

#include <Arduino.h>
#include "Adafruit_GFX.h"

class PrintingAnimation
{
    public:
        PrintingAnimation (Adafruit_GFX  &display);
        Adafruit_GFX  *display;
        boolean animate();
        void setBounds(int xStart, int yStart, int xFinish, int yFinish);
        void resetState(boolean right = true);

        // Settings
        int startX = 0;
        int startY = 31;
        int finishX = 63;
        int finishY = 0;
        int increments = 1;

        // State
        int currentX = 0;
        int currentY = 31;
        boolean directionRight = true;

        // Draw Style
        boolean drawFilament = false;
        boolean drawRails = true;
        boolean drawHead = true;

        uint16_t backgroundColour = 0x0000; //Black
        uint16_t railColour = 0x07FF; //Cyan
        uint16_t headColour = 0xF81F; //Magenta
        uint16_t filamentColour = 0xF800; //Red

    };

#endif