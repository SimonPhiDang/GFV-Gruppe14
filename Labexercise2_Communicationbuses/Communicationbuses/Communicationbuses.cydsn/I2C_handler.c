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

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1)

int8 readTemp()
{
    //Variabler 
    uint8 dataRecv[2] = {0,0};
    uint8 errorStatus[2] = {0,0};
    int16_t tempData;
    float temp;
    uint8 n = 9;
    while(1)
    {
    //1001 A2 A1 A0  Alle A'er er modsat ON = 0 
    errorStatus[0] = I2C_1_MasterSendStart(LM75address,I2C_1_READ_XFER_MODE);

    if (errorStatus[0] == I2C_1_MSTR_NO_ERROR)
        {
            // Læser første byte i register MSB første 8 bit
                dataRecv[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  
                UART_1_PutString("ack success\n");
            // Læser anden byte i register, sidste byte er NAKed  // LSB sidste 8 bit
                dataRecv[1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
                UART_1_PutString("nack success\n");
            //    MasterSendStop
                I2C_1_MasterSendStop();        
            //    The below function will return either a 1 or 0 if the bit is enabled and not the position.
                CHECK_BIT(dataRecv[0], n - 1);
            // 2 Komplement 
                tempData = dataRecv[0] | dataRecv[1];
            // Data skal aflæses som 2 komplements int så fortegn er korrekt
                temp = tempData / 256;
            //    Send temperature to UART
                writeTempToUART(temp);
                CyDelay(1000);
        }
    else if(errorStatus[0] == I2C_1_MSTR_BUS_BUSY)
        {
    //    report error on UART
          UART_1_PutString("Error BUS BUsy\n");
          CyDelay(1000);
    //    MasterSendStop
          I2C_1_MasterSendStop();
        }
    else if(errorStatus[0] == I2C_1_MSTR_NOT_READY)
        {
           UART_1_PutString("Error NOT rdy\n"); 
           CyDelay(1000);
        }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_LB_NAK)
    {
        UART_1_PutString("Error LB NAK\n");
        CyDelay(1000);
    }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_ARB_LOST)
    {
        UART_1_PutString("Error ARB LOST\n");
        CyDelay(1000);
    }
    else if (errorStatus[0] == I2C_1_MSTR_ERR_ABORT_START_GEN)
    {
        UART_1_PutString("Error ABORT STart GEN\n");
        CyDelay(1000);
    }
        
    }  //While(1)
    return 0;
}
// Udprintning til UART temperaturen
void writeTempToUART(float temp)
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp: %f \r\n", temp);
    UART_1_PutString(outputBuffer);
}


/* [] END OF FILE */
