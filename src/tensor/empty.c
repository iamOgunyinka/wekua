#include "tensor.h"
#include <string.h>

static int release_tensor(wtensor tensor){
	if (!tensor) return 0;

	if (tensor->shape) free(tensor->shape);
	if (tensor->vl_shape) free(tensor->vl_shape);
	if (tensor->work_items) free(tensor->work_items);

	int ret = clReleaseMemObject(tensor->buffer);
	if (ret == CL_SUCCESS) free(tensor);
	return ret;
}

wtensor wekuaTensorEmpty(wekuaContext ctx, uint8_t dtype, uint8_t com, uint32_t ndim, ...){
	if (!ctx || dtype > 9 || ndim == 0) return NULL;

	wtensor tensor = calloc(1, sizeof(struct _w_tensor));
	if (!tensor) return NULL;

    tensor->dtype = dtype;
	tensor->ctx = ctx;
    tensor->ndim = ndim;

	uint64_t *shape = calloc(ndim, sizeof(uint64_t));
	if (!shape) goto wekuaTensorEmpty_fail;
	tensor->shape = shape;

	uint64_t *vl_shape = calloc(ndim, sizeof(uint64_t));
	if (!vl_shape) goto wekuaTensorEmpty_fail;
	tensor->vl_shape = vl_shape;

	va_list args;
	va_start(args, ndim);

	uint64_t nelements = 1;
	for (uint64_t i = 0; i < ndim; i++){
		uint64_t l = va_arg(args, uint64_t);

		nelements *= l;
		shape[i] = l;
	}
	memcpy(vl_shape, shape, ndim*sizeof(uint64_t));
	va_end(args);

	uint64_t vl = (uint64_t) ctx->device.vectors_size[dtype];
	uint64_t dl = (uint64_t) ctx->dtype_length[dtype];
	uint64_t max = ctx->device.max_work_group_size;
	uint64_t columns = shape[ndim-1];
	uint64_t vl_nelements = nelements/columns;
	uint64_t size = nelements*dl;

	if (columns%vl) columns += vl - columns%vl;
	columns /= vl;

	vl_shape[ndim-1] = columns;
	vl_nelements *= columns;

	tensor->nelements = vl_nelements * vl;
	tensor->vl_nelements = vl_nelements;

	uint64_t *work_items = (uint64_t*) calloc(ndim*4 + 2, sizeof(uint64_t));
	get_local_work_items(vl_shape, work_items, ndim, max);
	get_local_work_items(shape, work_items + ndim*2, ndim, max);
	get_local_work_items(&vl_nelements, work_items + 4*ndim, 1, max);
	get_local_work_items(&nelements, work_items + 4*ndim, 1, max);
	for (uint64_t i = 0; i < ndim; i++) {
		get_local_work_items(vl_shape + i, work_items + ndim + i, 1, max);
		get_local_work_items(shape + i, work_items + 3*ndim + i, 1, max);
	}
	tensor->work_items = work_items;

	int ret = CL_SUCCESS;
	cl_mem buffer = clCreateBuffer(ctx->ctx, ctx->mem_flags, size, NULL, &ret);
	if (ret != CL_SUCCESS) goto wekuaTensorEmpty_fail;

	tensor->buffer = buffer;
	tensor->size = size;

	goto wekuaTensorEmpty_success;
	wekuaTensorEmpty_fail:
	wekuaFreeTensor(tensor);
	tensor = NULL;

	wekuaTensorEmpty_success:
	return tensor;
}