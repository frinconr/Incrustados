#include "LED.hpp"

LED::LED(const uint16_t LEDMask)
{
	// Constructor
	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P2.(0,1,2) is connected to RGB led
	P2->DIR |= (LEDMask % 8);
	P2->OUT &= ~(LEDMask % 8);
	mLEDMask = (LEDMask % 8);
}

uint8_t LED::run(void)
{
    //#########################
    // Toggle LED2
    //#########################
	P2->OUT ^= mLEDMask;

    return(NO_ERR);
}
