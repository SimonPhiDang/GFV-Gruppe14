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

void pushButtonState()
{
    /* We need to know the moment when SPI communication is complete
    * to display received data. SPIS_SPI_DONE status should be polled. 
    */
//    while(!(SPIM_1_ReadRxStatus() & SPIM_1_STS_SPI_DONE));
//    
//    UART_1_PutChar(SPIM_1_ReadRxData());
//    
//    SPIM_1_ClearRxBuffer();
}


void SPIM_handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 'b' :
        {
            UART_1_PutString("Button state pressed");
        }
        break;
        case 'i' :
        {
            UART_1_PutString("Button state idle");
        }

        break;
        default :
        {
            // nothing
        }
        break;
    }
    
}

/* [] END OF FILE */
