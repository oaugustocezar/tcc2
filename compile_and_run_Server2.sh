gcc CriptoWolf.c -lwolfssl \
    -I/home/pi/relic/relic-target/include \
    -I/home/pi/relic/include \
    -I/home/pi/openssl/include \
    -L/home/pi/relic/relic-target/lib \
    -lrelic_s \
    -o wolf.run
	for i in {0..30}
do
	./wolf.run 
	
done
