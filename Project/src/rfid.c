#import <rfid.h>

/* Assumes a pigpio handle was already setup */
void rfid_write(char addr, char val)
{
    spi_write();
}

void rfid_reset()
{

}

int rfid_init()
{


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

