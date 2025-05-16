INTERFACE

1.0 The device sends single conversion result every ~10 ms.
    Data format is raw 12-bit ADC result sent as 5 chars:
    four ASCII digits and newline symbol. Zeroes on the front
    are replaced with spaces.

2.0 The device sends 800 conversion results during one transmission.
    Less significant byte is sent first, followed by more significant one.
    After all 1600 bytes of results comes end sequence, consisting of 
    0xa ('\n') and 0xff. ADC measurement occurs every 125 µs, resulting in
    sampling frequency of 8 kHz.

2.1 The device sends 800 conversion results during one transmission.
    Less significant byte is sent first, followed by more significant one.
    After all 1600 bytes of results comes 4 bytes of the timer value,
    arranged least significant byte first. The value represents count of
    168 MHz timer ticks since the moment of first measurement until
    the last measurement contained in transmission. That value is followed by
    end sequence, consisting of 0xa ('\n') and 0xff.
    ADC measurement occurs every 125 µs, resulting in sampling frequency of 8 kHz.

2.2 Same as 2.1, but end sequence is 0xff 0xff instead of 0xa 0xff.

2.3 The device sends 10000 conversion results during one transmission.
    Less significant byte is sent first, followed by more significant one.
    After all 20000 bytes of results comes 4 bytes of the timer value,
    arranged least significant byte first. The value represents count of
    168 MHz timer ticks since the moment of first measurement until
    the last measurement contained in transmission. That value is followed by
    end sequence, consisting of two 0xff values.
    ADC measurement occurs every 10 µs, resulting in sampling frequency of 100 kHz.

2.4 Same as 2.3, but device sends 12000 samples, that is 24000 bytes + 4 bytes of
    timer value + 2 bytes of end sequence - 24006 bytes total. Set of measurements
    and transfer happens once every ~30 ms.

3.0
    The device sends 15000 conversion results (samples) during one transmission.
    ess significant byte is sent first, followed by more significant one.
    After all 30000 bytes of results comes 4 bytes of the timer value,
    arranged least significant byte first. The value represents count of
    168 MHz timer ticks since the moment of first measurement until
    the last measurement contained in transmission. That value is followed by
    end sequence, consisting of one value 0xff or 0xfe (0xff minus channel id -
    0 for channel 1 and 1 for channel 2) followed by fixed 0xff value.
    30 ms of delay is applied after burst of two transmissions (one per each channel).

3.1
    The device sends 15000 conversion results (samples) during one transmission.
    ess significant byte is sent first, followed by more significant one.
    After all 30000 bytes of results comes 4 bytes of the timer value,
    arranged least significant byte first. The value represents count of
    168 MHz timer ticks since the moment of first measurement until
    the last measurement contained in transmission. That value is followed by
    end sequence, consisting of one value 0xff or 0xfe (0xff minus channel id -
    0 for channel 1 and 1 for channel 2) followed by value 0xff minus number
    of active channels (up to 2, so valid values are 0xfd for dual channel mode,
    0xfe for single channel mode and 0xff for no channel active, but then no
    transmission takes place). 30 ms of delay is applied after burst of transmissions
    (one per each channel).


OTHER

A6 pin sets ADC range mode. Default is high state indicating faster mode.
D6 and D13 pins control (respectively) channel 1 and channel 2 status. Low state
is channel active, with default high, enforced by pull-up resistors.

Blue LED connected to PA5 is on during USB transmission. Frequency of blinking
helps to determine if ADC is set to faster or slower mode.