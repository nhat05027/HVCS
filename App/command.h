#ifndef COMMAND_H_
#define COMMAND_H_

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "stm32f0xx_ll_gpio.h"
#include <stdint.h>
#include "app.h"
#include "cmd_line.h"
#include "uart.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Include Task ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "cmd_line_task.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
extern bool is_relay_on[6];
extern int g_volt_prescaler;
GPIO_TypeDef *RELAY_PORT[6] = {H_S12_PORT, H_S23_PORT, H_P12_PORT, H_P23_PORT, IN_P2G_PORT, IN_P3G_PORT};
GPIO_TypeDef *RELAY_PIN[6] = {H_S12_PIN, H_S23_PIN, H_P12_PIN, H_P23_PIN, IN_P2G_PIN, IN_P3G_PIN};
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ::::::::::: Charge Command ::::::::: */
int CMD_EN_CHARGE(int argc, char *argv[]);
int CMD_SET_VOLTAGE(int argc, char *argv[]);
int CMD_GET_STATUS_CHARGE(int argc, char *argv[]);

/* ::::::: Switching Cap Command :::::: */
int CMD_PARALLEL_CAP(int argc, char *argv[]);
int CMD_SERIAL_CAP(int argc, char *argv[]);
int CMD_GET_STATUS_CAP(int argc, char *argv[]);

/* ::::::::: Relay Cap Command :::::::: */
int CMD_CONTROL_RELAY(int argc, char *argv[]);
int CMD_ALLOFF_RELAY(int argc, char *argv[]);
int CMD_LOCK_RELAY(int argc, char *argv[]);
int CMD_UNLOCK_RELAY(int argc, char *argv[]);
int CMD_GET_STATUS_RELAY(int argc, char *argv[]);

/* ::::::::: Half Bridge Command :::::::: */
int CMD_SET_PULSE_HB(int argc, char *argv[]);
int CMD_PULSE_OUT_HB(int argc, char *argv[]);

/* ::::::::::::: VOM Command :::::::::::: */
int CMD_SET_PRESCALE_VOLT(int argc, char *argv[]);
int CMD_GET_VOLT(int argc, char *argv[]);
int CMD_GET_CURRENT(int argc, char *argv[]);

/* :::::::::: Ultility Command :::::::: */
int CMD_MONITOR_DEBUG(int argc, char *argv[]);
int CMD_RESET(int argc, char *argv[]);
int CMD_HELP(int argc, char *argv[]);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

#endif /* COMMAND_H_ */
