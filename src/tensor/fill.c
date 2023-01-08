#include "tensor.h"
#include <string.h>

int wekuaFillTensor(
	wtensor tensor, void *real, void *imag,
	uint32_t nw, cl_event *be, cl_event *e
){
	if (!tensor) return CL_INVALID_MEM_OBJECT;

	uint8_t pattern[sizeof(uint64_t)*2] = {0};

	wekuaContext ctx = tensor->ctx;
	uint64_t dl = ctx->dtype_length[tensor->dtype];

	if (real) memcpy(pattern, real, dl);
	if (imag) memcpy(pattern + dl, imag, dl);

	int ret =clEnqueueFillBuffer(
		ctx->command_queue,
		tensor->buffer,
		pattern, dl*2,
		0, tensor->size,
		nw, be, e
	);

	return ret;
}