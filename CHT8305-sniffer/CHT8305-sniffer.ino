/**
  @AUTHOR Pluimvee
 
  based on the work of
  https://github.com/rhmswink/Resideo-R200C2-ESPHome-Mod
  https://github.com/RobTillaart/CHT8305/tree/master
  
  https://en.gassensor.com.cn/Product_files/Specifications/CM1106-C%20Single%20Beam%20NDIR%20CO2%20Sensor%20Module%20Specification.pdf
 */

#include <Arduino.h>

#define PIN_SDA D2 // GPIO4
#define PIN_SCL D1 // GPIO5

static volatile byte device_address;
static volatile byte device_register[256];
static volatile byte device_register_ptr;

static volatile bool i2cIdle = true;  // true if the I2C BUS is idle
static volatile int bitIdx  = 0;      // the bitindex within the frame
static volatile int byteIdx = 0;      // nr of bytes within tis frame
static volatile byte data = 0;        // the byte under construction, which will persited in device_register when acknowledged
static volatile bool writing = true;  // is the master reading or writing to the device

////////////////////////////
//// Interrupt handlers
/////////////////////////////

// Rising SCL makes reading the SDA
void IRAM_ATTR i2cTriggerOnRaisingSCL() 
{
  if (i2cIdle)    // we didnt get a strat signal yet
    return;

	//get the value from SDA
	int sda = digitalRead(PIN_SDA);

	//decide where we are and what to do with incoming data
	int i2cCase = 0;    // data bit

	if (bitIdx == 8)    // we are already at 8 bits, so this is the (N)ACK bit
		i2cCase = 1; 

	if (bitIdx == 7 && byteIdx == 0 ) // first byte is 7bits address, 8th bit is R/W
		i2cCase = 2;

 	bitIdx++; // we found the first bit (out of the switch as its also needed for case 2!)

	switch (i2cCase)
	{
    default:
		case 0: // data bit
      data = (data << 1) | (sda>0?1:0);
	  	break;//end of case 0 general

		case 1: //(N)ACK
			if (sda == 0) // SDA LOW ->  ACK
      {
        switch (byteIdx)
        {
          case 0:
            device_address = data;
            break;
          case 1:
            if (writing) {  // if the master is writing, the first byte is the address in the device registers
              device_register_ptr = data;
              break;
            }
          // fall thru
          default:
            device_register[device_register_ptr++] = data;
            break;
        }
      }
			byteIdx++;  // next byte
      data = 0;   // reset this data byte
			bitIdx = 0; // start with bit 0
  		break;

		case 2:
      writing = (sda == 0);  // if SDA is LOW, the master wants to write 
  		break;
	}
}

/**
 * This is for recognizing I2C START and STOP
 * This is called when the SDA line is changing
 * It is decided inside the function wheather it is a rising or falling change.
 * If SCL is on High then the falling change is a START and the rising is a STOP.
 * If SCL is LOW, then this is the action to set a data bit, so nothing to do.
 */
void IRAM_ATTR i2cTriggerOnChangeSDA()
{
	if (digitalRead(PIN_SDA) > 0) //RISING if SDA is HIGH (1) -> STOP 
	{
    if (digitalRead(PIN_SCL) == 0)  // if SCL is low we are still communicating
      return;
		i2cIdle = true;
	}
	else //FALLING if SDA is LOW -> START?
	{
		if (i2cIdle) //If we are idle than this is a START
		{
			bitIdx  = 0;
			byteIdx = 0;
      data = 0;
      device_register_ptr = 0;
			i2cIdle = false;
		}
	}
}

/////////////////////////////////
////  MAIN entry point of the program
/////////////////////////////////
void setup() 
{
	//Define pins for SCL, SDA
  pinMode(PIN_SCL, INPUT_PULLUP);   
  pinMode(PIN_SDA, INPUT_PULLUP);

  //reset variables
  memset((void *) device_register, sizeof(device_register), 0);
	i2cIdle = true;

  //Atach interrupt handlers to the interrupts on GPIOs
  attachInterrupt(PIN_SCL, i2cTriggerOnRaisingSCL, RISING); //trigger for reading data from SDA
  attachInterrupt(PIN_SDA, i2cTriggerOnChangeSDA,  CHANGE); //for I2C START and STOP

	Serial.begin(115200);
}

/**
 * LOOP
 */
void loop() 
{
  Serial.printf("Device address 0x%02X, w? %d, ptr %02X", device_address, writing, device_register_ptr);

  Serial.printf("\t First 4 bytes 0x%02X\t0x%02X\t0x%02X\t0x%02X", device_register[0], device_register[1], device_register[2], device_register[3]);
  
  double T = (device_register[0] <<8 | device_register[1]);
  T = (T * 165.0 / 65535.0) - 40.0;
  T -= 1.4;   // as seen on display

  double H = (device_register[2] <<8 | device_register[3]);
  H = (H * 100.0 / 65535.0);
  H += 2.0;   // as seen on display

  Serial.printf("\tTemp: %.1f\t Humi: %.1f", T, H);

  Serial.println();
  delay(5000);
}
