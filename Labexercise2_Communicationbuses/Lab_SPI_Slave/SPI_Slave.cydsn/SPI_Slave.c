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

// Definering af LED modes
#define LED_ON 1
#define LED_OFF 0

void buttonState() // Funktion til aflæsning af bitsekvens fra switch
{
     if(Pin_2_Read() == 00000001) // Hvis idle
     {
         SPIS_1_WriteTxData(1); // Skriv 1 til SPIM
     }
     else if(Pin_2_Read() == 00000000) // Hvis 
     {
          SPIS_1_WriteTxData(0);
     }            
}


void SPIS_handleByteReceived(uint8_t byteReceived)
{
    // Variabler
    int status;
    int tx_buf;
 
    if(byteReceived=='b')
    {
        switch(status)
        {
            case LED_ON:
            status=LED_OFF; // Skift toggle
            Pin_1_Write(~Pin_1_Read()); // Toggle LED
            UART_1_WriteTxData(byteReceived); // Skriv UART (DEBUGGING)
            buttonState(); // Aflæs button state og skriv det til SPIM
            break;
            case LED_OFF: // Gentagelse af case med toggle skift
                status=LED_ON;
                Pin_1_Write(~Pin_1_Read());
                UART_1_WriteTxData(byteReceived);
                buttonState();
            break;
        }
    }
    else
    {
        UART_1_WriteTxData(byteReceived); // Skriv UART da det ikke er specifik til at tænde LED
    }
    
    tx_buf = status;  
}
/* [] END OF FILE */
