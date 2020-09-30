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
    while(!(SPIM_1_ReadRxStatus() & SPIM_1_STS_SPI_DONE));
    
    UART_1_PutChar(SPIM_1_ReadRxData());
    
}

void writeLEDSPI()
{
    SPIM_1_WriteTxData(0x99u); /* initiates data transmission */
    SPIM_1_WriteTxData(0xAAu);
    SPIM_1_WriteTxData(0xBBu);
    SPIM_1_WriteTxData(0xCCu);
    SPIM_1_WriteTxData(0xDDu);
    SPIM_1_WriteTxData(0xEEu);
    SPIM_1_WriteTxData(0xFFu);
    SPIM_1_WriteTxData(0x12u);
}


/* [] END OF FILE */
