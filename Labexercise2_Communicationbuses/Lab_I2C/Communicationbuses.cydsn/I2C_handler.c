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

#define LM75address 0x48  // 1001 +  A0 = 0 , A1 = 0 A2 = 0
#define LM75address2 0x49 // 1001 +  A0 = 1 , A1 = 0 , A2 = 0

#define CHECK_BIT(var,pos) (((var)>>(pos)) & 1) // Bit manipulation

int8 readTemp()
{
    //Variabler 
    uint8 dataRecv[2] = {0,0};
    uint8 dataRecv2[2] = {0,0};
    uint8 errorStatus[2] = {0,0};
    uint8 errorStatus2[2] ={0,0};
    float temp;
    float temp2;
    
    while(1)  // Uendelig loop til måling af temp
    {
    errorStatus[0] = I2C_1_MasterSendStart(LM75address,I2C_1_READ_XFER_MODE); // Start den første LM75
    if (errorStatus[0] == I2C_1_MSTR_NO_ERROR)  // Tjekker for fejl
        {
            dataRecv[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  // Læser MSB 8 bit som er temperaturen
            dataRecv[1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);  // Læser LSB 8 bit hvor 0.5 findes       
            I2C_1_MasterSendStop();    //  Data er opsamlet så der sendes stop til I2C master  

            temp = dataRecv[0]; // uint8 temperaturen sættes i float
            if(CHECK_BIT(dataRecv[1],7) == 1)  // Tjekker om det specifikke bit er 1, dvs at man har 0.5 tilføjet
            {
                temp += 0.5;
            }
            if(CHECK_BIT(dataRecv[0],7) == 1) // Tjekker fortegn om det + eller - temperatur
            {
                temp -=128;
            }

            writeTempToUART(temp);  // Send temperature to UART
            CyDelay(1000);  // Delay med at få ny temperatur måling
        }
    else if(errorStatus[0] == I2C_1_MSTR_BUS_BUSY)
        {
          UART_1_PutString("Error I2C_1_MSTR_BUS_BUSY\n");  // Report specifik error på UART
          I2C_1_MasterSendStop();    //  Fejl så I2C master skal sende stop
          CyDelay(1000);  // Delay med næste temperatur måling
        }
    else if(errorStatus[0] == I2C_1_MSTR_NOT_READY)
        {
          UART_1_PutString("Error I2C_1_MSTR_NOT_READY\n"); // Report specifik error på UART
          I2C_1_MasterSendStop();    //  Fejl så I2C master skal sende stop
          CyDelay(1000);  // Delay med næste temperatur måling
        }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_LB_NAK)
        {
          UART_1_PutString("Error I2C_1_MSTR_ERR_LB_NAK\n"); // Report specifik error på UART
          I2C_1_MasterSendStop();    //  Fejl så I2C master skal sende stop
          CyDelay(1000);  // Delay med næste temperatur måling
        }
    else if(errorStatus[0] == I2C_1_MSTR_ERR_ARB_LOST)
        {
          UART_1_PutString("Error I2C_1_MSTR_ERR_ARB_LOST\n"); // Report specifik error på UART
          I2C_1_MasterSendStop();    //  Fejl så I2C master skal sende stop
          CyDelay(1000);  // Delay med næste temperatur måling
        }
    else if (errorStatus[0] == I2C_1_MSTR_ERR_ABORT_START_GEN)
        {
          UART_1_PutString("Error I2C_1_MSTR_ERR_ABORT_START_GE\n"); // Report specifik error på UART
          I2C_1_MasterSendStop();    //  Fejl så I2C master skal sende stop
          CyDelay(1000);  // Delay med næste temperatur måling
        }


    errorStatus2[0] = I2C_1_MasterSendStart(LM75address2,I2C_1_READ_XFER_MODE);  // Start af anden LM75 da første er færdig  
    
    if (errorStatus2[0] == I2C_1_MSTR_NO_ERROR) // Den anden LM75 gentages med samme opbygning, dog med 2 på de forskelige variabler
        {
                dataRecv2[0] = I2C_1_MasterReadByte(I2C_1_ACK_DATA);  
                dataRecv2[1] = I2C_1_MasterReadByte(I2C_1_NAK_DATA);
                I2C_1_MasterSendStop();         
                temp2 = dataRecv2[0];
                if(CHECK_BIT(dataRecv[1],7) == 1)
                {
                    temp2 += 0.5;
                }
                if(CHECK_BIT(dataRecv[0],7) == 1)
                {
                    temp2 -=128;
                }
                writeTempToUART2(temp2);
                CyDelay(1000);
        }
    else if(errorStatus2[0] == I2C_1_MSTR_BUS_BUSY)
        {
          UART_1_PutString("Error2 I2C_1_MSTR_BUS_BUSY\n");
          I2C_1_MasterSendStop(); 
          CyDelay(1000);  
        }
    else if(errorStatus2[0] == I2C_1_MSTR_NOT_READY)
        {
          UART_1_PutString("Error2 I2C_1_MSTR_NOT_READY\n"); 
          I2C_1_MasterSendStop(); 
          CyDelay(1000);  
        }
    else if(errorStatus2[0] == I2C_1_MSTR_ERR_LB_NAK)
        {
          UART_1_PutString("Error2 I2C_1_MSTR_ERR_LB_NAK\n");
          I2C_1_MasterSendStop(); 
          CyDelay(1000);  
        }
    else if(errorStatus2[0] == I2C_1_MSTR_ERR_ARB_LOST)
        {
          UART_1_PutString("Error2 I2C_1_MSTR_ERR_ARB_LOST\n");
          I2C_1_MasterSendStop(); 
          CyDelay(1000);  
        }
    else if (errorStatus2[0] == I2C_1_MSTR_ERR_ABORT_START_GEN)
        {
          UART_1_PutString("Error2 I2C_1_MSTR_ERR_ABORT_START_GE\n");
          I2C_1_MasterSendStop(); 
          CyDelay(1000);  
        }
        
    }  //While(1) 
    return 0;
}

void writeTempToUART(float temp) // Udprintning til UART temperaturen af første LM75
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp 1: %.2f \r\n", temp);
    UART_1_PutString(outputBuffer);
}
void writeTempToUART2(float temp) // Udprintning til UART temperaturen af anden LM75
{
    char outputBuffer[256];
    snprintf(outputBuffer, sizeof(outputBuffer), "Temp 2: %.2f \r\n", temp);
    UART_1_PutString(outputBuffer);
}


/* [] END OF FILE */
