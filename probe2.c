#include "headers/wekua.h"
#include <stdio.h>
#include <complex.h>

int main(){
	wekuaContext ctx = createSomeWekuaContext(CL_DEVICE_TYPE_CPU, 0, 0);

	printf("%p\n", ctx);
	printf("%s\n", ctx->device.name);

	freeWekuaContext(ctx);
	return 0;
}