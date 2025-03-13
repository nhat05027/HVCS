// APP HEADER //
#include "app.h"
#include "stm32f0xx_ll_gpio.h"

static void Status_Led(void*);

#define         SCHEDULER_TASK_COUNT  5
uint32_t        g_ui32SchedulerNumTasks = SCHEDULER_TASK_COUNT;
tSchedulerTask 	g_psSchedulerTable[SCHEDULER_TASK_COUNT] =
                {
                    {
                        &ADC_Task,
                        (void *) 0,
                        20,                          //call every 248us
                        0,			                //count from start
                        true		                //is active
                    },
                    {
                        &Charge_Task,
                        (void *) 0,
                        20,                          //call every 248us
                        0,			                //count from start
                        true		                //is active
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
    while (1)
    {
        SchedulerRun();
    }
}

static void Status_Led(void*)
{
    LL_GPIO_TogglePin(DEBUG_LED_PORT, DEBUG_LED_PIN);
}