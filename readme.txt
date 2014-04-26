Project 3: Arduino Night Light

Student: Maria Luisa Carrion (mcarrion)


The Arduino Night Light displays an imagen in an 8x8 RGB LED matrix.

For now, only the blue pins are being lighted up, since the Arduino Nano doesn't have enough pins for lighting up all the colors.

The arduino displays a default image at maximum intensity as default.
Then, the arduino waits for a Bluetooth signal,
  in order to change the image displayed and the intensity of the light.
This Bluetooth message is sent from an Android phone with the Night Light Android app.