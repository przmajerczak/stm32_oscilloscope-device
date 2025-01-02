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
