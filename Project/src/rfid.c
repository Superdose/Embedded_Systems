#import <rfid.h>

/* Assumes a pigpio handle was already setup */
void rfid_write(char addr, char val)
{
    bcm2835_spi_transfer();		
}

char rfid_read(char addr)
{
    return bcm2835_spi_transfer(addr);	
}

void rfid_reset()
{

}

void rfid_antenna_on() 
{
    
}

int rfid_init()
{
    
    bcm2835_spi_begin();
    /* heißt wenn CS0 auf low, dann ist CS0 aktive */
    bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
    /* 1.653125 MHz Clock */
    bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128);

    /* CS0 aktivieren */
    bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

    rfid_reset();
    rfid_write(REG_TMODE, 0x8D);
    rfid_write(REG_PRESCALER, 0x3E);
    rfid_write(REG_RELOADL, 30);   
    rfid_write(REG_RELOADH, 0);
    rfid_write(REG_TXAUTO, 0x40);
    rfid_write(REG_MODE, 0x3D);

    rfid_antenna_on();

    return 0;
}

