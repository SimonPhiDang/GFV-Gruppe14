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

#include <UART_handler.h>
#include <SPI_handler.h>


CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(SPI_rx_handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isruart_StartEx(ISR_UART_rx_handler);
    isrspim_StartEx(SPI_rx_handler);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    SPIM_1_Start();
    
    SPIM_1_ClearRxBuffer(); // Clear Rx buffer en gang i starten.
    
    for(;;)
    {}
}

CY_ISR(ISR_UART_rx_handler) // UART fra master til SPI SLAVE
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); // Aflæs bytes fra UART Master
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();  // Aflæs bytes fra UART
        UART_1_WriteTxData(byteReceived); // echo back
        handleByteReceived(byteReceived);   // LED ON eller OFF på UART     
        
        SPIM_1_WriteTxData(byteReceived);  //Skriv bytes til SPI Slave
            
        bytesToRead--;
    }
}
CY_ISR(SPI_rx_handler) // SPIM modtager fra SPIS status på switch
{
    uint8_t bytesToRead = SPIM_1_GetRxBufferSize();
    while(bytesToRead > 0)
    {
        uint8_t swStatus = SPIM_1_ReadRxData();  // Få byte fra FIFO rx buffer
        SPIM_handleByteReceived(swStatus); 
        bytesToRead--;
    }

}

/* [] END OF FILE */
