// APP HEADER //
#include "app.h"
#include "stm32f0xx_ll_gpio.h"

GPIO_TypeDef *RELAY_BOOT_PORT[6] = {L_S12_PORT, L_S23_PORT, L_P12_PORT, L_P23_PORT};
GPIO_TypeDef *RELAY_BOOT_PIN[6] = {L_S12_PIN, L_S23_PIN, L_P12_PIN, L_P23_PIN};
static void Status_Led(void*);
static void Bootstrap_Relay(void*);

#define         SCHEDULER_TASK_COUNT  7
uint32_t        g_ui32SchedulerNumTasks = SCHEDULER_TASK_COUNT;
tSchedulerTask 	g_psSchedulerTable[SCHEDULER_TASK_COUNT] =
                {
                    {
                        &ADC_Task,
                        (void *) 0,
                        5,                          //call every 248us
                        0,			                //count from start
                        true		                //is active
                    },
                    {
                        &Charge_Task,
                        (void *) 0,
                        5,                          //call every 248us
                        0,			                //count from start
                        true		                //is active
                    },
                    {
                        &HB_Task,
                        (void *) 0,
                        1000,                          //call every 248us
                        0,			                //count from start
                        true		                //is active
                    },
                    {
                        &Bootstrap_Relay,
                        (void *) 0,
                        50,                      //call every 1ms
                        0,                          //count from start
                        true                        //is active
                    },
                    {
                        &Status_Led,
                        (void *) 0,
                        10000,                      //call every 1000ms
                        0,                          //count from start
                        true                        //is active
                    },
                    {
                        &RS232_CMD_Line_Task,
                        (void *) 0,
                        50,                         //call every 500us
                        0,                          //count from start
                        true                        //is active
                    },
                    {
                        &DEBUG_CMD_Line_Task,
                        (void *) 0,
                        1000,                         //call every 500us
                        0,                          //count from start
                        true                        //is active
                    },
                };

void App_Main(void)
{   
    SchedulerInit(10000);

    ADC_Task_Init(LL_ADC_SAMPLINGTIME_7CYCLES_5);
    CMD_Line_Task_Init();
    Charge_Task_Init();
    HB_Task_Init();
    while (1)
    {
        SchedulerRun();
    }
}

static void Status_Led(void*)
{
    LL_GPIO_TogglePin(DEBUG_LED_PORT, DEBUG_LED_PIN);
}
static void Bootstrap_Relay(void*)
{   
    for (int i = 0; i < 4; i++)
    {
        if (is_relay_on[i]) LL_GPIO_TogglePin(RELAY_BOOT_PORT[i], RELAY_BOOT_PIN[i]);
        else LL_GPIO_ResetOutputPin(RELAY_BOOT_PORT[i], RELAY_BOOT_PIN[i]);
    }
}
