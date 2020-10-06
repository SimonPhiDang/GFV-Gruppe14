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
    UART_1_Start();  // Start af UART
    I2C_1_Start();  // Start af I2C

    UART_1_PutString("Start program \n");
    
    readTemp();
    
    for(;;)
    {   }
}

/* [] END OF FILE */
