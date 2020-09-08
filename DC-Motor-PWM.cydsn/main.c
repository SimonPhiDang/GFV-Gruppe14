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

// Prototyper
CY_ISR_PROTO(ISR_UART_rx_handler);
void handleByteReceived(uint8_t byteReceived);
void decreaseSpeed(void);
void increaseSpeed(void);
void driveForwards(void);
void driveBackwards(void);
void stop(void);

uint8_t mode = 0;
uint8 currentSpeed = 0;
uint8 IN1,IN2 = 0;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    UART_1_Start();
    PWM_1_Start();
    
    UART_1_PutString("DC-Motor-PWM application started\r\n");
    UART_1_PutString("0: Stop\r\n");
    UART_1_PutString("1: Drive forwards\r\n");
    UART_1_PutString("2: Drive backwards\r\n");
    UART_1_PutString("q: Decrease speed\r\n");
    UART_1_PutString("w: Increase speed\r\n");

    for(;;)
    {
        /* Place your application code here. */
        switch(mode)
        {
            case 1:
            //decrease speed
            if(currentSpeed != 0 && currentSpeed < 255)
            {
                currentSpeed = currentSpeed - 20;
                PWM_1_WriteCompare(currentSpeed);
            }
            break;
            case 2:
            //Increase speed
            if(currentSpeed != 0 && currentSpeed < 255)
            {
                currentSpeed = currentSpeed + 20;
                PWM_1_WriteCompare(currentSpeed);
            }
            break;
            case 3:
            //Drive Forwards
            currentSpeed = 200;
            PWM_1_WriteCompare(currentSpeed);
            
            //Styrelsen af H-Bro
            Pin_IN1_Write(IN1);
            Pin_IN2_Write(IN2);
            
            break;
            case 4:
            //Drive backwards
            currentSpeed = 200;
            PWM_1_WriteCompare(currentSpeed);
            
            //Styrelsen af H-Bro
            Pin_IN1_Write(IN1);
            Pin_IN2_Write(IN2);
            
            break;
            case 5:
            //Stop
            currentSpeed = 0;   // Stop PWM
            PWM_1_WriteCompare(currentSpeed);
            
            break;
        }
    }
}

CY_ISR(ISR_UART_rx_handler)
{
    uint8_t bytesToRead = UART_1_GetRxBufferSize();
    while (bytesToRead > 0)
    {
        uint8_t byteReceived = UART_1_ReadRxData();
        UART_1_WriteTxData(byteReceived); // echo back
        
        handleByteReceived(byteReceived);
        
        bytesToRead--;
    }
}

void handleByteReceived(uint8_t byteReceived)
{
    switch(byteReceived)
    {
        case 'q' :
        {
            decreaseSpeed();
        }
        break;
        case 'w' :
        {
            increaseSpeed();
        }
        break;
        case '1' :
        {
            driveForwards();
        }
        break;
        case '2' :
        {
            driveBackwards();
        }
        break;
        case '0' :
        {
            stop();
            
        }
        break;
        default :
        {
            // nothing
        }
        break;
    }
}

void decreaseSpeed()
{
    UART_1_PutString("Decreasing speed\r\n");
    mode = 1;
}

void increaseSpeed()
{
    UART_1_PutString("Increasing speed\r\n");
    mode = 2;
}

void driveForwards()
{
    UART_1_PutString("Set direction: forwards\r\n");
    mode = 3;
    IN1 = 1;
    IN2 = 0;
}

void driveBackwards()
{
    UART_1_PutString("Set direction: backwards\r\n");
    mode = 4;
    IN2 = 1;
    IN1 = 0;
}

void stop()
{
    UART_1_PutString("Stop\r\n");
    mode = 5;
}

/* [] END OF FILE */
