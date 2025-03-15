/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include <stdint.h>
#include <stdbool.h>

#include "app.h"

#include "stm32f0xx_ll_gpio.h"

#include "pwm_out_task.h"

#include "pwm.h"
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// PWM_TypeDef 	Halfbridge_Switching_PWM =
// {
//     .TIMx       =   HALFBRIDGE_PWM_HANDLE,
//     .Channel    =   HALFBRIDGE_PWM_CHANNEL,
//     .Prescaler  =   0,
//     .Mode       =   LL_TIM_OCMODE_PWM1,
//     .Polarity   =   LL_TIM_OCPOLARITY_HIGH,
//     .Duty       =   0,
//     .Freq       =   0,
// };

bool		HB_on = false;
uint16_t 	HB_freq;
uint8_t 	HB_duty;
uint8_t 	HB_cycle=0;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// static inline void HB_Set_Duty(PWM_TypeDef *PWMx, uint32_t _Duty);
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/* :::::::::: PID Init ::::::::::::: */
void HB_Task_Init(void)
{

	// PWM_Set_Freq(&Halfbridge_Switching_PWM, 60000);
	// LL_TIM_DisableIT_UPDATE(Halfbridge_Switching_PWM.TIMx);
    // PWM_Enable(&Halfbridge_Switching_PWM);
	LL_GPIO_SetOutputPin(HB_SD_PORT, HB_SD_PIN);
}

void HB_Task(void*)
{
	if (!HB_on)
	{
        // PWM_Set_Freq(&Halfbridge_Switching_PWM, HB_freq);
		// HB_Set_Duty(&Halfbridge_Switching_PWM, 0);
		LL_GPIO_SetOutputPin(HB_SD_PORT, HB_SD_PIN);
	}
	else if (HB_on)
	{
        // if (HB_cycle > 0)
        // {
        //     HB_Set_Duty(&Halfbridge_Switching_PWM, HB_duty);
        //     HB_cycle -= 1;
        // }
		LL_GPIO_ResetOutputPin(HB_SD_PORT, HB_SD_PIN);
        LL_GPIO_TogglePin(HB_IN_PORT, HB_IN_PIN);
	}

}


/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Functions ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
// static inline void HB_Set_Duty(PWM_TypeDef *PWMx, uint32_t _Duty)
// {
// 	// Limit the duty to 100
//     if (_Duty > 100)
//       return;

//     // Set PWM DUTY for channel 1
//     PWMx->Duty = (PWMx->Freq * (_Duty / 100.0));
//     switch (PWMx->Channel)
//     {
//     case LL_TIM_CHANNEL_CH1:
//         LL_TIM_OC_SetCompareCH1(PWMx->TIMx, PWMx->Duty);
//         break;
//     case LL_TIM_CHANNEL_CH2:
//         LL_TIM_OC_SetCompareCH2(PWMx->TIMx, PWMx->Duty);
//         break;
//     case LL_TIM_CHANNEL_CH3:
//         LL_TIM_OC_SetCompareCH3(PWMx->TIMx, PWMx->Duty);
//         break;
//     case LL_TIM_CHANNEL_CH4:
//         LL_TIM_OC_SetCompareCH4(PWMx->TIMx, PWMx->Duty);
//         break;

//     default:
//         break;
//     }
// }
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */