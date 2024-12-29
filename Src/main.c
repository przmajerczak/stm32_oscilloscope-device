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

#define USB_OUTPUT_BUFFER_SIZE 2000

const uint16_t SAMPLES_PER_DATA_TRANSFER = 3;

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint8_t usb_output_buffer[USB_OUTPUT_BUFFER_SIZE];
volatile uint16_t buffer_index = 0;

const uint16_t sin_values[800] = {
    2128, 2209, 2289, 2368, 2448, 2526, 2604, 2681, 2757, 2832, 2905, 2978,
    3049, 3118, 3186, 3252, 3316, 3378, 3438, 3496, 3552, 3605, 3656, 3705,
    3751, 3794, 3835, 3873, 3908, 3940, 3969, 3996, 4019, 4039, 4057, 4071,
    4082, 4090, 4094, 4096, 4094, 4090, 4082, 4071, 4057, 4039, 4019, 3996,
    3969, 3940, 3908, 3873, 3835, 3794, 3751, 3705, 3656, 3605, 3552, 3496,
    3438, 3378, 3316, 3252, 3186, 3118, 3049, 2978, 2905, 2832, 2757, 2681,
    2604, 2526, 2448, 2368, 2289, 2209, 2128, 2048, 1968, 1887, 1807, 1728,
    1648, 1570, 1492, 1415, 1339, 1264, 1191, 1118, 1047, 978, 910, 844,
    780, 718, 658, 600, 544, 491, 440, 391, 345, 302, 261, 223,
    188, 156, 127, 100, 77, 57, 39, 25, 14, 6, 2, 0,
    2, 6, 14, 25, 39, 57, 77, 100, 127, 156, 188, 223,
    261, 302, 345, 391, 440, 491, 544, 600, 658, 718, 780, 844,
    910, 978, 1047, 1118, 1191, 1264, 1339, 1415, 1492, 1570, 1648, 1728,
    1807, 1887, 1968, 2048, 2128, 2209, 2289, 2368, 2448, 2526, 2604, 2681,
    2757, 2832, 2905, 2978, 3049, 3118, 3186, 3252, 3316, 3378, 3438, 3496,
    3552, 3605, 3656, 3705, 3751, 3794, 3835, 3873, 3908, 3940, 3969, 3996,
    4019, 4039, 4057, 4071, 4082, 4090, 4094, 4096, 4094, 4090, 4082, 4071,
    4057, 4039, 4019, 3996, 3969, 3940, 3908, 3873, 3835, 3794, 3751, 3705,
    3656, 3605, 3552, 3496, 3438, 3378, 3316, 3252, 3186, 3118, 3049, 2978,
    2905, 2832, 2757, 2681, 2604, 2526, 2448, 2368, 2289, 2209, 2128, 2048,
    1968, 1887, 1807, 1728, 1648, 1570, 1492, 1415, 1339, 1264, 1191, 1118,
    1047, 978, 910, 844, 780, 718, 658, 600, 544, 491, 440, 391,
    345, 302, 261, 223, 188, 156, 127, 100, 77, 57, 39, 25,
    14, 6, 2, 0, 2, 6, 14, 25, 39, 57, 77, 100,
    127, 156, 188, 223, 261, 302, 345, 391, 440, 491, 544, 600,
    658, 718, 780, 844, 910, 978, 1047, 1118, 1191, 1264, 1339, 1415,
    1492, 1570, 1648, 1728, 1807, 1887, 1968, 2048, 2128, 2209, 2289, 2368,
    2448, 2526, 2604, 2681, 2757, 2832, 2905, 2978, 3049, 3118, 3186, 3252,
    3316, 3378, 3438, 3496, 3552, 3605, 3656, 3705, 3751, 3794, 3835, 3873,
    3908, 3940, 3969, 3996, 4019, 4039, 4057, 4071, 4082, 4090, 4094, 4096,
    4094, 4090, 4082, 4071, 4057, 4039, 4019, 3996, 3969, 3940, 3908, 3873,
    3835, 3794, 3751, 3705, 3656, 3605, 3552, 3496, 3438, 3378, 3316, 3252,
    3186, 3118, 3049, 2978, 2905, 2832, 2757, 2681, 2604, 2526, 2448, 2368,
    2289, 2209, 2128, 2048, 1968, 1887, 1807, 1728, 1648, 1570, 1492, 1415,
    1339, 1264, 1191, 1118, 1047, 978, 910, 844, 780, 718, 658, 600,
    544, 491, 440, 391, 345, 302, 261, 223, 188, 156, 127, 100,
    77, 57, 39, 25, 14, 6, 2, 0, 2, 6, 14, 25,
    39, 57, 77, 100, 127, 156, 188, 223, 261, 302, 345, 391,
    440, 491, 544, 600, 658, 718, 780, 844, 910, 978, 1047, 1118,
    1191, 1264, 1339, 1415, 1492, 1570, 1648, 1728, 1807, 1887, 1968, 2048,
    2128, 2209, 2289, 2368, 2448, 2526, 2604, 2681, 2757, 2832, 2905, 2978,
    3049, 3118, 3186, 3252, 3316, 3378, 3438, 3496, 3552, 3605, 3656, 3705,
    3751, 3794, 3835, 3873, 3908, 3940, 3969, 3996, 4019, 4039, 4057, 4071,
    4082, 4090, 4094, 4096, 4094, 4090, 4082, 4071, 4057, 4039, 4019, 3996,
    3969, 3940, 3908, 3873, 3835, 3794, 3751, 3705, 3656, 3605, 3552, 3496,
    3438, 3378, 3316, 3252, 3186, 3118, 3049, 2978, 2905, 2832, 2757, 2681,
    2604, 2526, 2448, 2368, 2289, 2209, 2128, 2048, 1968, 1887, 1807, 1728,
    1648, 1570, 1492, 1415, 1339, 1264, 1191, 1118, 1047, 978, 910, 844,
    780, 718, 658, 600, 544, 491, 440, 391, 345, 302, 261, 223,
    188, 156, 127, 100, 77, 57, 39, 25, 14, 6, 2, 0,
    2, 6, 14, 25, 39, 57, 77, 100, 127, 156, 188, 223,
    261, 302, 345, 391, 440, 491, 544, 600, 658, 718, 780, 844,
    910, 978, 1047, 1118, 1191, 1264, 1339, 1415, 1492, 1570, 1648, 1728,
    1807, 1887, 1968, 2048, 2128, 2209, 2289, 2368, 2448, 2526, 2604, 2681,
    2757, 2832, 2905, 2978, 3049, 3118, 3186, 3252, 3316, 3378, 3438, 3496,
    3552, 3605, 3656, 3705, 3751, 3794, 3835, 3873, 3908, 3940, 3969, 3996,
    4019, 4039, 4057, 4071, 4082, 4090, 4094, 4096, 4094, 4090, 4082, 4071,
    4057, 4039, 4019, 3996, 3969, 3940, 3908, 3873, 3835, 3794, 3751, 3705,
    3656, 3605, 3552, 3496, 3438, 3378, 3316, 3252, 3186, 3118, 3049, 2978,
    2905, 2832, 2757, 2681, 2604, 2526, 2448, 2368, 2289, 2209, 2128, 2048,
    1968, 1887, 1807, 1728, 1648, 1570, 1492, 1415, 1339, 1264, 1191, 1118,
    1047, 978, 910, 844, 780, 718, 658, 600, 544, 491, 440, 391,
    345, 302, 261, 223, 188, 156, 127, 100, 77, 57, 39, 25,
    14, 6, 2, 0, 2, 6, 14, 25, 39, 57, 77, 100,
    127, 156, 188, 223, 261, 302, 345, 391, 440, 491, 544, 600,
    658, 718, 780, 844, 910, 978, 1047, 1118, 1191, 1264, 1339, 1415,
    1492, 1570, 1648, 1728, 1807, 1887, 1968, 2048};

const uint16_t triangle_values[800] = {
    41, 61, 82, 102, 123, 143, 164, 184, 205, 225, 246, 266,
    287, 307, 328, 348, 369, 389, 410, 430, 451, 471, 492, 512,
    532, 553, 573, 594, 614, 635, 655, 676, 696, 717, 737, 758,
    778, 799, 819, 840, 860, 881, 901, 922, 942, 963, 983, 1004,
    1024, 1044, 1065, 1085, 1106, 1126, 1147, 1167, 1188, 1208, 1229, 1249,
    1270, 1290, 1311, 1331, 1352, 1372, 1393, 1413, 1434, 1454, 1475, 1495,
    1516, 1536, 1556, 1577, 1597, 1618, 1638, 1659, 1679, 1700, 1720, 1741,
    1761, 1782, 1802, 1823, 1843, 1864, 1884, 1905, 1925, 1946, 1966, 1987,
    2007, 2028, 2048, 2068, 2089, 2109, 2130, 2150, 2171, 2191, 2212, 2232,
    2253, 2273, 2294, 2314, 2335, 2355, 2376, 2396, 2417, 2437, 2458, 2478,
    2499, 2519, 2540, 2560, 2580, 2601, 2621, 2642, 2662, 2683, 2703, 2724,
    2744, 2765, 2785, 2806, 2826, 2847, 2867, 2888, 2908, 2929, 2949, 2970,
    2990, 3011, 3031, 3052, 3072, 3092, 3113, 3133, 3154, 3174, 3195, 3215,
    3236, 3256, 3277, 3297, 3318, 3338, 3359, 3379, 3400, 3420, 3441, 3461,
    3482, 3502, 3523, 3543, 3564, 3584, 3604, 3625, 3645, 3666, 3686, 3707,
    3727, 3748, 3768, 3789, 3809, 3830, 3850, 3871, 3891, 3912, 3932, 3953,
    3973, 3994, 4014, 4035, 4055, 4076, 4096, 0, 20, 41, 61, 82,
    102, 123, 143, 164, 184, 205, 225, 246, 266, 287, 307, 328,
    348, 369, 389, 410, 430, 451, 471, 492, 512, 532, 553, 573,
    594, 614, 635, 655, 676, 696, 717, 737, 758, 778, 799, 819,
    840, 860, 881, 901, 922, 942, 963, 983, 1004, 1024, 1044, 1065,
    1085, 1106, 1126, 1147, 1167, 1188, 1208, 1229, 1249, 1270, 1290, 1311,
    1331, 1352, 1372, 1393, 1413, 1434, 1454, 1475, 1495, 1516, 1536, 1556,
    1577, 1597, 1618, 1638, 1659, 1679, 1700, 1720, 1741, 1761, 1782, 1802,
    1823, 1843, 1864, 1884, 1905, 1925, 1946, 1966, 1987, 2007, 2028, 2048,
    2068, 2089, 2109, 2130, 2150, 2171, 2191, 2212, 2232, 2253, 2273, 2294,
    2314, 2335, 2355, 2376, 2396, 2417, 2437, 2458, 2478, 2499, 2519, 2540,
    2560, 2580, 2601, 2621, 2642, 2662, 2683, 2703, 2724, 2744, 2765, 2785,
    2806, 2826, 2847, 2867, 2888, 2908, 2929, 2949, 2970, 2990, 3011, 3031,
    3052, 3072, 3092, 3113, 3133, 3154, 3174, 3195, 3215, 3236, 3256, 3277,
    3297, 3318, 3338, 3359, 3379, 3400, 3420, 3441, 3461, 3482, 3502, 3523,
    3543, 3564, 3584, 3604, 3625, 3645, 3666, 3686, 3707, 3727, 3748, 3768,
    3789, 3809, 3830, 3850, 3871, 3891, 3912, 3932, 3953, 3973, 3994, 4014,
    4035, 4055, 4076, 4096, 0, 20, 41, 61, 82, 102, 123, 143,
    164, 184, 205, 225, 246, 266, 287, 307, 328, 348, 369, 389,
    410, 430, 451, 471, 492, 512, 532, 553, 573, 594, 614, 635,
    655, 676, 696, 717, 737, 758, 778, 799, 819, 840, 860, 881,
    901, 922, 942, 963, 983, 1004, 1024, 1044, 1065, 1085, 1106, 1126,
    1147, 1167, 1188, 1208, 1229, 1249, 1270, 1290, 1311, 1331, 1352, 1372,
    1393, 1413, 1434, 1454, 1475, 1495, 1516, 1536, 1556, 1577, 1597, 1618,
    1638, 1659, 1679, 1700, 1720, 1741, 1761, 1782, 1802, 1823, 1843, 1864,
    1884, 1905, 1925, 1946, 1966, 1987, 2007, 2028, 2048, 2068, 2089, 2109,
    2130, 2150, 2171, 2191, 2212, 2232, 2253, 2273, 2294, 2314, 2335, 2355,
    2376, 2396, 2417, 2437, 2458, 2478, 2499, 2519, 2540, 2560, 2580, 2601,
    2621, 2642, 2662, 2683, 2703, 2724, 2744, 2765, 2785, 2806, 2826, 2847,
    2867, 2888, 2908, 2929, 2949, 2970, 2990, 3011, 3031, 3052, 3072, 3092,
    3113, 3133, 3154, 3174, 3195, 3215, 3236, 3256, 3277, 3297, 3318, 3338,
    3359, 3379, 3400, 3420, 3441, 3461, 3482, 3502, 3523, 3543, 3564, 3584,
    3604, 3625, 3645, 3666, 3686, 3707, 3727, 3748, 3768, 3789, 3809, 3830,
    3850, 3871, 3891, 3912, 3932, 3953, 3973, 3994, 4014, 4035, 4055, 4076,
    4096, 0, 20, 41, 61, 82, 102, 123, 143, 164, 184, 205,
    225, 246, 266, 287, 307, 328, 348, 369, 389, 410, 430, 451,
    471, 492, 512, 532, 553, 573, 594, 614, 635, 655, 676, 696,
    717, 737, 758, 778, 799, 819, 840, 860, 881, 901, 922, 942,
    963, 983, 1004, 1024, 1044, 1065, 1085, 1106, 1126, 1147, 1167, 1188,
    1208, 1229, 1249, 1270, 1290, 1311, 1331, 1352, 1372, 1393, 1413, 1434,
    1454, 1475, 1495, 1516, 1536, 1556, 1577, 1597, 1618, 1638, 1659, 1679,
    1700, 1720, 1741, 1761, 1782, 1802, 1823, 1843, 1864, 1884, 1905, 1925,
    1946, 1966, 1987, 2007, 2028, 2048, 2068, 2089, 2109, 2130, 2150, 2171,
    2191, 2212, 2232, 2253, 2273, 2294, 2314, 2335, 2355, 2376, 2396, 2417,
    2437, 2458, 2478, 2499, 2519, 2540, 2560, 2580, 2601, 2621, 2642, 2662,
    2683, 2703, 2724, 2744, 2765, 2785, 2806, 2826, 2847, 2867, 2888, 2908,
    2929, 2949, 2970, 2990, 3011, 3031, 3052, 3072, 3092, 3113, 3133, 3154,
    3174, 3195, 3215, 3236, 3256, 3277, 3297, 3318, 3338, 3359, 3379, 3400,
    3420, 3441, 3461, 3482, 3502, 3523, 3543, 3564, 3584, 3604, 3625, 3645,
    3666, 3686, 3707, 3727, 3748, 3768, 3789, 3809, 3830, 3850, 3871, 3891,
    3912, 3932, 3953, 3973, 3994, 4014, 4035, 4055};

const uint16_t pulse_values[800] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096, 4096,
    4096, 4096, 4096, 4096, 4096, 4096, 4096, 0};
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

void write_next_two_byte_value_into_buffer(const uint16_t value)
{
    write_next_byte_into_buffer(value & 0xff);
    write_next_byte_into_buffer((value >> 8) & 0xff);
}

void write_end_sequence_into_buffer(void)
{
    write_next_byte_into_buffer('\n');
    write_next_byte_into_buffer(0xff);
}

uint8_t time_to_transfer_data()
{
    return buffer_index == (2 * SAMPLES_PER_DATA_TRANSFER);
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
  MX_USB_DEVICE_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

    // uint16_t adc_output;

    for (int i = 0; i < 800; ++i)
    {
        write_next_two_byte_value_into_buffer(sin_values[i]);
    }
    write_end_sequence_into_buffer();

    HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
        /*
        if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
        {
            adc_output = HAL_ADC_GetValue(&hadc1);

            write_next_two_byte_value_into_buffer(adc_output);

            HAL_ADC_Start(&hadc1);
        }
        else
        {
            HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_7);
        }

        if (time_to_transfer_data())
        {
            write_end_sequence_into_buffer();
            */
        CDC_Transmit_FS(usb_output_buffer, buffer_index);

        // buffer_index = 0;

        HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);
        HAL_Delay(100);
        //}

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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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

#ifdef  USE_FULL_ASSERT
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
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
