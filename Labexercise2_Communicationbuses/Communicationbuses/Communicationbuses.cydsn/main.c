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
#include "project.h"

/* Inkludering af I2C håndtering headerfil  */
#include <I2C_handler.h>



int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    UART_1_Start();
    I2C_1_Start();
    
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    
    UART_1_PutString("Test");
    readTemp();
    for(;;)
    {
        readTemp();       
        CyDelay(1000);
        
    }
}

/* [] END OF FILE */
