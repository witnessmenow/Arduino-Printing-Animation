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

#include "PrintingAnimation.h"

PrintingAnimation::PrintingAnimation(Adafruit_GFX &display) {
    this->display = &display;
}

boolean PrintingAnimation::animate()
{
    int xWidth = this->finishX - this->startX + 1;
    
    // clear above the print head
    if (this->currentY > this->finishY) {
        int yHeight = this->currentY - this->finishY;
        this->display->fillRect(this->startX, this->finishY, xWidth, yHeight, this->backgroundColour);
    }
    
    // draw rails
    if(this->drawRails) {
        this->display->drawFastHLine(this->startX, this->currentY - 2, xWidth, this->railColour);
        this->display->drawFastHLine(this->startX, this->currentY - 4, xWidth, this->railColour);
    }

    // draw print head
    if(this->drawHead) {
        this->display->fillRect(this->currentX - 1, this->currentY - 4, 3, 3, this->headColour);
        this->display->drawPixel(this->currentX, this->currentY - 1, this->headColour);
    }
    
    // draw filament
    if(this->drawFilament) {
        int filamentDistance = (this->currentY - 4) - this->finishY;
        if (filamentDistance > 0) {
            this->display->drawFastVLine(this->currentX, this->finishY, filamentDistance, this->filamentColour);
        }
    }
    
    // Clear the same line that we are currently on
    // but haven't reached yet.
    if(this->directionRight) {
        if(this->currentX < this->finishX) {
            int lineWidth = this->finishX - this->currentX - 1;
            this->display->drawFastHLine(this->currentX, this->currentY, lineWidth, this->backgroundColour);
        }
    } else {
        if(this->currentX > this->startX){
            int lineWidth = this->currentX - this->startX - 1;
            this->display->drawFastHLine(this->startX, this->currentY, lineWidth, this->backgroundColour);
        }
    }
    
    // Handle Moving the head for the next time.
    if(this->directionRight) {
        this->currentX += this->increments;
        if (this->currentX > this->finishX) {
            this->currentY --;
            this->directionRight = false;
        }
    } else {
        this->currentX -= this->increments;
        if (this->currentX < this->startX) {
            this->currentY --;
            this->directionRight = true;
        }
    }

    // check if the Y is above out bounds
    // we are finished if it is.
    return this->currentY < this->finishY;
}

void PrintingAnimation::setBounds(int xStart, int yStart, int xFinish, int yFinish){
    this->startX = xStart;
    this->startY = yStart;
    this->finishX = xFinish;
    this->finishY = yFinish;
}

void PrintingAnimation::resetState(boolean right){
    this->currentX = this->startX;
    this->currentY = this->startY;
    this->directionRight = right;
}