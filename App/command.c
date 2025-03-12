/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "command.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uart_stdio_typedef RS232_UART;
extern uart_stdio_typedef DEBUG_UART;

tCmdLineEntry g_psCmdTable[] = {
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Charge Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "CHARGE_EN",		CMD_EN_CHARGE, 		" : Enable charge task" },
	
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "HELLO",			HELLO, 			" : TEST" },
	{ "HELP", 					CMD_HELP,					" : Display list of commands" },
	{ 0, 0, 0 }
};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Cap Control Command :::::::: */

int HELLO(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	// int receive_argm[2];
	// receive_argm[0] = atoi(argv[1]);
	// receive_argm[1] = atoi(argv[2]);

	// if ((receive_argm[0] > 300) || (receive_argm[0] < 0))
	// 	return CMDLINE_INVALID_ARG;
	// else if ((receive_argm[1] > 50) || (receive_argm[1] < 0))
	// 	return CMDLINE_INVALID_ARG;

	UART_Send_String(&RS232_UART, "> HI\n");

	return CMDLINE_OK;
}

int CMD_HELP(int argc, char *argv[])
{
	tCmdLineEntry *pEntry;

	UART_Send_String(&RS232_UART, "\nAvailable commands\r\n");
	UART_Send_String(&RS232_UART, "------------------\r\n");

	// Point at the beginning of the command table.
	pEntry = &g_psCmdTable[0];

	// Enter a loop to read each entry from the command table.  The
	// end of the table has been reached when the command name is NULL.
	while (pEntry->pcCmd) {
		// Print the command name and the brief description.
		UART_Send_String(&RS232_UART, pEntry->pcCmd);
		UART_Send_String(&RS232_UART, pEntry->pcHelp);
		UART_Send_String(&RS232_UART, "\r\n");

		// Advance to the next entry in the table.
		pEntry++;

	}
	// Return success.
	return (CMDLINE_OK);
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */