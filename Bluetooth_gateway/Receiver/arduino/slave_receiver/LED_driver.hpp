
#ifndef _LED_DISPLAY_HPP
#define _LED_DISPLAY_HPP

#include <Arduino.h>
#include <SPI.h>
#include <DMD2.h>
#include <fonts/Arial14.h>
#include <string.h>


enum TRANSITION : uint8_t
{
  NO = 0,
  NORMAL,
  FROM_0_TO_1
};



SoftDMD dmd(1,1);

class Box
{
    public:
        void Execute()
        {
            dmd.drawString(xPosition, this->yPosition, String(this->currentValue));
        }

        TRANSITION Update(uint8_t value)
        {
            this->currentValue = value;
            TRANSITION out;
            if (previousValue != this->currentValue)
            {
            if(previousValue == 0 && currentValue == 1)
            {
                out = TRANSITION::FROM_0_TO_1;
            }
            else
            {
                out = TRANSITION::NORMAL;
            }
            this->previousValue = this->currentValue;
            }
            else
            {
                out = TRANSITION::NO;
            }
            this->Execute();
            return out;
        }

        TRANSITION Update(String symbol)
        {
            dmd.drawString(this->xPosition, this->yPosition, symbol);
            return TRANSITION::NO;
        }

        static const uint8_t STEP = ARIAL_14_WIDTH - 2;
        static const uint8_t Y_OFFSET = 2;
        uint8_t xPosition;
        uint8_t yPosition;
        uint32_t multiplier;
        uint8_t currentValue = 1;
        uint8_t previousValue = 1;

        Box(uint8_t xPos, uint8_t yPos, uint32_t multi) :
            multiplier(multiplier)
        {
            this->xPosition = xPos;
            this->yPosition = yPos;
            this->multiplier = multi;
        }


};

class LED
{
    public:
        LED() :   
            hundred(Box(0, Box::Y_OFFSET, 1000)),
            decimal(Box(Box::STEP, Box::Y_OFFSET, 100)),
            digit(Box(2*Box::STEP, Box::Y_OFFSET, 10)),
            milis(Box(3*Box::STEP + 2, Box::Y_OFFSET, 1)),
            dot(Box(3*Box::STEP -1, Box::Y_OFFSET, 1))
        {

        }
        
        
        void SetValue(int i)
        {
            uint8_t result = static_cast<uint8_t>(TRANSITION::NO);
            result |= static_cast<uint8_t>(hundred.Update(i/hundred.multiplier));
            result |= static_cast<uint8_t>(decimal.Update(i/decimal.multiplier % 10));
            result |= static_cast<uint8_t>(digit.Update(i/digit.multiplier % 10));
            result |= static_cast<uint8_t>(milis.Update(i/milis.multiplier % 10));
            //result |= static_cast<uint8_t>(milis.Update(i/milis.multiplier % 10));
            result |= static_cast<uint8_t>(dot.Update("'"));

            if(result & FROM_0_TO_1)
            {
                dmd.clearScreen();
                hundred.Execute();
                decimal.Execute();
                digit.Execute();
                milis.Execute();
                dot.Update("'");
            }
        }

        void SetValue(float data)
        {
            float precision = 10.0;
            int converted = static_cast<int>(data * precision);
            SetValue(converted);
        }
        
        void Init()
        {
            dmd.setBrightness(255); // Set brightness 0 - 255 
            dmd.selectFont(Arial14); // Font used
            dmd.begin();     // Start DMD 
        }

    private:
        Box hundred;
        Box decimal;
        Box digit;
        Box milis;
        Box dot;
};

#endif
