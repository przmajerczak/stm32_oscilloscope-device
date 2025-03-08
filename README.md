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
    timer value + 2 bytes of end sequence - 24006 bytes total.

OTHER

There is reference square wave generated on G2 and G3 pins, basing on timer 4.
G2 frequency is 500 kHz, G3 frequency is 2.5 kHz
D11 pin sets ADC range mode. Default is high state indicating faster (DMA) mode.
