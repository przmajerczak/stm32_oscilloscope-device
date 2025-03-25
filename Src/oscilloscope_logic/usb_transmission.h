#ifndef __USB_TRANSMISSION_H__
#define __USB_TRANSMISSION_H__

#include "defines.h"
#include "usb_device.h"

uint8_t usb_output_buffer[USB_OUTPUT_BUFFER_SIZE];
volatile uint16_t buffer_index = 0;

const uint8_t ONE_ACTIVE_CHANNEL = 1;
const uint8_t TWO_ACTIVE_CHANNELS = 2;

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

void write_end_sequence_into_buffer(const uint16_t channel_id,
                                    const uint16_t number_of_active_channels)
{
    write_next_byte_into_buffer(0xff - channel_id);
    write_next_byte_into_buffer(0xff - number_of_active_channels);
}

void reset_buffer_index() { buffer_index = 0; }

void dualChannelMode_writeChannel1ToBuffer(const uint32_t adc_data[],
                                           const uint32_t measurements_period)
{
    for (uint16_t sample_idx = 0; sample_idx < SAMPLES_PER_DATA_TRANSFER;
         sample_idx += 3)
    {
        write_next_two_byte_value_into_buffer(adc_data[sample_idx] & 0xffff);
        write_next_two_byte_value_into_buffer((adc_data[sample_idx] >> 16) &
                                              0xffff);

        write_next_two_byte_value_into_buffer(adc_data[sample_idx + 1] & 0xffff);
    }

    write_next_four_byte_value_into_buffer(measurements_period);
    write_end_sequence_into_buffer(CHANNEL_1, TWO_ACTIVE_CHANNELS);
}

void dualChannelMode_writeChannel2ToBuffer(const uint32_t adc_data[],
                                           const uint32_t measurements_period)
{
    for (uint16_t sample_idx = 0; sample_idx < SAMPLES_PER_DATA_TRANSFER;
         sample_idx += 3)
    {
        write_next_two_byte_value_into_buffer((adc_data[sample_idx + 1] >> 16) &
                                              0xffff);

        write_next_two_byte_value_into_buffer(adc_data[sample_idx + 2] & 0xffff);
        write_next_two_byte_value_into_buffer((adc_data[sample_idx + 2] >> 16) &
                                              0xffff);
    }

    write_next_four_byte_value_into_buffer(measurements_period);
    write_end_sequence_into_buffer(CHANNEL_2, TWO_ACTIVE_CHANNELS);
}

void singleChannelMode_writeOnlyChannelToBuffer(
    const uint32_t adc_data[], const uint32_t measurements_period,
    const uint16_t channelId)
{
    for (uint16_t sample_idx = 0; sample_idx < SAMPLES_PER_DATA_TRANSFER / 2;
         ++sample_idx)
    {
        write_next_two_byte_value_into_buffer(adc_data[sample_idx] & 0xffff);
        write_next_two_byte_value_into_buffer((adc_data[sample_idx] >> 16) &
                                              0xffff);
    }

    write_next_four_byte_value_into_buffer(measurements_period);
    write_end_sequence_into_buffer(channelId, ONE_ACTIVE_CHANNEL);
}

void transmit_data_over_usb(const uint16_t number_of_active_channels)
{
    CDC_Transmit_FS(usb_output_buffer,
                    number_of_active_channels *
                        (2 * SAMPLES_PER_DATA_TRANSFER + 4 + 2));

    reset_buffer_index();
}

#endif
