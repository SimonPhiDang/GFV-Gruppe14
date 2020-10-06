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

// Inkludering af SPI_slave handler
#include <SPI_Slave.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(SPIS_rx_handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isruart_StartEx(ISR_UART_rx_handler); // Start af UART interrupt
    isrspis_StartEx(SPIS_rx_handler); // Start af SPIS rx interrupt 
    UART_1_Start(); // UART Start
    SPIS_1_Start(); // SPIS Start
    SPIS_1_ClearRxBuffer(); // Clear Rx buffer
    
    for(;;)
    {}
}

CY_ISR(ISR_UART_rx_handler) // Debuggig af UART
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); // Aflæs bytes fra UART Master
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();  // Aflæs bytes fra UART
        UART_1_WriteTxData(byteReceived); // echo back
        bytesToRead--;
    }
}
CY_ISR(SPIS_rx_handler) // Modtag bytes fra SPIM og håndtere dem
{
    uint8_t bytesToRead = SPIS_1_GetRxBufferSize();  // Få buffer size
    while (bytesToRead > 0) // Aflæs hele buffer
    {
        uint8_t byteReceived = SPIS_1_ReadRxData(); // Aflæs Rx data    
        SPIS_handleByteReceived(byteReceived);  // Håndtering af rx data hvis det 'b' eller andet
        bytesToRead--;
    }
}

/* [] END OF FILE */
