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
#include <SPI_Slave.h>

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(SPIS_rx_handler);

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    isruart_StartEx(ISR_UART_rx_handler);
    isrspis_StartEx(SPIS_rx_handler);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    UART_1_Start();
    SPIS_1_Start();
    SPIS_1_ClearRxBuffer(); // Clear Rx buffer en gang i starten.
    
   
    
    for(;;)
    {
        
        /* Place your application code here. */
        
    }
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize(); // Aflæs bytes fra UART Master
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();  // Aflæs bytes fra UART
        UART_1_WriteTxData(byteReceived); // echo back
//        SPIS_1_WriteTxData(byteReceived);  //Skriv bytes til SPI master
        
//        handleByteReceived(byteReceived);
        
        bytesToRead--;
    }
}
CY_ISR(SPIS_rx_handler)
{
    uint8_t bytesToRead = SPIS_1_GetRxBufferSize();  // SPI Master rx interrupt få size
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = SPIS_1_ReadRxData(); // Aflæs Rx data
 //       UART_1_WriteTxData(byteReceived);  // Skriv bytes til UART
 //      UART_1_PutString("TEST");
        
        SPIS_handleByteReceived(byteReceived); 
        
       // bytesToRead = SPIS_1_GetRxBufferSize();
        bytesToRead--;
    }
}

/* [] END OF FILE */
