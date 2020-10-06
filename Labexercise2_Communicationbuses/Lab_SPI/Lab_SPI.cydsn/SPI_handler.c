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

#include <SPI_handler.h>

void SPIM_handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 1 :
        {
            UART_1_PutString("Button state Idle \r\n");
        }
        break;
        case 0 :
        {
            UART_1_PutString("Button state Pressed \r\n");
        }

        break;
        default :
        {}
        break;
    }
    
}

/* [] END OF FILE */
