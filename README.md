# Embedded_Systems

## Aufgabenstellung

Vorbereitung:
    Pi mit RFID aufrüsten

1. Aufgabe: Gruppen erstellen (Vorschlag max. 4)

2. Aufgabe: Jede Gruppe sollte über einen Raspberry Pi und einen RFID-Chip (+ Lesegerät) verfügen

3. Aufgabe: Wie schließe ich das Board hardware-mäßig an (Kabel, Pins, etc.)
Vorsicht nicht den RP oder das Board zerstören!

4. Aufgabe: Software-Anbindung, Bibliotheken etc (nur low level)
C

5. Aufgabe: Die eigentliche Aufgabe (in C/C++)
Oberfläche

RFID auslesen und in Konsole ausgeben, ob RFID akzeptiert wird (+ Daten ausgeben). RFID beschreiben

Dokumentation
- Inbetriebnahme
- Quellen

## Idee Umsetzung

- Grüne/Rote LED wenn RFID-Chip akzeptiert/nicht akzeptiert
- Buzzer
- Konsoleneingabe: "Chip beschreiben/auslesen"
- beschreiben nur mit PW-Eingabe
- Datenbank-Emulation als Array (oder Extra-Datei)
- User/Administrator Trennung -> ein Chip ist ein normaler User, der andere Administrator

- Zusatzideen (Log, PWM - Motor oder RGB-LED, 

## Umsetzung

1. RFID-Leser löten
2. SPI (http://www.netzmafia.de/skripten/hardware/RasPi/RasPi_SPI.html)

## Bibliotheken

- [bcm2835](https://www.airspayce.com/mikem/bcm2835/) - gut dokummentierte C-Bibliothek für RPi mit SPI-Funktionalität

- Logging (https://github.com/rxi/log.c)

## Kompilierung

- gcc in Linux -> "gcc -o ../target/run.sh main.c ../libs/*.c" (ausführen in "src" Ordner; eventuell muss "target" Ordner extra erstellt werden)
