#include "LED.hpp"

LED::LED()
{
	// Constructor
	// ****************************
	//         PORT CONFIG
	// ****************************
	// - P2.(0,1,2) is connected to RGB led
	P2->DIR |= BIT2;
}

uint8_t LED::run(void)
{
    //#########################
    // Toggle LED2
    //#########################
	P2->OUT ^= BIT2;

    return(NO_ERR);
}
