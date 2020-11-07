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
#include <stdio.h>
#include "project.h"
#include "PIDControl.h"
#include "I2C_handler.h"

CY_ISR_PROTO(ISR_UART_rx_handler);
static char outputBuffer[256];
static float setPoint = 50; // degrees celcius

#define SAMPLES_PER_SECOND 3
static uint16_t sampleWaitTimeInMilliseconds = 1000 / SAMPLES_PER_SECOND;


int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */
    PWM_1_Start();
    I2C_1_Start();
    UART_1_Start();
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    /* Place your initialization/startup code here (e.g. MyInst_Start()) */

    float Kp = 0.5f;
    float Ki = 1.0f/30.0f;
    float Kd = 0.0f;
    float integralMax = 3000;
    float integralMin = -3000;
    float temp = 0;
    float controlSignal = 0;
    float dt = ((float)sampleWaitTimeInMilliseconds) / 1000; // dt is measured in seconds
    PIDControl_init(Kp, Ki, Kd, integralMax, integralMin, dt);
    PIDControl_changeSetPoint(setPoint);
    
    UART_1_PutString("Temperature control application started\r\n");
    for(;;)
    {
        temp = (float)readTemp();
        /* Place your application code here. */
        float error = setPoint - temp;
        float proportionalPart = 0;
        float integralPart = 0;
        float derivativePart = 0;
        
        controlSignal = PIDControl_doStep(temp, &proportionalPart, &integralPart, &derivativePart);            
        snprintf(outputBuffer, sizeof(outputBuffer), "%2.f,%2.f,%2.f, %2.f, %2.f, %2.f, %2.f, %2.f, %2.f, %2.f \r\n\n", 
                                                     setPoint, temp, error, controlSignal, Kp, Ki, Kd, 
                                                   proportionalPart, integralPart, derivativePart);
        UART_1_PutString(outputBuffer);
        
        
        PWM_1_WriteCompare(controlSignal);
        
        CyDelay(sampleWaitTimeInMilliseconds);
    }
}

void handleByteReceived(uint8_t ByteReceived)
{
    switch(ByteReceived)
    {
        case '3':
        {
            UART_1_PutString("30 set\n");
           setPoint = 30;
           PIDControl_changeSetPoint(30);
        }
        break;
        case '5':
        {
            UART_1_PutString("50 set\n");
            setPoint = 50;
            PIDControl_changeSetPoint(50);
            
        }
        break;
        default:
        {}
        break;
    }
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while(bytesToRead > 0)
    {
        uint8_t bytesReceived = UART_1_ReadRxData();
        
        handleByteReceived(bytesReceived);
        
        bytesToRead--;
    }
}


/* [] END OF FILE */
