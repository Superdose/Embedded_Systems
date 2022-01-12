## Make als Root/Sudo ausführen (sudo su)

```
ssh pi@192.168.0.xxx
sudo su
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71.tar.gz
cd bcm2835-1.71
./configure
make
make check
make install
```

## Build:
```
g++ MFRC522.cpp main.cpp -std=c++11 -lbcm2835
```

## PIN-Layout gemäß Anleitung: 

- RST --> 22
- SDA --> 24
- MOSI --> 19
- MISO --> 21
- SCK --> 23
- 3v --> 1
- GND --> 25

