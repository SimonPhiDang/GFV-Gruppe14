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

CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(ISR_timer_handler);
void handleByteReceived(uint8_t byteReceived);

void WaveDrive(), FullStep(), HalfStep(), Stop(), status(),
skiftRet(),increaseSpeed(),decreaseSpeed();

uint8 High = 1, Low = 0;
uint16 timerCount = 0;
uint16 timerInterrupt = 1000;
typedef enum  // Forskellige Modes
{
    Mode_WaveDrive,
    Mode_FullStep,
    Mode_HalfStep,
    Mode_Stop
}Mode;

typedef enum  // De mulige positioner
{
    Position_A1,  // A
    Position_B1,  // B 
    Position_A2,  // A/
    Position_B2,   // B/
    Position_A1B1, // A+B
    Position_B1A2, // B+A/
    Position_A2B2, // A/+B/
    Position_B2A1, // B/+A
}Position;

typedef enum  // Hvilken retning steppermotor skal køre
{
   Rotate_F,
   Rotate_B
}Rotate;

static Mode modeChosen;
static Rotate currRot = Rotate_F;
static Position currPos = Position_A1;

int main(void)
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    isr_uart_rx_StartEx(ISR_UART_rx_handler);
    isr_timer_StartEx(ISR_timer_handler);
    
    UART_1_Start();
    Timer_1_Start();
    
    UART_1_PutString("DC-Motor-PWM application started\r\n");
    UART_1_PutString("1: WaveDrive\r\n");
    UART_1_PutString("2: FullStep\r\n");
    UART_1_PutString("3: HalfStep\r\n");
    UART_1_PutString("4: Stop\r\n");
    UART_1_PutString("5: Rotate\r\n");
    UART_1_PutString("q: Increase speed\r\n");
    UART_1_PutString("w: Decrease speed\r\n");    
    for(;;)
    {}
}

CY_ISR(ISR_timer_handler)
{
   timerCount++;
    if(timerCount == timerInterrupt)
    {
        status();
        timerCount = 0; 
        
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
        case '1' :
        {
            WaveDrive(); 
        }
        break;
        case '2' :
        {
            FullStep();
        }
        break;
        case '3' :
        {
            HalfStep();
        }
        break;
        case '4' :
        {
            Stop();
        }
        break;
        case '5' :
        {
             skiftRet();  
        }
        case 'q' :
        {
            increaseSpeed();               
        }
        case 'w' :
        {
            decreaseSpeed();   
        }
        default :
        {
            // nothing
        }
        break;
    }
}

void WaveDrive()
{
    UART_1_PutString("WaveDrive");
    modeChosen = Mode_WaveDrive;
}

void FullStep()
{
    UART_1_PutString("FullStep");
    modeChosen = Mode_FullStep;
}

void HalfStep()
{
    UART_1_PutString("HalfStep");
    modeChosen = Mode_HalfStep;
}

void Stop()
{
    UART_1_PutString("Stop");
    modeChosen = Mode_Stop;
}

void skiftRet()
{
    if(currRot == Rotate_F)
    {
        currRot = Rotate_B;
    }
    else
    {
        currRot = Rotate_F;   
    }
}

void increaseSpeed()
{
    if(timerInterrupt >= 0 && timerInterrupt < 5000)
    {
        timerInterrupt = timerInterrupt + 250;
    }
}

void decreaseSpeed()
{
    if(timerInterrupt > 0 && timerInterrupt <= 5000)
    {
        timerInterrupt = timerInterrupt - 250;
    }
}

void status()
{
    switch(modeChosen)
    {
        case Mode_WaveDrive:   // Mode
            switch(currRot)        // Retning
            {
                case Rotate_F:   
                    switch(currPos)   // Position
                    {
                        case Position_A1:   // Pin_1a ON HVID LEDNING
                            UART_1_PutString("1");
                            Pin_1a_Write(High);  
                            Pin_1b_Write(Low);                            
                            Pin_2a_Write(Low);                            
                            Pin_2b_Write(Low);
                            currPos = Position_B1;
                        break;
                        case Position_B1:  // Pin_1b ON BLÅ LEDNING
                            UART_1_PutString("2");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(High);                            
                            Pin_2a_Write(Low);                            
                            Pin_2b_Write(Low);
                            currPos = Position_A2;
                        break;                                                     
                            
                        case Position_A2:  // Pin_2a ON RØD Ledning
                            UART_1_PutString("3");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(Low);                            
                            Pin_2a_Write(High);                            
                            Pin_2b_Write(Low);
                            currPos = Position_B2;
                        break;
                            
                        case Position_B2:  // Pin_2b ON GUL LEDNING
                            UART_1_PutString("4");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(Low);
                            Pin_2a_Write(Low);
                            Pin_2b_Write(High);
                            currPos = Position_A1;
                        break;
                        default:
                            {
                            }
                        break;
                    }
                            
                break;
                case Rotate_B:
                    switch(currPos)   // Position
                    {
                            case Position_A1:
                            UART_1_PutString("1");
                            Pin_1a_Write(High);  
                            Pin_1b_Write(Low);                            
                            Pin_2a_Write(Low);                            
                            Pin_2b_Write(Low);
                            currPos = Position_B2;
                        break;
                        case Position_B2:  
                            UART_1_PutString("4");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(Low);                            
                            Pin_2a_Write(Low);                            
                            Pin_2b_Write(High);
                            currPos = Position_A2;
                        break;                                                     
                            
                        case Position_A2: 
                            UART_1_PutString("3");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(Low);                            
                            Pin_2a_Write(High);                            
                            Pin_2b_Write(Low);
                            currPos = Position_B1;
                        break;
                            
                        case Position_B1:
                            UART_1_PutString("2");
                            Pin_1a_Write(Low);
                            Pin_1b_Write(High);
                            Pin_2a_Write(Low);
                            Pin_2b_Write(Low);
                            currPos = Position_A1;
                        break;
                        default:
                            {
                            }
                        break;                        
                    }
                break;
            }
        
        break;
        
        case Mode_FullStep:
                switch(currRot)        // Retning
                    {
                        case Rotate_F:   
                            switch(currPos)   // Position
                            {
                                case Position_A1B1:   // Pin_1a ON HVID LEDNING
                                    UART_1_PutString("1");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B1A2;
                                break;
                                case Position_B1A2:  // Pin_1b ON BLÅ LEDNING
                                    UART_1_PutString("2");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A2B2;
                                break;                                                     
                                    
                                case Position_A2B2:  // Pin_2a ON RØD Ledning
                                    UART_1_PutString("3");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_B2A1;
                                break;
                                    
                                case Position_B2A1:  // Pin_2b ON GUL LEDNING
                                    UART_1_PutString("4");
                                    Pin_1a_Write(High);
                                    Pin_1b_Write(Low);
                                    Pin_2a_Write(Low);
                                    Pin_2b_Write(High);
                                    currPos = Position_A1B1;
                                break;
                                default:
                                    {
                                    }
                                break;
                            }
                                    
                        break;
                        case Rotate_B:
                            switch(currPos)   // Position
                            {
                                    case Position_A1B1:
                                    UART_1_PutString("1");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B2A1;
                                break;
                                case Position_B2A1:  
                                    UART_1_PutString("4");
                                    Pin_1a_Write(High);
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_A2B2;
                                break;                                                     
                                    
                                case Position_A2B2: 
                                    UART_1_PutString("3");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_B1A2;
                                break;
                                    
                                case Position_B1A2:
                                    UART_1_PutString("2");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(High);
                                    Pin_2a_Write(High);
                                    Pin_2b_Write(Low);
                                    currPos = Position_A1B1;
                                break;
                                default:
                                    {
                                    }
                                break;
                            }
                        break;
                    }        
        break;
        
        case Mode_HalfStep:
               switch(currRot)        // Retning
                    {
                        case Rotate_F:   
                            switch(currPos)   // Position
                            {
                                case Position_A1:   // 1
                                    UART_1_PutString("1");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A1B1;
                                break;
                                case Position_A1B1:   // 2
                                    UART_1_PutString("2");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B1;
                                break;
                                    
                                case Position_B1:   // 3
                                    UART_1_PutString("3");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B1A2; 
                                break;    
                                case Position_B1A2:  // 4
                                    UART_1_PutString("4");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A2;
                                break;                                                     
                                case Position_A2:   // 5
                                    UART_1_PutString("5");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A2B2;
                                break;    
                                case Position_A2B2:  // 6
                                    UART_1_PutString("6");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_B2;
                                break;
                                case Position_B2:   // 7
                                    UART_1_PutString("7");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_B2A1; 
                                break;   
                                case Position_B2A1:  // 8
                                    UART_1_PutString("8");
                                    Pin_1a_Write(High);
                                    Pin_1b_Write(Low);
                                    Pin_2a_Write(Low);
                                    Pin_2b_Write(High);
                                    currPos = Position_A1;
                                break;      
                                default:
                                    {}
                                break;
                                      
                            }
                                    
                        break;
                        case Rotate_B:
                            switch(currPos)   // Position
                            {
                                case Position_A1:   // 1
                                    UART_1_PutString("1");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B2A1;
                                break;
                                case Position_A1B1:   // 2
                                    UART_1_PutString("2");
                                    Pin_1a_Write(High);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A1;
                                break;
                                    
                                case Position_B1:   // 3
                                    UART_1_PutString("3");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_A1B1; 
                                break;    
                                case Position_B1A2:  // 4
                                    UART_1_PutString("4");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(High);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B1;
                                break;                                                     
                                case Position_A2:   // 5
                                    UART_1_PutString("5");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(Low);
                                    currPos = Position_B1A2;
                                break;    
                                case Position_A2B2:  // 6
                                    UART_1_PutString("6");
                                    Pin_1a_Write(Low);
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(High);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_A2;
                                break;
                                case Position_B2:   // 7
                                    UART_1_PutString("7");
                                    Pin_1a_Write(Low);  
                                    Pin_1b_Write(Low);                            
                                    Pin_2a_Write(Low);                            
                                    Pin_2b_Write(High);
                                    currPos = Position_A2B2;
                                break;   
                                case Position_B2A1:  // 8
                                    UART_1_PutString("8");
                                    Pin_1a_Write(High);
                                    Pin_1b_Write(Low);
                                    Pin_2a_Write(Low);
                                    Pin_2b_Write(High);
                                    currPos = Position_B2;
                                break;
                                default:
                                    {}
                                break;
                            }
                        break;
                    }                
        break;
        
        case Mode_Stop:
        
        break;
        
        default:
        
        break;
        
    }
    
    
}

/* [] END OF FILE */
