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
#include <I2C_handler.h>

#include <project.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define LM75address 0x48
#define LM75address2 0x49

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

int8 readTemp()
{
    //Variabler 
    uint8 dataRecv[2] = {0,0};
    uint8 dataRecv2[2] = {0,0};
    uint8 errorStatus[2] = {0,0};
    uint8 errorStatus2[2] ={0,0};
    float temp;
    float temp2;
    
    while(1)
    {
    //1001 A2 A1 A0  Alle A'er er modsat ON = 0 
    errorStatus[0] = I2C_1_MasterSendStart(LM75address,I2C_1_READ_XFER_MODE);

    if (errorStatus[0] == I2C_1_MSTR_NO_ERROR)
        {
            // Læser første byte i register MSB første 8 bit
                dataRecv[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  
            // Læser anden byte i register, sidste byte er NAKed  // LSB sidste 8 bit
                dataRecv[1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
            //    MasterSendStop
                I2C_1_MasterSendStop();         

            // Data skal aflæses som 2 komplements int så fortegn er korrekt
                temp = dataRecv[0];
                if(CHECK_BIT(dataRecv[1],7) == 1)
                {
                    temp += 0.5;
                }
                if(CHECK_BIT(dataRecv[0],7) == 1)
                {
                    temp -=128;
                }
            //    Send temperature to UART
                writeTempToUART(temp);
                CyDelay(1000);
        }
    else if(errorStatus[0] == I2C_1_MSTR_BUS_BUSY)
        {
    //    report error on UART
          UART_1_PutString("Error I2C_1_MSTR_BUS_BUSY\n");
          CyDelay(1000);
    //    MasterSendStop
          I2C_1_MasterSendStop();
        }
    else if(errorStatus[0] == I2C_1_MSTR_NOT_READY)
        {
           UART_1_PutString("Error I2C_1_MSTR_NOT_READY\n"); 
           CyDelay(1000);
           I2C_1_MasterSendStop();
        }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_LB_NAK)
        {
            UART_1_PutString("Error I2C_1_MSTR_ERR_LB_NAK\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_ARB_LOST)
        {
            UART_1_PutString("Error I2C_1_MSTR_ERR_ARB_LOST\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }
    else if (errorStatus[0] == I2C_1_MSTR_ERR_ABORT_START_GEN)
        {
            UART_1_PutString("Error I2C_1_MSTR_ERR_ABORT_START_GE\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }


    errorStatus2[0] = I2C_1_MasterSendStart(LM75address2,I2C_1_READ_XFER_MODE);    
    
    if (errorStatus2[0] == I2C_1_MSTR_NO_ERROR)
        {
            // Læser første byte i register MSB første 8 bit
                dataRecv2[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  
            // Læser anden byte i register, sidste byte er NAKed  // LSB sidste 8 bit
                dataRecv2[1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
            //    MasterSendStop
                I2C_1_MasterSendStop();         

            // Data skal aflæses som 2 komplements int så fortegn er korrekt
                temp2 = dataRecv2[0];
                if(CHECK_BIT(dataRecv[1],7) == 1)
                {
                    temp2 += 0.5;
                }
                if(CHECK_BIT(dataRecv[0],7) == 1)
                {
                    temp2 -=128;
                }
            //    Send temperature to UART
                writeTempToUART2(temp2);
                CyDelay(1000);
        }
    else if(errorStatus2[0] == I2C_1_MSTR_BUS_BUSY)
        {
    //    report error on UART
          UART_1_PutString("Error2 I2C_1_MSTR_BUS_BUSY\n");
          CyDelay(1000);
    //    MasterSendStop
          I2C_1_MasterSendStop();
        }
    else if(errorStatus2[0] == I2C_1_MSTR_NOT_READY)
        {
           UART_1_PutString("Error2 I2C_1_MSTR_NOT_READY\n"); 
           CyDelay(1000);
           I2C_1_MasterSendStop();
        }
    else if(errorStatus2[0] == I2C_1_MSTR_ERR_LB_NAK)
        {
            UART_1_PutString("Error2 I2C_1_MSTR_ERR_LB_NAK\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }
    else if(errorStatus2[0] == I2C_1_MSTR_ERR_ARB_LOST)
        {
            UART_1_PutString("Error2 I2C_1_MSTR_ERR_ARB_LOST\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }
    else if (errorStatus2[0] == I2C_1_MSTR_ERR_ABORT_START_GEN)
        {
            UART_1_PutString("Error2 I2C_1_MSTR_ERR_ABORT_START_GE\n");
            CyDelay(1000);
            I2C_1_MasterSendStop();
        }
        
    }  //While(1)
    return 0;
}
// Udprintning til UART temperaturen
void writeTempToUART(float temp)
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp 1: %.2f \r\n", temp);
    UART_1_PutString(outputBuffer);
}
void writeTempToUART2(float temp)
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp 2: %.2f \r\n", temp);
    UART_1_PutString(outputBuffer);
}


/* [] END OF FILE */
