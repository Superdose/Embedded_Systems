#include <stdio.h>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string> 
#include <vector>
#include <fstream>

#include "MFRC522.h"
#include <unistd.h>

using namespace std;

string userinput;               //Eingabe des Userinputs
vector<string> paketlist;       //Liste der registrierten Pakete
char str[32] = "";              //RFID-Card Nummer  


MFRC522::MIFARE_Key key;        //Key der RFID-Card
byte myKey[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};    //Values des Keys
byte blockAdresse = 20; //Hier wird die Blockadresse des Value Blocks angegeben
byte status;        //Status der Card
byte sectorTrailer;     //Sectortrailer
byte readBuf[18];       //eingelesener Wert der Card
byte sizeOfreadBuf = sizeof(readBuf);
long wert;      //Wert

string STRINGstatusPaket;       //Status des Paketes (String)
int32_t INTStatusPaket;         //Status des Paketes (Int)

MFRC522 mfrc;                   //Instanz der Klasse MFRC522

/* Funktionendeklaration */
void hello(); 
void checkUserinputforMenu(string s);
void checkUserinputforStatusPaket(string s);
void selectfunc(int i);
void paketregister();
void paketcheck();
void paketdelete();
void admin();
void readCard();
void paketchangeData(int32_t i32);
void zeigeBlock();
void array_to_string(byte array[], unsigned int len, char buffer[]);
void setup();
string statusSTRING(long i);


//Funktion, die für die Zuweisung der Eingabe zur Variable userinput sorgt
void hello() 
{
    cout << "Willkommen beim DVL - Lieferdienst \n";
    cout << "1 - Neues Paket hinzufügen\n";
    cout << "2 - Aktuelles Paket überprüfen \n";
    cout << "3 - Aktuelles Paket löschen \n";
    cout << "4 - Alle Pakete anzeigen \n";
    cout << "5 - Status des Paketes ändern \n";
    cout << "6 - Anwendung verlassen \n";
    cout << "Eingabe: ";  
    
    cin >> userinput;
}

//Funktion zum Behandeln von Userinput im Menü
void checkUserinputforMenu(string s) 
{
    try
    {
        int temp = stoi(s);
        selectfunc(temp);
    }
    catch(const std::exception& e)
    {
        cout << "Fehlerhafte Eingabe. Bitte erneut versuchen.\nEingabe: ";
        cin >> s;
        checkUserinputforMenu(s);
    }  
}

//Funktion zum Behandeln von Userinput vom Paketmenü
void checkUserinputforStatusPaket(string s) 
{
    try
    {
        INTStatusPaket = stoi(s);   
        if(INTStatusPaket == 1 || INTStatusPaket == 2 || INTStatusPaket == 3 || INTStatusPaket == 4 || INTStatusPaket == 5) 
        {
            paketchangeData(INTStatusPaket);
        } 
        else 
        {
            cout << "Fehlerhafte Eingabe. Bitte erneut versuchen.\nEingabe: ";
            cin >> s;
            checkUserinputforStatusPaket(s);
        }
        
    }
    catch(const std::exception& e)
    {
        cout << "Fehlerhafte Eingabe. Bitte erneut versuchen.\nEingabe: ";
        cin >> s;
        checkUserinputforStatusPaket(s);
    }  
}

//Auswahl der Funktion nach Userinput
void selectfunc(int i)
{
    switch (i)
    {
    case 1:
        paketregister();
        break;
    case 2:
        paketcheck();
        break;
    case 3:
        paketdelete();
        break;
    case 4:
        admin();
        break;
    case 5:
        cout << "Bitte Status des Paketes angeben: \n";
        cout << "1 - registriert \n";
        cout << "2 - wird für den Versand vorbereitet \n";
        cout << "3 - versendet \n";
        cout << "4 - wird zugestellt \n";
        cout << "5 - zugestellt \n";
        cout << "Eingabe: ";
        cin >> STRINGstatusPaket;
        checkUserinputforStatusPaket(STRINGstatusPaket);
        break;
    case 6:
        break; 
    default:
        hello();
        break;
    }
}

//Funktion, der Card einliest, überprüft, ob Card bis jetzt registriert und ggf. in Liste der registrierten Pakete hinzufügt
void paketregister() 
{
    readCard();
    if(find(paketlist.begin(), paketlist.end(), str) != paketlist.end()) 
    {
        cout << "Das Paket " << str << " wurde bereits registriert.\n";
    } 
    else 
    {
        paketlist.push_back(str);
        cout << "Das Paket " << str << " wurde registriert. \n";
        cout << "Bitte Status des Paketes angeben: \n";
        cout << "1 - registriert \n";
        cout << "2 - wird für den Versand vorbereitet \n";
        cout << "3 - versendet \n";
        cout << "4 - wird zugestellt \n";
        cout << "5 - zugestellt \n";
        cout << "Eingabe: ";
        cin >> STRINGstatusPaket;
        checkUserinputforStatusPaket(STRINGstatusPaket);
    }
    cout << "\n";
    cout << "\n";
    hello();
    checkUserinputforMenu(userinput);   
}

//Funktion, der Card einliest, überprüft welchen Status die Card hat
void paketcheck() 
{
    readCard();
    if(find(paketlist.begin(), paketlist.end(), str) != paketlist.end()) 
    {
        cout << "Das Paket " << str << " wurde erkannt.\n";
        cout << "Es hat den Status ";
        cout << statusSTRING(wert);
        cout << ".\n";
    } 
    else 
    {
        cout << "Das Paket wurde bisher noch nicht registriert. \n";
    }

    cout << "\n";
    cout << "\n";
    hello();
    checkUserinputforMenu(userinput); 
}

//Funktion, die registrierte Card aus Liste löscht
void paketdelete() 
{   
    vector<string>::iterator it;
    int index;

    readCard();
    if(find(paketlist.begin(), paketlist.end(), str) != paketlist.end()) 
    {   
        it = find(paketlist.begin(), paketlist.end(), str);
        index = it - paketlist.begin();
        paketlist.erase(paketlist.begin() + index);
        cout << "Das Paket " << str << " wurde erfolgreich gelöscht.\n";
    } 
    else 
    {
        cout << "Das Paket war nicht in der Liste der registrierten Geräte und konnte nicht gelöscht werden. \n";
    }

    cout << "\n";
    cout << "\n";
    hello();
    checkUserinputforMenu(userinput); 
}

//Funktion, die registrierten Pakete auf Konsole ausgibt
void admin() 
{
    if(!paketlist.empty()) 
    {
        for (string i: paketlist)
        {
            cout << i << "\n";
        } 
    } 
    else 
    {
        cout << "Keine registrierten Pakete. \n";
    }  

    cout << "\n";
    cout << "\n";
    hello();
    checkUserinputforMenu(userinput);   
}

//Einlesen der Karte
void readCard() 
{
    cout << "Bitte RFID-Chip zum Lesen an den RFID-Leser halten: \n";
    while(1)
    {
        // Nach Karte suchen
        if(!mfrc.PICC_IsNewCardPresent())
            continue;

        if(!mfrc.PICC_ReadCardSerial())
            continue;

        array_to_string(mfrc.uid.uidByte, 4, str);

        status = mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, sectorTrailer, &key, &(mfrc.uid));      //Authentifizierung der karte
        
		mfrc.MIFARE_GetValue(blockAdresse, &wert);      //Wert auslesen

        //Beende Authentifizierung des RFID-Tags
        mfrc.PICC_HaltA();
        mfrc.PCD_StopCrypto1();
        break;
    }
}

//Ändern des Valueblockes der Karte
void paketchangeData(int32_t i32) 
{
    cout << "Bitte RFID-Chip zum Status ändern an den RFID-Leser halten: \n";

    while(1)
    {
        // Nach Karte suchen
        if(!mfrc.PICC_IsNewCardPresent())
            continue;

        if(!mfrc.PICC_ReadCardSerial())
            continue;

        status = mfrc.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, sectorTrailer, &key, &(mfrc.uid)); //Authentifizierung der karte

        mfrc.MIFARE_Read(blockAdresse, readBuf, &sizeOfreadBuf);        //Valueblock auslesen
        mfrc.MIFARE_SetValue(blockAdresse, i32);                        //Valueblock setzen
        mfrc.MIFARE_Transfer(blockAdresse);                             //auf Card schreiben

        //Beende Authentifizierung des RFID-Tags
        mfrc.PICC_HaltA();
        mfrc.PCD_StopCrypto1();

        break;
    }

    long l = i32;
    cout << "Der Status des Paketes wurde zu ";
    cout << statusSTRING(l);
    cout << " geändert.\n";
    cout << "\n";
    cout << "\n";
    hello();
    checkUserinputforMenu(userinput);
}

//wandelt Bytearray zum String um
void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

//Initialisiert mfrc und überprüft, ob angesprochener Valueblock existiert
void setup() 
{
    mfrc.PCD_Init();
    for (byte i = 0; i < 6; i++) key.keyByte[i] = myKey[i];

    if ((blockAdresse + 1) % 4 == 0 || blockAdresse == 0 || blockAdresse > 63)
    {
        printf("%x", blockAdresse);
        if (blockAdresse == 0) cout << " ist ungueltig, weil Herstellerdaten-Block!";
        else if (blockAdresse > 63) cout << " ist ungueltig, weil ausserhalb Bereich!";
        else cout << " ist ungueltig, weil SectorTrailer!";
        cout << "Blockadresse für Value Block im Deklarationsteil des Programms aendern!!";
        while(1); //Wenn if-Bedingung erfuellt, dann geht's hier nicht mehr weiter!
    }
    sectorTrailer = blockAdresse + 3 - (blockAdresse % 4);
}

//wandelt long Wert in Status um
string statusSTRING(long i)
{
    string s;
    if(i == 1) {
        s = "registriert";
    }
    if(i == 2) {
        s = "wird für den Versand vorbereitet";
    }
    if(i == 3) {
        s = "versendet";
    }
    if(i == 4) {
        s = "wird zugestellt";
    }
    if(i == 5) {
        s = "zugestellt";
    }

    return s;
}

int main () 
{   
    setup();

	hello();
    checkUserinputforMenu(userinput);

    cout << "Programm wird geschlossen ...";
	return 0;
}

