version := 1.71

setup:
	wget http://www.airspayce.com/mikem/bcm2835/bcm2835-$(version).tar.gz
	tar zxvf bcm2835-$(version).tar.gz
	cd ./bcm2835-$(version); ./configure; make; make install


run:
	g++ MFRC522.cpp main.cpp -std=c++11 -lbcm2835
	./a.out

clean:
	rm ./a.out || true
	rm -rf bcm2835-$(version) || true
	rm bcm2835-$(version).tar.gz
