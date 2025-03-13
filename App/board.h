#ifndef BOARD_H_
#define BOARD_H_

/*********************ADC***********************/
#define ADC_FEEDBACK_HANDLE     ADC1
#define ADC_FEEDBACK_IRQ        ADC1_IRQn

#define ADC_CHANNEL_COUNT       2
#define ADC_VOLTAGE_CHANNEL        LL_ADC_CHANNEL_0
#define ADC_CURRENT_CHANNEL        LL_ADC_CHANNEL_1
/***********************************************/

/*********************FLYBACK*******************/
#define FLYBACK_EN_PORT        GPIOB
#define FLYBACK_EN_PIN         LL_GPIO_PIN_1
/***********************************************/

/**************SWITCHING CAPACITOR**************/
#define H_S12_PORT             GPIOB
#define H_S12_PIN              LL_GPIO_PIN_0

#define L_S12_PORT             GPIOB
#define L_S12_PIN              LL_GPIO_PIN_2
//--------------------------------------------//
#define H_S23_PORT             GPIOB
#define H_S23_PIN              LL_GPIO_PIN_5

#define L_S23_PORT             GPIOB
#define L_S23_PIN              LL_GPIO_PIN_4
//--------------------------------------------//
#define H_P12_PORT             GPIOA
#define H_P12_PIN              LL_GPIO_PIN_7

#define L_P12_PORT             GPIOA
#define L_P12_PIN              LL_GPIO_PIN_6
//--------------------------------------------//
#define H_P23_PORT             GPIOA
#define H_P23_PIN              LL_GPIO_PIN_5

#define L_P23_PORT             GPIOA
#define L_P23_PIN              LL_GPIO_PIN_4
//--------------------------------------------//
#define IN_P2G_PORT            GPIOB
#define IN_P2G_PIN             LL_GPIO_PIN_3

#define IN_P3G_PORT            GPIOA
#define IN_P3G_PIN             LL_GPIO_PIN_15
/***********************************************/

/*************HALF-BRIDGE OUTPUT****************/
#define HALFBRIDGE_PWM_HANDLE   TIM17
#define HALFBRIDGE_PWM_CHANNEL  LL_TIM_CHANNEL_CH1

#define HB_IN_PORT              GPIOA
#define HB_IN_PIN               LL_GPIO_PIN_3

#define HB_SD_PORT              GPIOA
#define HB_SD_PIN               LL_GPIO_PIN_2
/***********************************************/

/************VOLTAGE MEASUREMENT SW*************/
#define V_DIV1_PORT             GPIOB
#define v_DIV1_PIN              LL_GPIO_PIN_8

#define V_DIV2_PORT             GPIOB
#define v_DIV2_PIN              LL_GPIO_PIN_7

#define V_DIV3_PORT             GPIOB
#define v_DIV3_PIN              LL_GPIO_PIN_6

#define V_DIV4_PORT             GPIOB
#define v_DIV4_PIN              LL_GPIO_PIN_9
/***********************************************/

/*********************UART**********************/
#define RS232_UART_HANDLE       USART1
#define RS232_UART_IRQ          USART1_IRQn

#define DEBUG_UART_HANDLE       USART3
#define DEBUG_UART_IRQ          USART3_6_IRQn


/*******************DEBUG LED*******************/
#define DEBUG_LED_PORT          GPIOC
#define DEBUG_LED_PIN           LL_GPIO_PIN_13
/***********************************************/



#endif /* BOARD_H_ */