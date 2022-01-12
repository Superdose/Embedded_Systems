#include <string.h>
#include <stdio.h>
#include "MFRC522.h"
#include <iostream>

MFRC522 mfrc;
MFRC522::MIFARE_Key key;

byte sector = 1;
byte blockAddr = 4;

byte secretKey[]    = {
        0x01, 0x03, 0x03, 0x07, 
        0x01, 0x03, 0x03, 0x07, 
        0xff, 0xee, 0xdd, 0xcc, 
        0xdc, 0x3d, 0x4d, 0xc3  
    };
byte purge[]    = {
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00  
    };
byte trailerBlock = 7;
byte buffer[18];
byte size = sizeof(buffer);

void toHexString(byte *buffer, byte bufferSize, char *output) {
  char lookup[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
    for (int i = 0; i < bufferSize; i++) {
        output[2*i  ] = lookup[(buffer[i]>>4)&0xF];
        output[2*i+1] = lookup[buffer[i] & 0xF];
    }
    output[2*bufferSize] = 0;
}

void read()
{
  while(1){
    // Look for a card
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;

    
    MFRC522::StatusCode status;

    status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc.uid));

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Authenticate() failed: ";
        cout << mfrc.GetStatusCodeName(status);
    }
    
    status = (MFRC522::StatusCode) mfrc.MIFARE_Read(blockAddr, buffer, &size);
    
    char out[64];
    toHexString(buffer, 16, out);
    cout << out << "\n";
    
    // Halt PICC
    mfrc.PICC_HaltA();
    // Stop encryption on PCD
    mfrc.PCD_StopCrypto1();
    break;
    
  }
}

void write(byte value[])
{
  while(1)
  {
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;

        
    MFRC522::StatusCode status;

    status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc.uid));

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Authenticate() failed: ";
        cout << mfrc.GetStatusCodeName(status);
    }

    status = (MFRC522::StatusCode) mfrc.MIFARE_Write(blockAddr, value, 16);

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Write() failed: ";
        cout << mfrc.GetStatusCodeName(status);
    } else {
      cout << "Write Success\n";
    }

    
    // Halt PICC
    mfrc.PICC_HaltA();
    // Stop encryption on PCD
    mfrc.PCD_StopCrypto1();
   
    break;
    
  }
}


int main()
{
  mfrc.PCD_Init();
  for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

  while(1)
  {
    cout << "Card-Tool\n";
    cout << "1 - register new card\n";
    cout << "2 - delete card\n";
    cout << "3 - print card information\n";

    int choice;
    cin >> choice;

    switch (choice)
    {
    case 1:
      write(secretKey);
      break;
    case 2:
      write(purge);
      break;
    case 3:
      read();
      break;    
    default:
      break;
    }
  }
  
  return 0;
}



