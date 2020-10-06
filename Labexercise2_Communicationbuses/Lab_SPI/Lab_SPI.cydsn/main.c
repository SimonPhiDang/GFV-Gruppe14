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
#include <project.h>
#include <stdio.h>
#include <stdint.h>

// Inkludering af SPI_handler
#include <SPI_handler.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(SPI_rx_handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isruart_StartEx(ISR_UART_rx_handler); // Start af interrupt
    isrspim_StartEx(SPI_rx_handler);  // Start af interrupt
    UART_1_Start(); // Start af UART
    SPIM_1_Start(); // Start af SPIM
    
    SPIM_1_ClearRxBuffer(); // Clear Rx buffer 
    
    for(;;)
    {}
}

CY_ISR(ISR_UART_rx_handler) // UART Master input modtages og sendes til slave
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); // Få størrelse af buffer
    while (bytesToRead > 0) // Aflæs hele buffer
    {
        uint8_t byteReceived = UART_1_ReadRxData();  // Aflæs bytes fra UART
        UART_1_WriteTxData(byteReceived); // echo back på UART Master
        SPIM_1_WriteTxData(byteReceived);  //Skriv bytes til SPI Slave
            
        bytesToRead--; // træk 1 byte væk fra bytes to read
    }
}
CY_ISR(SPI_rx_handler) // SPIM aflæser data fra SPIS og udskriver på UART
{
    uint8_t bytesToRead = SPIM_1_GetRxBufferSize(); // Får størrelse af buffer
    while(bytesToRead > 0) // Aflæs hele buffer
    {
        uint8_t swStatus = SPIM_1_ReadRxData();  // Får 1 eller 0 ud fra status af switch på SPI slave switch
        SPIM_handleByteReceived(swStatus);  // Håndtering af 1 eller 0
        bytesToRead--;
    }

}

/* [] END OF FILE */
