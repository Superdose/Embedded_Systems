Make als Root/Sudo ausführen (sudo su)

```
ssh pi@192.168.0.xxx
sudo su
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71
cd bcm2835-1.71
./configure
make
make check
make install
```

Build:
```
g++ MFRC522.cpp main.cpp -std=c++11 -lbcm2835
```
