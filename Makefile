CC = gcc
CFLAGS = -W -Wall -g -fPIC -O2
archives = wekua.o

main: $(archives)
	$(CC) $(CFLAGS) -shared -lOpenCL -pthread $(archives) -o libwekua.so -lm

debug: $(archives)
	$(CC) $(CFLAGS) -shared -fsanitize=address -fsanitize-recover=address -lOpenCL -pthread $(archives) -o libwekua.so -lm

%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

install:
	cp libwekua.so /usr/lib/
	rm -rf /usr/include/wekua
	cp -r headers /usr/include/wekua
	rm -rf /usr/lib/wekua_kernels/
	cp -r kernels/ /usr/lib/wekua_kernels/
	chmod 755 /usr/lib/wekua_kernels
	chmod 644 /usr/lib/wekua_kernels/*
	chmod 755 /usr/include/wekua
	chmod 644 /usr/include/wekua/*

uninstall:
	rm -rf /usr/lib/libwekua.so
	rm -rf /usr/include/wekua
	rm -rf /usr/lib/wekua_kernels

clean:
	rm -rf $(archives)