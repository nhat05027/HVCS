#ifndef APP_H_
#define APP_H_

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "stm32f030xc.h"

#include "board.h"

// USER DRIVER //
#include "scheduler.h"

// INCLUDE TASK //
#include "cmd_line_task.h"
#include "adc_task.h"
#include "charge_task.h"
#include "pwm_out_task.h"
#include "command.h"

// INCLUDE LIB //
#include "uart.h"
void App_Main(void);

#endif /* APP_H_ */
