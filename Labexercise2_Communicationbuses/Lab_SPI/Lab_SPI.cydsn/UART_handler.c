/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <UART_handler.h>

#define LED_ON '1'
#define LED_OFF '0'


void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case LED_ON :
        UART_1_PutString("LED ON \n");
        break;
        case LED_OFF :
        UART_1_PutString("LED OFF \n");
        break;
        default :
        {}
        break;
    }
}
/* [] END OF FILE */
