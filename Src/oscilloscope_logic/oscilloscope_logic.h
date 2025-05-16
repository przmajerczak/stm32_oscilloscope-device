#ifndef __OSCILLOSCOPE_LOGIC_H__
#define __OSCILLOSCOPE_LOGIC_H__

#include "adc_utils.h"
#include "defines.h"
#include "usb_transmission.h"

uint32_t adc_data[SAMPLES_PER_DATA_TRANSFER];
uint8_t channel_1_active_flag = 1;
uint8_t channel_2_active_flag = 1;
uint8_t number_of_active_channels = 0;
uint32_t adc_sample_time = ADC_SAMPLETIME_3CYCLES;
uint32_t measurements_period = 0;
uint32_t single_channel_mode_active_channel;

int data_ready = 0;

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    measurements_period = TIM2->CNT;
    HAL_ADCEx_MultiModeStop_DMA(&hadc1);
    data_ready = 1;
}

void readPinConfiguration()
{
    do
    {
        adc_sample_time = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_6) == GPIO_PIN_SET)
                              ? ADC_SAMPLETIME_3CYCLES
                              : ADC_SAMPLETIME_144CYCLES;
        channel_1_active_flag =
            (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6) == GPIO_PIN_RESET) ? 1 : 0;
        channel_2_active_flag =
            (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_13) == GPIO_PIN_RESET) ? 1 : 0;

        number_of_active_channels = channel_1_active_flag + channel_2_active_flag;

    } while (number_of_active_channels == 0);

    if (number_of_active_channels == 1)
    {
        single_channel_mode_active_channel =
            channel_1_active_flag ? ADC_CHANNEL_3 : ADC_CHANNEL_13;
    }
}

void decode_adc_data_into_usb_buffer()
{
    if (number_of_active_channels == 2)
    {
        if (channel_1_active_flag)
        {
            dualChannelMode_writeChannel1ToBuffer(adc_data, measurements_period);
        }

        if (channel_2_active_flag)
        {
            dualChannelMode_writeChannel2ToBuffer(adc_data, measurements_period);
        }
    }
    else if (number_of_active_channels == 1)
    {
        const uint16_t channelId = channel_1_active_flag ? CHANNEL_1 : CHANNEL_2;
        singleChannelMode_writeOnlyChannelToBuffer(
            adc_data, measurements_period, channelId);
    }
}

void reconfigure_and_restart_ADC()
{
    readPinConfiguration();
    startADC(adc_data, number_of_active_channels, adc_sample_time,
             single_channel_mode_active_channel);
}

void oscilloscope_init()
{
    HAL_TIM_Base_Start(&htim2);
    reconfigure_and_restart_ADC();
}

void oscilloscope_single_iteration()
{
    if (data_ready == 1)
    {
        data_ready = 0;

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 1);

        stopADC();

        decode_adc_data_into_usb_buffer();

        transmit_data_over_usb(number_of_active_channels);

        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, 0);
        HAL_Delay(70);

        reconfigure_and_restart_ADC();

        TIM2->CNT = 0;
    }
}

#endif
