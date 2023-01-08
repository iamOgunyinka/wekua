CC = gcc
CFLAGS = -W -Wall -g -fPIC -O2
archives = wekua.o tensor.o
modules = tensor

main: $(modules) $(archives)
	$(CC) $(CFLAGS) -shared -lOpenCL -pthread $(archives) -o libwekua.so -lm

debug: $(modules) $(archives)
	$(CC) $(CFLAGS) -shared -fsanitize=address -fsanitize-recover=address -lOpenCL -pthread $(archives) -o libwekua.so -lm

%.o: src/%.c
	$(CC) -c $(CFLAGS) $< -o $@

tensor:
	$(make) -C src/tensor
	cp src/tensor/tensor.o tensor.o

tensor_debug:
	$(make) -C src/tensor debug
	cp src/tensor/tensor.o tensor.o

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