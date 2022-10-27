gcc CriptoRelic.c \
    -I/home/pi/relic/relic-target/include \
    -I/home/pi/relic/include \
    -I/home/pi/openssl/include \
    -L/home/pi/relic/relic-target/lib \
    -lrelic_s \
    -lcrypto \
    -o server.run
	for i in {0..30}
do
	./server.run 
	
done
