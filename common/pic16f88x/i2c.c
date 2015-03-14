#include "I2C.h"
/**
 * Initialize the I2C module as a master mode<br/>
 * SCL - RC3
 * SDA - RC4
 * 100kHz I2C click at 4Mhz crystal
 */
void i2c_open(unsigned long clock_Hz){
    TRISC3 = 1;//make SCL as input
    TRISC4 = 1;//make SDA as intput

    //----------configs for SSPSTAT register------------
    SSPSTAT = 0b10000000;//SMP = 1 [Slew rate control disabled for standard speed mode (100 kHz and 1 MHz)]
    //--------------------------------------------------

    //---------- configs for SSPCON register------------
    //Synchronous Serial Port Mode Select bits
    //configs as I2C Master mode, clock = FOSC / (4 * (SSPADD+1))
    SSPM0 = 0;
    SSPM1 = 0;
    SSPM2 = 0;
    SSPM3 = 1;

    SSPEN = 1;//Enables the serial port and configures the SDA and SCL pins as the source of the serial port pins

    SSPOV = 0;//No overflow

    WCOL = 0;//No collision
    //--------------------------------------------------

    //---------- configs for SSPCON2 register-----------
    SSPCON2 = 0;//initially no operations on the bus
    //--------------------------------------------------

    //SSPADD = 40;// 100kHz clock speed at 4Mhz cystal
    SSPADD = (((uint32_t)_XTAL_FREQ >> 2) / clock_Hz) - 1;
    //----------PIR1-----------
    SSPIF = 0;//clear  Master Synchronous Serial Port (MSSP) Interrupt Flag bit

    //-----------PIR2-----------
    BCLIF = 0;//clear Bus Collision Interrupt Flag bit
}

/**
 * wait until I2C bus become idel
 */
void i2c_wait(){
    while(R_W || SEN || RSEN || PEN || RCEN || ACKEN);
}

/**
 * Send 8 bit(1 byte) through I2C bus
 * @param data - 8 bit data t be sent
 */
void i2c_send(unsigned char data){
    i2c_wait();
    SSPBUF = data;
    //while(BF) continue;// wait until complete this bit transmision
    i2c_wait();//wait until any pending transaction
}

/**
 * Send start condition to I2C bus
 */
void i2c_start(){
    i2c_wait();
    SEN = 1;//Initiate Start condition on SDA and SCL pins. Automatically cleared by hardware
    i2c_wait();
}

/**
 * Send stop condition to I2C bus
 */
void i2c_stop(){
    i2c_wait();
    PEN = 1;//Initiate Stop condition on SDA and SCL pins. Automatically cleared by hardware.
    i2c_wait();
}

/**
 * Send restart condition to I2C bus
 */
void i2c_restart(){
    i2c_wait();
    RSEN = 1;//Initiate Repeated Start condition on SDA and SCL pins. Automatically cleared by hardware.
    i2c_wait();
}

/**
 * read the I2C bus
 * @return Read data from I2C slave
 */
unsigned char i2c_read(){
    unsigned char temp;

    i2c_wait();

    //configure MSSP for data recieving
    RCEN = 1;//Enables Receive mode for I2C

    i2c_wait();

    while(!BF);//wait for buffer full
    temp = SSPBUF;//read the buffer
    i2c_wait();//wait for any transaction
    return temp;//return the bufferd byte
}

/**
 * send acknowledge condition
 */
void i2c_send_ack(){
    i2c_wait();
    ACKDT = 0;//Acknowledge Data bit(0 = Acknowledge)
    ACKEN = 1;//Initiate Acknowledge sequence on SDA and SCL pins, and transmit ACKDT data bit.Automatically cleared by hardware.
    i2c_wait();
}

/**
 * send not acknowledge condition
 */
void i2c_send_nack(){
    i2c_wait();
    ACKDT = 1;//Acknowledge data bit(1 = Not Acknowledge)
    ACKEN = 1;//Initiate Acknowledge sequence on SDA and SCL pins, and transmit ACKDT data bit.Automatically cleared by hardware.
    i2c_wait();
}

