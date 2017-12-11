# ABIEncoder
Arduino Incremental encoder library to use a LS7366R.

The LS7366R handles the decoding of A+B/I signals from a quadrature encoder.

There is a small board implementing the LS7366R from Mikroe: https://www.mikroe.com/counter-click

<img src="https://raw.githubusercontent.com/Ben0it/ABIEncoder/master/Documentation/Mikroe%20Counter%20Click.jpg"/>

It only uses the SPI signals from the Arduino:
 - MISO: Master In Slave Out, or the data coming from the slave (LS7366R) into the master (Arduino).
 - MOSI: Master Out Slave I, or the data coming from the master (Arduino) into the slave (LS7366R).
 - SCK: clock.
 - SS: Slave Select, one per slave to choose from multiple components on the same SPI bus.

You can read more about quadrature encoder signals on <a href="https://en.wikipedia.org/wiki/Rotary_encoder">Wikipedia</a>.
