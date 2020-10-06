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
#include <SPI_Slave.h>
#include <project.h>
#define LED_ON 1
#define LED_OFF 0
int status;
int tx_buf;

void buttonState()
{
     if(Pin_2_Read() == 00000001)
     {
         SPIS_1_WriteTxData(1);
     }
     else if(Pin_2_Read() == 00000000)
     {
          SPIS_1_WriteTxData(0);
     }            
}


void SPIS_handleByteReceived(uint8_t byteReceived)
{
    if(byteReceived=='b')
    {
        switch(status)
        {
            case LED_ON:
                status=LED_OFF;
                Pin_1_Write(~Pin_1_Read()); // Toggle LED
                UART_1_WriteTxData(byteReceived); // Skriv UART hvad fået
                // Skrive til SPIM fra SPIS
                buttonState();
               
            break;
            
            case LED_OFF:
                status=LED_ON;
                Pin_1_Write(~Pin_1_Read());
                UART_1_WriteTxData(byteReceived);
                buttonState();   
              
            break;
        }
    }
    else
    {
    UART_1_WriteTxData(byteReceived);
    }
    
    tx_buf = status;
}
/* [] END OF FILE */
