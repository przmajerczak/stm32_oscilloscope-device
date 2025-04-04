#ifndef __ADC_UTILS_H__
#define __ADC_UTILS_H__

#include "adc.h"
#include "defines.h"

void configChannels(ADC_HandleTypeDef *hadc,
                    const uint32_t number_of_active_channels,
                    const uint32_t adc_sample_time,
                    const uint32_t single_channel_mode_active_channel)
{
    ADC_ChannelConfTypeDef sConfig = {0};

    if (number_of_active_channels == 2)
    {
        sConfig.Channel = ADC_CHANNEL_3;
        sConfig.Rank = 1;
        sConfig.SamplingTime = adc_sample_time;
        HAL_ADC_ConfigChannel(hadc, &sConfig);

        sConfig.Channel = ADC_CHANNEL_13;
        sConfig.Rank = 2;
        HAL_ADC_ConfigChannel(hadc, &sConfig);
    }
    else if (number_of_active_channels == 1)
    {
        sConfig.Channel = single_channel_mode_active_channel;
        sConfig.Rank = 1;
        sConfig.SamplingTime = adc_sample_time;
        HAL_ADC_ConfigChannel(hadc, &sConfig);
    }
}

void InitADC1(const uint32_t number_of_active_channels,
              const uint32_t adc_sample_time,
              const uint32_t single_channel_mode_active_channel)
{
    hadc1.Instance = ADC1;
    hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc1.Init.Resolution = ADC_RESOLUTION_12B;
    hadc1.Init.ScanConvMode = ENABLE;
    hadc1.Init.ContinuousConvMode = ENABLE;
    hadc1.Init.DiscontinuousConvMode = DISABLE;
    hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
    hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
    hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc1.Init.NbrOfConversion = number_of_active_channels;
    hadc1.Init.DMAContinuousRequests = ENABLE;
    hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    HAL_ADC_Init(&hadc1);

    ADC_MultiModeTypeDef multimode = {0};
    multimode.Mode = ADC_TRIPLEMODE_INTERL;
    multimode.DMAAccessMode = ADC_DMAACCESSMODE_2;
    multimode.TwoSamplingDelay = ADC_TWOSAMPLINGDELAY_5CYCLES;
    HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode);

    configChannels(&hadc1, number_of_active_channels, adc_sample_time,
                   single_channel_mode_active_channel);
}
void InitADC2(const uint32_t number_of_active_channels,
              const uint32_t adc_sample_time,
              const uint32_t single_channel_mode_active_channel)
{
    hadc2.Instance = ADC2;
    hadc2.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc2.Init.Resolution = ADC_RESOLUTION_12B;
    hadc2.Init.ScanConvMode = ENABLE;
    hadc2.Init.ContinuousConvMode = ENABLE;
    hadc2.Init.DiscontinuousConvMode = DISABLE;
    hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc2.Init.NbrOfConversion = number_of_active_channels;
    hadc2.Init.DMAContinuousRequests = DISABLE;
    hadc2.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    HAL_ADC_Init(&hadc2);

    configChannels(&hadc2, number_of_active_channels, adc_sample_time,
                   single_channel_mode_active_channel);
}
void InitADC3(const uint32_t number_of_active_channels,
              const uint32_t adc_sample_time,
              const uint32_t single_channel_mode_active_channel)
{
    hadc3.Instance = ADC3;
    hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
    hadc3.Init.Resolution = ADC_RESOLUTION_12B;
    hadc3.Init.ScanConvMode = ENABLE;
    hadc3.Init.ContinuousConvMode = ENABLE;
    hadc3.Init.DiscontinuousConvMode = DISABLE;
    hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
    hadc3.Init.NbrOfConversion = number_of_active_channels;
    hadc3.Init.DMAContinuousRequests = DISABLE;
    hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
    HAL_ADC_Init(&hadc3);

    configChannels(&hadc3, number_of_active_channels, adc_sample_time,
                   single_channel_mode_active_channel);
}

void startADC(uint32_t adc_data_buffer[],
              const uint32_t number_of_active_channels,
              const uint32_t adc_sample_time,
              const uint32_t single_channel_mode_active_channel)
{
    InitADC1(number_of_active_channels, adc_sample_time,
             single_channel_mode_active_channel);
    InitADC2(number_of_active_channels, adc_sample_time,
             single_channel_mode_active_channel);
    InitADC3(number_of_active_channels, adc_sample_time,
             single_channel_mode_active_channel);

    HAL_ADC_Start(&hadc3);
    HAL_ADC_Start(&hadc2);
    HAL_ADCEx_MultiModeStart_DMA(&hadc1, adc_data_buffer,
                                 number_of_active_channels *
                                     SAMPLES_PER_DATA_TRANSFER / 2);
}

void stopADC()
{
    HAL_ADC_Stop(&hadc1);
    HAL_ADC_Stop(&hadc2);
    HAL_ADC_Stop(&hadc3);

    HAL_ADC_DeInit(&hadc1);
    HAL_ADC_DeInit(&hadc2);
    HAL_ADC_DeInit(&hadc3);
}

#endif
