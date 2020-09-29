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


void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case '1' :
        {
           UART_1_PutString("LED On\r\n");
        }
        break;
        case '2' :
        {
            UART_1_PutString("LED Off \r\n");
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
