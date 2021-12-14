# Hardware

## Raspberry Pi Model B (Revision 2011.12)

- VideoCore IV-Basierter SBC, BCM2835, 1 Kern 700 MHz ARMv6.
- Muss ich mehr Sagen?
- OS: ``
- Pinout:![](https://developer-blog.net/wp-content/uploads/2013/09/raspberry-pi-rev2-gpio-pinout.jpg)

## AZDelivery RFID Kit RC522-Modul:

- Enthaltener NFC-Chipsatz: NXP Semiconductors MFRC522 [(Datenblatt)](https://www.nxp.com/docs/en/data-sheet/MFRC522.pdf)

- Kompatibel zu ISO/IEC 14443A ([NXP Semiconductors MIFARE-Serie](https://de.wikipedia.org/wiki/Mifare),[Herstellerseite](https://www.nxp.com/products/rfid-nfc/mifare-hf:MC_53422)) Und [NTAG](https://www.nxp.com/products/rfid-nfc/nfc-hf/ntag-for-tags-labels:NTAG-TAGS-AND-LABELS) NFC-Tags

  - Fun Fact: Unsere HTWK-Cards sind MIFARE DESFire EV1 (MF3ICDH81)

- Lässt sich (Entgegen dem Mini-E-Book) per UART, I²C oder SPI ansteuern, wobei SPI den höchsten Pincount hat.

- Besitrt einen Hardware IRQ-Pin zum Anzeigen von Interruptereignissen, **Nutzung hiervon sollte dem Krämer gut gefallen.**

- Pinout:

  ![](https://cdn.discordapp.com/attachments/895947649021251585/920419123903725618/7d5r13tRFcywyPPj4qUhst6ZcPEpm3hF9fqhdC_fmHFJoV9BwmRdpv-dXBf1qGOzC0tkG2mhmREFmnxecbzEmkUMETOziDuXBZwHpjxlXXWqVE5jw6LneP-mLkiWE9_cc-xMeIDg.png)

## NFC-Tags

- Vom Typ MIFARE Classic (bzw. *kompatibel*, später dazu mehr...) 
- 1 kB EEPROM

  - 16 Sektoren mit je 4 Blöcken

    - Block enthält 16 Byte
  - 7 Byte UID oder 4 Byte NUID [^1]
    - Bei Original-Karten: Read-Only
  - Proprietäres Zusatzprotokoll zur Echtheitsprüfung wenn von NXP hergestellt
- Zugriffskontrolle
  - Zwei Schlüssel (A und B) pro Sektor, Jedem Schlüssel werden pro Sektor Rechte zugewiesen.
    - Beispiel: Schlüssel **A** kann Sektor nur lesen, **B** kann lesen und Schreiben.

  - Praktisch ausgehebelt[^2]


### NFC-Tags im Lieferumfang enthalten

**Um die Weitere Verwendung im Projekt zu vereinfachen habe ich sie mit UID und anderen Infos die die TagInfo-App von NXP auslesen lässt beschriftet!**

- Karte
  ![image-20211214224734461](https://raw.githubusercontent.com/Superdose/Embedded_Systems/main/Docs/Voruntersuchungen.assets/image-20211214224734461.png)
  Kompatibler Klon, nicht von NXP hergestellt, UID `8A:08:B1:1A`

  

- Schlüsselanhänger
  ![image-20211214230003159](https://raw.githubusercontent.com/Superdose/Embedded_Systems/main/Docs/Voruntersuchungen.assets/image-20211214230003159.png)


  Kompatibler Klon, nicht von NXP hergestellt, UID `BA:D2:2D:86`

## NFC-Tags aus eigener Sammlung

- Weitere Klone:

  ![image-20211214230114325](https://raw.githubusercontent.com/Superdose/Embedded_Systems/main/Docs/Voruntersuchungen.assets/image-20211214230114325.png)
  UIDs(Links oben nach rechts unten): `E0:28:55:1B`,`FA:68:05:09`,`FA:72:C0:09`,`FA:67:49:09`

- NXP-Originalkarten:

  ![image-20211214230340374](https://raw.githubusercontent.com/Superdose/Embedded_Systems/main/Docs/Voruntersuchungen.assets/image-20211214230340374.png)

  Original NXP-Hardware, MF1S50
  UIDs: `0A:66:6A:43` und `0A:3D:DB:0A`

## Verkabelung

| RF522 Modul | Raspberry Pi           |
| :---------- | :--------------------- |
| SDA         | Pin 24 / GPIO8 (CE0)   |
| SCK         | Pin 23 / GPIO11 (SCKL) |
| MOSI        | Pin 19 / GPIO10 (MOSI) |
| MISO        | Pin 21 / GPIO9 (MISO)  |
| IRQ         | —                      |
| GND         | Pin6 (GND)             |
| RST         | Pin22 / GPIO25         |
| 3.3V        | Pin 1 (3V3)            |

Quelle: https://tutorials-raspberrypi.de/raspberry-pi-rfid-rc522-tueroeffner-nfc/

**Muss noch angepasst werden um IRQ einzubeziehen. Doch dazu später mehr.**

# Software

## Planung

- Mini-Zugriffskontrollsystem ähnlich existenter MIFARE-basierter Zugriffskontrollsysteme:
  - Pi ist "Türschloss"
  - Unterteilung in Master-Keycard und normale Keycards:
    - Master-Keycard setzt Schloss in Anlernmodus
      - Nächste gescannte normale Keycard:
        - Nicht bekannt: erhält Zugangsberechtigung, "Schloss-ID" wird geschrieben
        - Bekannt: Bekommt Zugangsberechtigung entzogen, "Schloss-ID" wird entfernt
    - Normale Keycard:
      - Bekannt: Erfolgsmeldung ("Schloss geöffnet o.Ä.")
      - Unbekannt: Fehlermeldung ("Zutritt verweigert o.Ä.")

[^1]: Non-Unique Identifier
[^2]: https://www.cs.ru.nl/~flaviog/publications/Attack.MIFARE.pdf