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
#include "ADC.h"
#include "project.h"
#include <stdio.h>

void ADC_init()
{
    ADC_SAR_1_Start();
    ADC_SAR_1_StartConvert();
}

void ADC_Measure()
{
      char uartBuffer[256];
      char uartBuffer2[256];  
    
    if (ADC_SAR_1_IsEndConversion(ADC_SAR_1_WAIT_FOR_RESULT))
        {
            uint16_t result = ADC_SAR_1_GetResult16();
            snprintf(uartBuffer, sizeof(uartBuffer), "ADC result: 0x%x \r\n", result);
            UART_1_PutString(uartBuffer);
            
            float kg = 0.0005 * result - 0.0553;
            snprintf(uartBuffer2, sizeof(uartBuffer2), "Kg result: %f \r\n", kg);
            UART_1_PutString(uartBuffer2); 
            
        }   
}


/* [] END OF FILE */
