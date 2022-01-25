#include <string.h>
#include <stdio.h>
#include "MFRC522.h"
#include <iostream>
#include <iomanip>
#include <sstream>


MFRC522 mfrc;
MFRC522::MIFARE_Key key;

// Each sector consists of 4 blocks, i.e. 3 data blocks and one sector trailer
// Sector 0 has Blocks 0-3 :  0 (manufacturer data), 1 (data),  2 (data),  3 (sector trailer)
// Sector 1 has Blocks 4-7 :  4 (data),              5 (data),  6 (data),  7 (sector trailer)
// Sector 2 has Blocks 8-11:  8 (data),              9 (data), 10 (data), 11 (sector trailer)
// and so on...
byte sector                 = 1;  // The sector we want to write data to
byte nameBlock              = sector * 4 + 0; // Name of the card owner
byte keyBlock               = sector * 4 + 1; // Secret KeySecurity
//byte ...                  = sector * 4 + 2; // Not used
byte trailerBlock           = sector * 4 + 3; // This is the sector trailer block in sector 1


// The array we read the data into (16 bytes for data, 2 for CRC)
byte buffer[18];
byte size = sizeof(buffer);

// Secret Key that is stored on the card
//byte[16] secretKey = "v3ry53cr37k3y123"
//byte[16] flushKey = "0000000000000000"


void nicePrint(const string &output)
{
    cout << endl << endl;

    const int width = 44;
    const char c = '*';
    int stringLength = static_cast<int>(output.length());

    string horizontal_line(width, c);

    cout << horizontal_line << endl;
    if(stringLength % 2 == 1) {
        cout << c << setw(width / 2 - 2 - stringLength / 2) << "" << left << output << setw(width / 2 - stringLength / 2) << right << c << endl;
    } else{
        cout << c << setw(width / 2 - 2 - stringLength / 2) << "" << left << output << setw(width / 2 - stringLength / 2 + 1) << right << c << endl;
    }
    cout << horizontal_line << endl << endl;
}


string getUID() 
{
  while(1){
    // Look for a card
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;
    
    // --- Read UID ---
    stringstream uid;
    
    for(byte i = 0; i < mfrc.uid.size; ++i){
      uid << std::uppercase << std::setfill('0') << std::setw(2) << hex << (int) mfrc.uid.uidByte[i] << " ";
    }
    
    return uid.str();        
  }
}


string read(byte sector, byte block)
{
  while(1){
    // Look for a card
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;
    

    // Calculate block number of sector trailer
    byte trailerBlock = sector * 4 + 3;
    
    MFRC522::StatusCode status;

    status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc.uid));

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Authenticate() failed: ";
        cout << mfrc.GetStatusCodeName(status);
        continue;
    }
    
    // --- Read Data ---        
    status = (MFRC522::StatusCode) mfrc.MIFARE_Read(block, buffer, &size);
  
    string data;    
    
    for(int i=0; i< 16; i++)
    {
      data.push_back((char) (int) buffer[i]);
    }
    
    // Halt PICC
    mfrc.PICC_HaltA();
    // Stop encryption on PCD
    mfrc.PCD_StopCrypto1();
    
    return data;
    
  }
}

void write(byte sector, byte block, byte value[], byte size)
{
  while(1)
  {
    if(!mfrc.PICC_IsNewCardPresent())
      continue;

    if( !mfrc.PICC_ReadCardSerial())
      continue;

  
    // Calculate block number of sector trailer
    byte trailerBlock = sector * 4 + 3;
    
    MFRC522::StatusCode status;
  
    status = (MFRC522::StatusCode) mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc.uid));

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Authenticate() failed: ";
        cout << mfrc.GetStatusCodeName(status);
    }

    status = (MFRC522::StatusCode) mfrc.MIFARE_Write(block, value, size);

    if (status != MFRC522::STATUS_OK) {
        cout << "MIFARE_Write() failed: ";
        cout << mfrc.GetStatusCodeName(status);
        
        continue;
    } else {
      cout << "Write Success" << endl;
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
  // The default key is 0xFFFFFFFFFFFF
  for (byte i = 0; i < 6; i++) {
        key.keyByte[i] = 0xFF;
    }

  
  while(1)
  {
    nicePrint("Card Registration Service");
    cout << "1 - Register new card\n";
    cout << "2 - Delete card\n";
    cout << "3 - Print card information\n";
    
    
    int choice;
    cout << endl << "Your choice: ";
    cin >> choice;
    
    cin.ignore();
    
    switch (choice)
    {
    case 1:
      {
        std::string name;
        cout << "Please enter the name of the card recipient: ";
        std::getline(cin, name);
        
        byte nameArr[16] = {(int) ' '};  // array filled with whitespaces
        
        for(int i = 0; i < min<size_t>(16, name.size()); i++) {
          nameArr[i] = (int) name[i];
        }
        
        cout << "Waiting for RFID Chip... " << std::flush;
        
        write(sector, nameBlock, nameArr, 16);
        //write(sector, keyBlock, secretKey, 16);
     
        break;
     }
     
    case 2:
      { 
        byte nameArr[16] = {(int) ' '};  // array filled with whitespaces
               
        write(sector, nameBlock, nameArr, 16);
        //write(sector, keyBlock, flushKey, 16);
     
        break;
     }
  
    case 3:
      {    
        string uid = getUID();
        string name = read(sector, nameBlock);
        //string key = read(sector, keyBlock);
        
        cout << "Waiting for RFID Chip... " << std::flush;
        
        cout << endl << endl;                
        cout << "--- Card Information ---" << endl << endl;
        cout << "Card ID:    " << uid << endl;
        cout << "Card Owner: " << name << endl;
        //cout << "Secret Key: " << key << endl;
        break;
      }      
      
    default:
      break;
    }
  }
  
  return 0;
}



