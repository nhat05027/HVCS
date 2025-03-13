/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "command.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool is_relay_on[6] = {false, false, false, false, false, false};
bool is_relay_lock[6] = {false, false, false, false, false, false};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static void alloff_relay();
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern uart_stdio_typedef RS232_UART;
extern uart_stdio_typedef DEBUG_UART;
int g_volt_prescaler = 5;

tCmdLineEntry g_psCmdTable[] = {
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Charge Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "EN_CHARGE",		CMD_EN_CHARGE, 			" : Enable charge task" },
	{ "SET_VOLT",		CMD_SET_VOLTAGE, 		" : Set target voltage" },
	{ "STATUS_CHARGE",	CMD_GET_STATUS_CHARGE, 	" : Get charge status" },
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~Switching Cap Command ~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "PARALLEL",		CMD_PARALLEL_CAP, 		" : Set parrallel capacitor" },
	{ "SERIAL",			CMD_SERIAL_CAP, 		" : Set serial capacitor" },
	{ "STATUS_CAP",		CMD_GET_STATUS_CAP, 	" : Get capacitor status" },
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Relay Cap Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "SET_RELAY",		CMD_CONTROL_RELAY, 		" : Set relay on/off" },
	{ "ALLOFF_RELAY",	CMD_ALLOFF_RELAY, 		" : Set all relay off" },
	{ "LOCK_RELAY",		CMD_LOCK_RELAY, 		" : Lock relay" },
	{ "UNLOCK_RELAY",	CMD_UNLOCK_RELAY, 		" : Unlock relay" },
	{ "STATUS_RELAY",	CMD_GET_STATUS_RELAY, 	" : Get all relay status" },
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Half Bridge Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "SET_PULSE",		CMD_SET_PULSE_HB, 		" : Set pulse out" },
	{ "PULSE_OUT",		CMD_PULSE_OUT_HB, 		" : Out pulse" },
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ VOM Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "SET_PRESCALE",	CMD_SET_PRESCALE_VOLT, 	" : Set prescaler" },
	{ "GET_VOLT",		CMD_GET_VOLT, 			" : Get voltage" },
	{ "GET_CURRENT", 	CMD_GET_CURRENT,		" : Get current" },
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Ultility Command ~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
	{ "DEBUG",			CMD_MONITOR_DEBUG, 		" : Enable debug uart" },
	{ "RESET",			CMD_RESET, 				" : Reset" },
	{ "HELP", 			CMD_HELP,				" : Display list of commands" },
	{ 0, 0, 0 }
};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Cap Control Command :::::::: */
int CMD_EN_CHARGE(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	int receive_argm;
	receive_argm = atoi(argv[1]);
	g_en_charge = (receive_argm==1);

	return CMDLINE_OK;
}

int CMD_SET_VOLTAGE(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	int receive_argm = atoi(argv[1]);
	if (receive_argm>=0 & receive_argm<=300)
	{
		g_set_voltage = receive_argm;
		UART_Printf(&RS232_UART, "> Set voltage to %dV\n", g_set_voltage);
	}
	else UART_Send_String(&RS232_UART, "> Invalid voltage!\n");

	return CMDLINE_OK;
}
int CMD_GET_STATUS_CHARGE(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	UART_Printf(&RS232_UART, "> Cap voltage: %dV\n", g_Feedback[0]*200*g_volt_prescaler/3300);
	UART_Printf(&RS232_UART, "> Set voltage: %dV\n", g_set_voltage);
	UART_Printf(&RS232_UART, "> Is charging: %s\n", g_en_charge ? "True" : "False");

	return CMDLINE_OK;
}

/* ::::::: Switching Cap Command :::::: */
int CMD_PARALLEL_CAP(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	
	alloff_relay();

	LL_GPIO_SetOutputPin(H_P12_PORT, H_P12_PIN);
	LL_GPIO_SetOutputPin(H_P23_PORT, H_P23_PIN);
	LL_GPIO_SetOutputPin(IN_P2G_PORT, IN_P2G_PIN);
	LL_GPIO_SetOutputPin(IN_P3G_PORT, IN_P3G_PIN);
	is_relay_on[2] = true;
	is_relay_on[3] = true;
	is_relay_on[4] = true;
	is_relay_on[5] = true;
	UART_Send_String(&RS232_UART, "> Parallel Capacitor!\n");

	return CMDLINE_OK;
}
int CMD_SERIAL_CAP(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	
	alloff_relay();

	LL_GPIO_SetOutputPin(H_S12_PORT, H_S12_PIN);
	LL_GPIO_SetOutputPin(H_S23_PORT, H_S23_PIN);
	is_relay_on[0] = false;
	is_relay_on[1] = false;
	
	UART_Send_String(&RS232_UART, "> Serial Capacitor!\n");

	return CMDLINE_OK;
}
int CMD_GET_STATUS_CAP(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	UART_Send_String(&RS232_UART, "> HI\n");

	return CMDLINE_OK;
}

/* ::::::::: Relay Cap Command :::::::: */
int CMD_CONTROL_RELAY(int argc, char *argv[])
{
	if (argc < 3)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 3)
		return CMDLINE_TOO_MANY_ARGS;
	if (argv[1]=='S12') 
	{
		if (!is_relay_lock[0]) 
		{
			if (atoi(argv[2])==1)
			{
				LL_GPIO_SetOutputPin(H_S12_PORT, H_S12_PIN);
				is_relay_on[0] = true;
				UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
			}
			else
			{
				LL_GPIO_ResetOutputPin(H_S12_PORT, H_S12_PIN);
				is_relay_on[0] = false;
				UART_Printf(&RS232_UART, "> Disable %s\n", argv[1]);
			}
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else if (argv[1]=='S23')
	{
		if (!is_relay_lock[1]) 
		{
			LL_GPIO_SetOutputPin(H_S23_PORT, H_S23_PIN);
			is_relay_on[1] = true;
			UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else if (argv[1]=='P12')
	{
		if (!is_relay_lock[2]) 
		{
			LL_GPIO_SetOutputPin(H_P12_PORT, H_P12_PIN);
			is_relay_on[2] = true;
			UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else if (argv[1]=='P23')
	{
		if (!is_relay_lock[3]) 
		{
			LL_GPIO_SetOutputPin(H_P23_PORT, H_P23_PIN);
			is_relay_on[3] = true;
			UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else if (argv[1]=='P2G')
	{
		if (!is_relay_lock[4]) 
		{
			LL_GPIO_SetOutputPin(IN_P2G_PORT, IN_P2G_PIN);
			is_relay_on[4] = true;
			UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else if (argv[1]=='P3G')
	{
		if (!is_relay_lock[5]) 
		{
			LL_GPIO_SetOutputPin(IN_P3G_PORT, IN_P3G_PIN);
			is_relay_on[5] = true;
			UART_Printf(&RS232_UART, "> Enable %s\n", argv[1]);
		} 
		else UART_Printf(&RS232_UART, "> Relay %s is locked!\n", argv[1]);
	}
	else
	{
		UART_Send_String(&RS232_UART, "> Invalid relay name!\n");
		return CMDLINE_OK;
	}
	
	return CMDLINE_OK;

	return CMDLINE_OK;
}int CMD_ALLOFF_RELAY(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;

	alloff_relay();
	UART_Send_String(&RS232_UART, "> ALL RELAY OFF!\n");

	return CMDLINE_OK;
}
int CMD_LOCK_RELAY(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	if (argv[1]=='S12') is_relay_lock[0] = true;
 	else if (argv[1]=='S23') is_relay_lock[1] = true;
	else if (argv[1]=='P12') is_relay_lock[2] = true;
	else if (argv[1]=='P23') is_relay_lock[3] = true;
	else if (argv[1]=='P2G') is_relay_lock[4] = true;
	else if (argv[1]=='P3G') is_relay_lock[5] = true;
	else
	{
		UART_Send_String(&RS232_UART, "> Invalid relay name!\n");
		return CMDLINE_OK;
	}
	
	UART_Printf(&RS232_UART, "> Lock relay %s\n", argv[1]);
	return CMDLINE_OK;
}
int CMD_UNLOCK_RELAY(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	if (argv[1]=='S12') is_relay_lock[0] = false;
	else if (argv[1]=='S23') is_relay_lock[1] = false;
	else if (argv[1]=='P12') is_relay_lock[2] = false;
	else if (argv[1]=='P23') is_relay_lock[3] = false;
	else if (argv[1]=='P2G') is_relay_lock[4] = false;
	else if (argv[1]=='P3G') is_relay_lock[5] = false;
	else
	{
		UART_Send_String(&RS232_UART, "> Invalid relay name!\n");
		return CMDLINE_OK;
	}
	
	UART_Printf(&RS232_UART, "> Unlock relay %s\n", argv[1]);
	return CMDLINE_OK;
}
int CMD_GET_STATUS_RELAY(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	UART_Printf(&RS232_UART, "> S12: %s %s\n", is_relay_on[0]?"ON":"OFF", is_relay_lock[0]?"Locked":"Free");
	UART_Printf(&RS232_UART, "> S23: %s %s\n", is_relay_on[1]?"ON":"OFF", is_relay_lock[1]?"Locked":"Free");
	UART_Printf(&RS232_UART, "> P12: %s %s\n", is_relay_on[2]?"ON":"OFF", is_relay_lock[2]?"Locked":"Free");
	UART_Printf(&RS232_UART, "> P23: %s %s\n", is_relay_on[3]?"ON":"OFF", is_relay_lock[3]?"Locked":"Free");
	UART_Printf(&RS232_UART, "> P2G: %s %s\n", is_relay_on[4]?"ON":"OFF", is_relay_lock[4]?"Locked":"Free");
	UART_Printf(&RS232_UART, "> P3G: %s %s\n", is_relay_on[5]?"ON":"OFF", is_relay_lock[5]?"Locked":"Free");

	return CMDLINE_OK;
}

/* ::::::::: Half Bridge Command :::::::: */
int CMD_SET_PULSE_HB(int argc, char *argv[])
{
	if (argc < 4)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 4)
		return CMDLINE_TOO_MANY_ARGS;
	HB_freq = atoi(argv[1]);
	HB_duty = atoi(argv[2]);
	HB_cycle = atoi(argv[3]);
	UART_Printf(&RS232_UART, "> Set pulse: %dHz %d%% %d cycle.\n", HB_freq, HB_duty, HB_cycle);

	return CMDLINE_OK;
}
int CMD_PULSE_OUT_HB(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	int receive_argm;
	receive_argm = atoi(argv[1]);
	HB_on = (receive_argm==1);

	return CMDLINE_OK;
}
/* ::::::::::::: VOM Command :::::::::::: */
int CMD_SET_PRESCALE_VOLT(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	g_volt_prescaler = atoi(argv[1]);
	switch (g_volt_prescaler)
	{
	case 1:
		LL_GPIO_ResetOutputPin(V_DIV1_PORT, v_DIV1_PIN);
		LL_GPIO_ResetOutputPin(V_DIV2_PORT, v_DIV2_PIN);
		LL_GPIO_ResetOutputPin(V_DIV3_PORT, v_DIV3_PIN);
		LL_GPIO_ResetOutputPin(V_DIV4_PORT, v_DIV4_PIN);
		break;
	case 2:
		LL_GPIO_SetOutputPin(V_DIV1_PORT, v_DIV1_PIN);
		LL_GPIO_ResetOutputPin(V_DIV2_PORT, v_DIV2_PIN);
		LL_GPIO_ResetOutputPin(V_DIV3_PORT, v_DIV3_PIN);
		LL_GPIO_ResetOutputPin(V_DIV4_PORT, v_DIV4_PIN);
		break;
	case 3:
		LL_GPIO_SetOutputPin(V_DIV1_PORT, v_DIV1_PIN);
		LL_GPIO_SetOutputPin(V_DIV2_PORT, v_DIV2_PIN);
		LL_GPIO_ResetOutputPin(V_DIV3_PORT, v_DIV3_PIN);
		LL_GPIO_ResetOutputPin(V_DIV4_PORT, v_DIV4_PIN);
		break;
	case 4:
		LL_GPIO_SetOutputPin(V_DIV1_PORT, v_DIV1_PIN);
		LL_GPIO_SetOutputPin(V_DIV2_PORT, v_DIV2_PIN);
		LL_GPIO_SetOutputPin(V_DIV3_PORT, v_DIV3_PIN);
		LL_GPIO_ResetOutputPin(V_DIV4_PORT, v_DIV4_PIN);
		break;
	default:
		LL_GPIO_SetOutputPin(V_DIV1_PORT, v_DIV1_PIN);
		LL_GPIO_SetOutputPin(V_DIV2_PORT, v_DIV2_PIN);
		LL_GPIO_SetOutputPin(V_DIV3_PORT, v_DIV3_PIN);
		LL_GPIO_SetOutputPin(V_DIV4_PORT, v_DIV4_PIN);
		g_volt_prescaler = 5;
		break;
	}
	

	return CMDLINE_OK;
}
int CMD_GET_VOLT(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	UART_Printf(&RS232_UART, "> VOLTAGE: %u \n", g_Feedback[0]*200*g_volt_prescaler/3300);

	return CMDLINE_OK;
}
int CMD_GET_CURRENT(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	UART_Printf(&RS232_UART, "> CURRENT: %u \n", g_Feedback[1]*10/3300);

	return CMDLINE_OK;
}
/* :::::::::: Ultilities Command :::::::: */
int CMD_MONITOR_DEBUG(int argc, char *argv[])
{
	if (argc < 2)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 2)
		return CMDLINE_TOO_MANY_ARGS;
	int receive_argm;
	receive_argm = atoi(argv[1]);
	g_debug_on = (receive_argm==1);

	return CMDLINE_OK;
}

int CMD_RESET(int argc, char *argv[])
{
	if (argc < 1)
		return CMDLINE_TOO_FEW_ARGS;
	else if (argc > 1)
		return CMDLINE_TOO_MANY_ARGS;
	UART_Send_String(&RS232_UART, "> Reset\n");

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
static void alloff_relay()
{
    LL_GPIO_ResetOutputPin(H_S12_PORT, H_S12_PIN);
	LL_GPIO_ResetOutputPin(H_S23_PORT, H_S23_PIN);
	LL_GPIO_ResetOutputPin(H_P12_PORT, H_P12_PIN);
	LL_GPIO_ResetOutputPin(H_P23_PORT, H_P23_PIN);
	LL_GPIO_ResetOutputPin(IN_P2G_PORT, IN_P2G_PIN);
	LL_GPIO_ResetOutputPin(IN_P3G_PORT, IN_P3G_PIN);

	is_relay_on[0] = false;
	is_relay_on[1] = false;
	is_relay_on[2] = false;
	is_relay_on[3] = false;
	is_relay_on[4] = false;
	is_relay_on[5] = false;
}