/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Include~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
#include "adc_task.h"
#include "app.h"

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Defines ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* Low-pass filter settings */
#define LP_FILTER_SHIFT      3   /* Alpha = 1/(2^3) = 1/8. Increase shift for heavier filtering. */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Enum ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Struct ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Class ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Private Types ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
static uint8_t  ADC_channel_index = 0;
static uint16_t ADC_Value[ADC_CHANNEL_COUNT] = {0};
static uint16_t filtered_values[ADC_CHANNEL_COUNT] = {0};    // Filtered ADC values
static bool filter_initialized[ADC_CHANNEL_COUNT] = {false}; // Filter initialization flags

static bool is_ADC_read_completed       = false;
//static bool is_ADC_sequence_completed   = false;

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
uint16_t g_Feedback[ADC_CHANNEL_COUNT] = {0};

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Public Function ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* :::::::::: ADC Task Init :::::::: */
void ADC_Task_Init(uint32_t Sampling_Time)
{
    ADC_Init(ADC_FEEDBACK_HANDLE, Sampling_Time);

    LL_ADC_REG_SetSequencerChannels(ADC_FEEDBACK_HANDLE, ADC_VOLTAGE_CHANNEL | ADC_CURRENT_CHANNEL );
    LL_ADC_REG_SetSequencerDiscont(ADC_FEEDBACK_HANDLE, LL_ADC_REG_SEQ_DISCONT_1RANK);

    LL_ADC_EnableIT_EOC(ADC_FEEDBACK_HANDLE);

    LL_ADC_REG_StartConversion(ADC_FEEDBACK_HANDLE);
}

/* :::::::::: ADC Task ::::::::::::: */
//TODO: Áp dụng ring buffer và mạch lọc cho ADC.
void ADC_Task(void*)
{
    if (is_ADC_read_completed == true)
    {
        is_ADC_read_completed = false;

        LL_ADC_REG_StartConversion(ADC_FEEDBACK_HANDLE);
    }
}

/* :::::::::: ADC Interupt Handler ::::::::::::: */
// void ADC_Task_IRQHandler(void)
// {
//     if(LL_ADC_IsActiveFlag_EOC(ADC_FEEDBACK_HANDLE) == true)
//     {
//         is_ADC_read_completed = true;
//         LL_ADC_ClearFlag_EOC(ADC_FEEDBACK_HANDLE);

//         ADC_Value[ADC_channel_index] = LL_ADC_REG_ReadConversionData12(ADC_FEEDBACK_HANDLE);
//         g_Feedback[ADC_channel_index] = 
//             __LL_ADC_CALC_DATA_TO_VOLTAGE(3300, ADC_Value[ADC_channel_index], LL_ADC_RESOLUTION_12B);

//         if(ADC_channel_index >= 1)
//         {
//             ADC_channel_index = 0;
//         }
//         else
//         {
//             ADC_channel_index = ADC_channel_index + 1;
//         }  
//     }
// }

void ADC_Task_IRQHandler(void)
{
    if(LL_ADC_IsActiveFlag_EOC(ADC_FEEDBACK_HANDLE))
    {
        is_ADC_read_completed = true;
        LL_ADC_ClearFlag_EOC(ADC_FEEDBACK_HANDLE);

        // Read raw ADC value
        ADC_Value[ADC_channel_index] = LL_ADC_REG_ReadConversionData12(ADC_FEEDBACK_HANDLE);
        
        // Convert to voltage (mV)
        uint16_t new_sample = __LL_ADC_CALC_DATA_TO_VOLTAGE(
            3300, 
            ADC_Value[ADC_channel_index], 
            LL_ADC_RESOLUTION_12B
        );

        // Apply low-pass filter
        if (!filter_initialized[ADC_channel_index])
        {
            // Initialize filter with first sample
            filtered_values[ADC_channel_index] = new_sample;
            filter_initialized[ADC_channel_index] = true;
        }
        else
        {
            // IIR filter: filtered = (new_sample + 7*prev_filtered) / 8
            uint32_t filtered = new_sample + 7 * filtered_values[ADC_channel_index];
            filtered >>= LP_FILTER_SHIFT; // Efficient division by 8
            filtered_values[ADC_channel_index] = (uint16_t)filtered;
        }

        // Update global feedback with filtered value
        g_Feedback[ADC_channel_index] = filtered_values[ADC_channel_index];

        // Cycle channel index
        ADC_channel_index = (ADC_channel_index >= 1) ? 0 : (ADC_channel_index + 1);
    }
}

/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Private Prototype ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ End of the program ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */
