/*
  
  Project 3: Arduino Night Light
  Student: Maria Luisa Carrion (mcarrion)
  
  The Arduino Night Light displays an imagen in an 8x8 RGB LED matrix.
  For now, only the blue pins are being lighted up, since the Arduino Nano doesn't have enough pins for lighting up all the colors.
  The arduino displays a default image at maximum intensity as default. Then, the arduino waits for a Bluetooth signal,
  in order to change the image displayed and the intensity of the light. This Bluetooth message is sent from an Android phone with the Night Light Android app.

  
    Pin distribution:
    The following table presnts how the matrix pins are connected to the arduino pins. Here are the explanations of the headers:
      * mat = number of the pin in the matrix
      * col/row = matrix col/row to which the mat pin is connected
      * pin = arduino pin to which the mat (matrix pin) is connected
  
    mat  col  pin
    1    1    2* green
    2    2    3* yellow
    3    3    4* orange
    4    4    5* red
    5    5    6* brown
    6    6    7* black
    7    7    8* white
    8    8    9* gray
    
    mat  row  pin
    17   1    14 A0 * gray
    18   2    15 A1 * purple
    19   3    16 A2 * blue
    20   4    17 A3 * brown
    29   5    18 A4 * red
    30   6    19 A5 * orange
    31   7    11 *    green
    32   8    12 *    yellow
    
    Memory Registers:
    
      D – 0..7  (D0..D7)   (D0, D1 are com to PC)
      B – 8..13 (D8..D13)            (6 bit port)
      C – 14..19 (A0..A5) (D14..D19) (6 bit port)
  
*/

// Include library for the _delay_us(duration) function
#include <avr/delay.h>

// Set the pins of input/output devices
const int rows[8] = { 14, 15, 16, 17, 18, 19, 11, 12 };  // Pins of the rows
const int cols[8] = { 2, 3, 4, 5, 6, 7, 8, 9 };          // Pins of the columns

int image[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };              // holds the image to be displayed
int usec_on = 100;                                      // amount of micro seconds the lights will be on
int usec_off = 0;                                       // amount of micro seconds the lights will be off
char value = '0';                                       // Stores the value sent over Bluetooth

/* Initializization code */
void setup() {
  
  Serial.begin(9600);       // start serial communication at 9600bps
   
  DDRC |=  B00111111;   // Set rows 0 to 5 as OUTPUT
  DDRB |=  B00011000;   // Set rows 6 and 7 as OUTPUT
  PORTC &= ~B00111111;   // Set rows 0 to 5 as LOW
  PORTB &= ~B00011000;   // Set rows 6 and 7 as LOW
  DDRD |= B11111100;   // Set cols 0 to 5 as OUTPUT
  DDRB |= B00000011;   // Set cols 6 and 7 as OUTPUT
  PORTD &= ~B11111100;   // Set cols 0 to 5 as LOW
  PORTB &= ~B00000011;   // Set cols 6 and 7 as LOW
  
  // Load initial image to display
  load_image(image, '1');
  
}

/* Main routine (called repeated by the Arduino framework) */
void loop() {
  
  if( Serial.available() )       // if data is available to read
  {
    value = Serial.read();         // read it and store it in 'value'
    load_image(image, value);
    change_intensity(value);
  }
  
  if (usec_on != 0) {
    // Show the content of the image buffers in the LED matrix
    for( int row = 0; row < 8; row++ ){
      
      // Reverse bits to show the correct image (and not the mirror image)
      int reversed = (image[row] * 0x0202020202ULL & 0x010884422010ULL) % 1023;
      
      // Turn on columns - output, low
      //DDRD |= B11111100;   // Set cols 0 to 5 as OUTPUT
      //DDRB |= B00000011;   // Set cols 6 and 7 as OUTPUT
      PORTD =  ((reversed ^ B11111111) & B00111111) << 2;   // Write the LOW values for cols 0 to 5, for the LEDs we want to show; HIGH for the ones we don't want to show
      PORTB =  ((reversed ^ B11111111) & B11000000) >> 6;   // Write the LOW values for cols 6 and 7, for the LEDs we want to show; HIGH for the ones we don't want to show
     
      
      // Turn on the row
      digitalWrite( rows[row], HIGH);
      
      _delay_us( usec_on );
      
      // Turn off the row 
      digitalWrite( rows[row], LOW);
      
      _delay_us( usec_off );
      
      // Turn off columns - output, low    
      PORTD &= ~B11111100;   // Set cols 0 to 5 as LOW
      PORTB &= ~B00000011;   // Set cols 6 and 7 as LOW
      
    }
  }
  
  
  
}

// Load image based on the code received
void load_image(int buf[], char code) {
  if ( code == '1' ) { // water drop image
    buf[0] = B00001000;
    buf[1] = B00011000;
    buf[2] = B00110100;
    buf[3] = B01110100;
    buf[4] = B01110010;
    buf[5] = B01110010;
    buf[6] = B01111110;
    buf[7] = B00111100;
  }
  else if ( code == '2' ) { // sword image
    buf[0] = B00000011;
    buf[1] = B00000111;
    buf[2] = B00001110;
    buf[3] = B11011100;
    buf[4] = B01111000;
    buf[5] = B00110000;
    buf[6] = B01011000;
    buf[7] = B10001000;
  }
  else if ( code == '3' ) { // Smiley face image
    buf[0] = B11111111;
    buf[1] = B10000001;
    buf[2] = B10101001;
    buf[3] = B10101001;
    buf[4] = B10000101;
    buf[5] = B10111001;
    buf[6] = B10000001;
    buf[7] = B11111111;
  }
  else if ( code == '4' ) { // Space invaders image
    buf[0] = B00011000;
    buf[1] = B00111100;
    buf[2] = B01111110;
    buf[3] = B11011011;
    buf[4] = B11111111;
    buf[5] = B01011010;
    buf[6] = B10000001;
    buf[7] = B01000010;
  }
  else if ( code == '5' ) { // All LEDs on
    buf[0] = B11111111;
    buf[1] = B11111111;
    buf[2] = B11111111;
    buf[3] = B11111111;
    buf[4] = B11111111;
    buf[5] = B11111111;
    buf[6] = B11111111;
    buf[7] = B11111111;
  }
}

void change_intensity(char code) {
  if ( code == '0' ) { // maximum intensity
    usec_on = 1600;
    usec_off = 0;
  }
  else if ( code == '9' ) { // high intensity
    usec_on = 1200;
    usec_off = 400;
  }
    else if ( code == '8' ) { // medium intesity
    usec_on = 800;
    usec_off = 800;
  }
    else if ( code == '7' ) { // low intensity
    usec_on = 400;
    usec_off = 1200;
  }
    else if ( code == '6' ) { // turn lights off
    usec_on = 0;
    usec_off = 0;
  }
}
