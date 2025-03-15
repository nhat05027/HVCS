/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdbool.h>

#include "cmd_line_task.h"
#include "command.h"

#include "app.h"

#include "cmd_line.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
struct _cmd_line_typedef
{
                uint16_t    buffer_size;
                char*       p_buffer;

    volatile    uint16_t    write_index;
    volatile    char        RX_char;
};
typedef struct _cmd_line_typedef cmd_line_typedef;
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uart_stdio_typedef  RS232_UART;
char                g_RS232_UART_TX_buffer[1024];
char                g_RS232_UART_RX_buffer[64];

uart_stdio_typedef  DEBUG_UART;
char                g_DEBUG_UART_TX_buffer[256];
char                g_DEBUG_UART_RX_buffer[64];

cmd_line_typedef    RS232_CMD_line;
char                g_RS232_CMD_line_buffer[64];

cmd_line_typedef    DEBUG_CMD_line;
char                g_DEBUG_CMD_line_buffer[64];

static const char * ErrorCode[7] = 
{
    "OK\n",
    "CMDLINE_BAD_CMD\n",
    "CMDLINE_TOO_MANY_ARGS\n",
    "CMDLINE_TOO_FEW_ARGS\n",
    "CMDLINE_INVALID_ARG\n",
    "CMDLINE_INVALID_CMD\n",
    "CALIB IS RUNNING, USE CALIB_EXIT TO EXIT CALIB\n",
};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool g_debug_on = false;
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: CMD Line Task Init :::::::: */
void CMD_Line_Task_Init()
{

    UART_Init(  &RS232_UART, RS232_UART_HANDLE, RS232_UART_IRQ,
                g_RS232_UART_TX_buffer, g_RS232_UART_RX_buffer,
                sizeof(g_RS232_UART_TX_buffer), sizeof(g_RS232_UART_RX_buffer));

    UART_Init(  &DEBUG_UART, DEBUG_UART_HANDLE, DEBUG_UART_IRQ,
        g_DEBUG_UART_TX_buffer, g_DEBUG_UART_RX_buffer,
        sizeof(g_DEBUG_UART_TX_buffer), sizeof(g_DEBUG_UART_RX_buffer));

    RS232_CMD_line.p_buffer         = g_RS232_CMD_line_buffer;
    RS232_CMD_line.buffer_size      = 64;
    RS232_CMD_line.write_index      = 0;

    if(RS232_CMD_line.buffer_size != 0)
    {
        memset((void *)RS232_CMD_line.p_buffer, 0, sizeof(RS232_CMD_line.p_buffer));
    }

    DEBUG_CMD_line.p_buffer         = g_DEBUG_CMD_line_buffer;
    DEBUG_CMD_line.buffer_size      = 64;
    DEBUG_CMD_line.write_index      = 0;

    if(DEBUG_CMD_line.buffer_size != 0)
    {
        memset((void *)DEBUG_CMD_line.p_buffer, 0, sizeof(DEBUG_CMD_line.p_buffer));
    }

    UART_Send_String(&RS232_UART, "HIGH VOLTAGE ELECTROPORATOR\n");
    UART_Send_String(&RS232_UART, "> ");

    UART_Send_String(&DEBUG_UART, "DEBUG\n");
}

/* :::::::::: CMD Line Task ::::::::::::: */
void RS232_CMD_Line_Task(void*)
{
    uint8_t cmd_return, time_out;

    for(time_out = 50; (!RX_BUFFER_EMPTY(&RS232_UART)) && (time_out != 0); time_out--)
    {
        RS232_CMD_line.RX_char = UART_Get_Char(&RS232_UART);
        
        if(((RS232_CMD_line.RX_char == 8) || (RS232_CMD_line.RX_char == 127)))
        {
            if (RS232_CMD_line.write_index == 0)
                break;

            RS232_CMD_line.write_index--;
            UART_Send_Char(&RS232_UART, RS232_CMD_line.RX_char);
            break;
        }

        UART_Send_Char(&RS232_UART, RS232_CMD_line.RX_char);

        if((RS232_CMD_line.RX_char == '\r') || (RS232_CMD_line.RX_char == '\n'))
        {
            if(RS232_CMD_line.write_index > 0)
            {
                // Add a NUL char at the end of the CMD
                RS232_CMD_line.p_buffer[RS232_CMD_line.write_index] = 0;
                RS232_CMD_line.write_index++;

                cmd_return = CmdLineProcess(RS232_CMD_line.p_buffer);
                //RS232_CMD_line.read_index = RS232_CMD_line.write_index;
                RS232_CMD_line.write_index    = 0;

                UART_Send_String(&RS232_UART, "> ");
                UART_Printf(&RS232_UART, ErrorCode[cmd_return]);
                UART_Send_String(&RS232_UART, "> ");
            }
        }
        else
        {
            RS232_CMD_line.p_buffer[RS232_CMD_line.write_index] = RS232_CMD_line.RX_char;
            RS232_CMD_line.write_index++;

            if (RS232_CMD_line.write_index > RS232_CMD_line.buffer_size)
            {
                // > CMD too long!
                // > 
                UART_Send_String(&RS232_UART, "\n> CMD too long!\n> ");
                //RS232_CMD_line.write_index = RS232_CMD_line.read_index;
                RS232_CMD_line.write_index    = 0;
            }
        }
    }
}


void DEBUG_CMD_Line_Task(void*)
{
    // uint8_t cmd_return, time_out;
    if (g_debug_on)
    {
        char buf[10];
        float v_temp = g_Feedback[0]*(1/g_volt_prescaler)*0.68;
        gcvt(v_temp, 3, buf);
        UART_Printf(&DEBUG_UART, "%s\n", buf);
    }
    
    // for(time_out = 50; (!RX_BUFFER_EMPTY(&DEBUG_UART)) && (time_out != 0); time_out--)
    // {
    //     DEBUG_CMD_line.RX_char = UART_Get_Char(&DEBUG_UART);
        
    //     if(((DEBUG_CMD_line.RX_char == 8) || (DEBUG_CMD_line.RX_char == 127)))
    //     {
    //         if (DEBUG_CMD_line.write_index == 0)
    //             break;

    //         DEBUG_CMD_line.write_index--;
    //         UART_Send_Char(&DEBUG_UART, DEBUG_CMD_line.RX_char);
    //         break;
    //     }

    //     UART_Send_Char(&DEBUG_UART, DEBUG_CMD_line.RX_char);

    //     if((DEBUG_CMD_line.RX_char == '\r') || (DEBUG_CMD_line.RX_char == '\n'))
    //     {
    //         if(DEBUG_CMD_line.write_index > 0)
    //         {
    //             // Add a NUL char at the end of the CMD
    //             DEBUG_CMD_line.p_buffer[DEBUG_CMD_line.write_index] = 0;
    //             DEBUG_CMD_line.write_index++;

    //             cmd_return = CmdLineProcess(DEBUG_CMD_line.p_buffer);
    //             //DEBUG_CMD_line.read_index = DEBUG_CMD_line.write_index;
    //             DEBUG_CMD_line.write_index    = 0;

    //             UART_Send_String(&DEBUG_UART, "> ");
    //             UART_Printf(&DEBUG_UART, ErrorCode[cmd_return]);
    //             UART_Send_String(&DEBUG_UART, "> ");
    //         }
    //         else
    //         {

    //             UART_Send_String(&DEBUG_UART, "> ");
    //         }
    //     }
    //     else
    //     {
    //         DEBUG_CMD_line.p_buffer[DEBUG_CMD_line.write_index] = DEBUG_CMD_line.RX_char;
    //         DEBUG_CMD_line.write_index++;

    //         if (DEBUG_CMD_line.write_index > DEBUG_CMD_line.buffer_size)
    //         {
    //             UART_Send_String(&DEBUG_UART, "\n> CMD too long!\n> ");
    //             //RF_CMD_line.write_index = RF_CMD_line.read_index;
    //             DEBUG_CMD_line.write_index    = 0;
    //         }
    //     }
    // }
}

/* :::::::::: IRQ Handler ::::::::::::: */
void RS232_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_TXE(RS232_UART.handle) == true)
    {
        if(TX_BUFFER_EMPTY(&RS232_UART))
        {
            // Buffer empty, so disable interrupts
            LL_USART_DisableIT_TXE(RS232_UART.handle);
        }
        else
        {
            // There is more data in the output buffer. Send the next byte
            UART_Prime_Transmit(&RS232_UART);
        }
    }

    if(LL_USART_IsActiveFlag_RXNE(RS232_UART.handle) == true)
    {
        RS232_UART.RX_irq_char = LL_USART_ReceiveData8(RS232_UART.handle);

        // NOTE: On win 10, default PUTTY when hit enter only send back '\r',
        // while on default HERCULES when hit enter send '\r\n' in that order.
        // The code bellow is modified so that it can work on PUTTY and HERCULES.
        if((!RX_BUFFER_FULL(&RS232_UART)) && (RS232_UART.RX_irq_char != '\n'))
        {
            if (RS232_UART.RX_irq_char == '\r')
            {
                RS232_UART.p_RX_buffer[RS232_UART.RX_write_index] = '\n';
                ADVANCE_RX_WRITE_INDEX(&RS232_UART);
            }
            else
            {
                RS232_UART.p_RX_buffer[RS232_UART.RX_write_index] = RS232_UART.RX_irq_char;
                ADVANCE_RX_WRITE_INDEX(&RS232_UART);
            }
        }
    }
}

void DEBUG_IRQHandler(void)
{
    if(LL_USART_IsActiveFlag_TXE(DEBUG_UART.handle) == true)
    {
        if(TX_BUFFER_EMPTY(&DEBUG_UART))
        {
            // Buffer empty, so disable interrupts
            LL_USART_DisableIT_TXE(DEBUG_UART.handle);
        }
        else
        {
            // There is more data in the output buffer. Send the next byte
            UART_Prime_Transmit(&DEBUG_UART);
        }
    }

    if(LL_USART_IsActiveFlag_RXNE(DEBUG_UART.handle) == true)
    {
        DEBUG_UART.RX_irq_char = LL_USART_ReceiveData8(DEBUG_UART.handle);

        // NOTE: On win 10, default PUTTY when hit enter only send back '\r',
        // while on default HERCULES when hit enter send '\r\n' in that order.
        // The code bellow is modified so that it can work on PUTTY and HERCULES.
        if((!RX_BUFFER_FULL(&DEBUG_UART)) && (DEBUG_UART.RX_irq_char != '\n'))
        {
            if (DEBUG_UART.RX_irq_char == '\r')
            {
                DEBUG_UART.p_RX_buffer[DEBUG_UART.RX_write_index] = '\n';
                ADVANCE_RX_WRITE_INDEX(&DEBUG_UART);
            }
            else
            {
                DEBUG_UART.p_RX_buffer[DEBUG_UART.RX_write_index] = DEBUG_UART.RX_irq_char;
                ADVANCE_RX_WRITE_INDEX(&DEBUG_UART);
            }
        }
    }
}
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
