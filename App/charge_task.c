/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

#include "stm32f0xx_ll_gpio.h"

#include "app.h"

#include "charge_task.h"

//#include "pwm.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* :::::::::: PID Task State ::::::::::::: */
typedef enum
{
	CHARGE_OFF_STATE,
    CHARGE_START_STATE,
} Charge_State_typedef;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static Charge_State_typedef Charge_State = CHARGE_OFF_STATE;

//static 	PWM_TypeDef Discharge_300V_PWM;
//static 	PWM_TypeDef	Discharge_50V_PWM;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
bool g_en_charge = false;
int g_set_voltage = 0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: Discharge Task Init ::::::::::::: */
void Charge_Task_Init(void)
{
    LL_GPIO_ResetOutputPin(FLYBACK_EN_PORT, FLYBACK_EN_PIN);
}

/* :::::::::: Discharge Task ::::::::::::: */
void Charge_Task(void*)
{
    switch (Charge_State)
    {
    case CHARGE_OFF_STATE:
        LL_GPIO_ResetOutputPin(FLYBACK_EN_PORT, FLYBACK_EN_PIN);

        if (g_en_charge == true)
        {
            Charge_State = CHARGE_START_STATE;
        }

        break;
    case CHARGE_START_STATE:
        if (g_en_charge == false)
        {
            LL_GPIO_ResetOutputPin(FLYBACK_EN_PORT, FLYBACK_EN_PIN);
            Charge_State = CHARGE_OFF_STATE;
        }
        if (g_Feedback[0]*(1/g_volt_prescaler)*0.68 <= g_set_voltage) LL_GPIO_SetOutputPin(FLYBACK_EN_PORT, FLYBACK_EN_PIN);
        else LL_GPIO_ResetOutputPin(FLYBACK_EN_PORT, FLYBACK_EN_PIN);
        
        break;
    default:
        break;
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */