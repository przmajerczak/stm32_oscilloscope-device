/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

#define SAMPLES_PER_DATA_TRANSFER 12000
#define USB_OUTPUT_BUFFER_SIZE 24100

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t usb_output_buffer[USB_OUTPUT_BUFFER_SIZE];

uint32_t dma_buffer[SAMPLES_PER_DATA_TRANSFER];
uint32_t adc_data[SAMPLES_PER_DATA_TRANSFER];

volatile uint16_t buffer_index = 0;
uint32_t measurements_period = 0;

int FUTURE_ADC_RANGE_FLAG = 1;
int new_data_needed_flag = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void write_next_byte_into_buffer(const uint16_t byte)
{
    usb_output_buffer[buffer_index++] = byte;
}

void write_next_four_byte_value_into_buffer(const uint32_t value)
{
    write_next_byte_into_buffer(value & 0xff);
    write_next_byte_into_buffer((value >> 8) & 0xff);
    write_next_byte_into_buffer((value >> 16) & 0xff);
    write_next_byte_into_buffer((value >> 24) & 0xff);
}

void write_next_two_byte_value_into_buffer(const uint16_t value)
{
    write_next_byte_into_buffer(value & 0xff);
    write_next_byte_into_buffer((value >> 8) & 0xff);
}

void write_end_sequence_into_buffer(void)
{
    write_next_byte_into_buffer(0xff);
    write_next_byte_into_buffer(0xff);
}

void switchAdcRangeIfNeeded()
{
    if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_11))
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);
        // FUTURE_ADC_RANGE_FLAG = 1;
    }
    else
    {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);
        // FUTURE_ADC_RANGE_FLAG = 0;
    }
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (new_data_needed_flag == 0)
    {
        measurements_period = TIM2->CNT;

        memcpy(adc_data, dma_buffer, SAMPLES_PER_DATA_TRANSFER);

        new_data_needed_flag = 1;
    }
    switchAdcRangeIfNeeded();

    TIM2->CNT = 0;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
    /* USER CODE BEGIN 1 */

    /* USER CODE END 1 */

    /* MCU Configuration--------------------------------------------------------*/

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    HAL_Init();

    /* USER CODE BEGIN Init */

    /* USER CODE END Init */

    /* Configure the system clock */
    SystemClock_Config();

    /* USER CODE BEGIN SysInit */

    /* USER CODE END SysInit */

    /* Initialize all configured peripherals */
    MX_GPIO_Init();
    MX_DMA_Init();
    MX_USB_DEVICE_Init();
    MX_ADC1_Init();
    MX_TIM2_Init();
    MX_TIM4_Init();
    MX_ADC2_Init();
    MX_ADC3_Init();
    /* USER CODE BEGIN 2 */

    HAL_TIM_Base_Start(&htim2);
    HAL_TIM_Base_Start_IT(&htim4); // Timer4 ticks every 1 us

    HAL_ADC_Start(&hadc3);
    HAL_ADC_Start(&hadc2);
    HAL_ADCEx_MultiModeStart_DMA(&hadc1, dma_buffer, SAMPLES_PER_DATA_TRANSFER);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
        if (new_data_needed_flag == 1)
        {
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 1);
            for (uint16_t sample_idx = 0; sample_idx < SAMPLES_PER_DATA_TRANSFER; sample_idx += 3)
            {
                write_next_two_byte_value_into_buffer(adc_data[sample_idx] & 0xffff);
                write_next_two_byte_value_into_buffer((adc_data[sample_idx] >> 16) & 0xffff);

                write_next_two_byte_value_into_buffer(adc_data[sample_idx + 1] & 0xffff);
                // write_next_two_byte_value_into_buffer((adc_data[sample_idx + 1] >> 16) & 0xffff);

                // write_next_two_byte_value_into_buffer(adc_data[sample_idx + 2] & 0xffff);
                // write_next_two_byte_value_into_buffer((adc_data[sample_idx + 2] >> 16) & 0xffff);
            }

            write_next_four_byte_value_into_buffer(measurements_period);
            write_end_sequence_into_buffer();

            CDC_Transmit_FS(usb_output_buffer, 2 * SAMPLES_PER_DATA_TRANSFER + 4 + 2);

            buffer_index = 0;
            HAL_Delay(30);
            HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, 0);
            new_data_needed_flag = 0;
        }
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
    }
    /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    /** Configure the main internal regulator output voltage
     */
    __HAL_RCC_PWR_CLK_ENABLE();
    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

    /** Initializes the RCC Oscillators according to the specified parameters
     * in the RCC_OscInitTypeDef structure.
     */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLM = 4;
    RCC_OscInitStruct.PLL.PLLN = 168;
    RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
    RCC_OscInitStruct.PLL.PLLQ = 7;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
     */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
    {
        Error_Handler();
    }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    __disable_irq();
    while (1)
    {
    }
    /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
    /* USER CODE BEGIN 6 */
    /* User can add his own implementation to report the file name and line
       number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
       line) */
    /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
