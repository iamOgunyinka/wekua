#ifndef WEKUA_TENSOR_H
#define WEKUA_TENSOR_H

#include "wekua.h"

typedef struct _w_tensor {
	// Free function
	int (*free)(void *);

	// Buffer information
	cl_mem buffer;
	uint64_t size;

	wekuaContext ctx;

	// Tensor shape
	uint64_t ndim; // Dimesion number
	uint64_t *shape;
	uint64_t *vl_shape; // Shape in Vector format

	uint64_t length; // Total number of elements in the tensor

	uint8_t com; // Does the matrix use complex elements?

	// Info for OpenCL

	// Here we save the work items numbers. This array has 4*ndim elements
	// The first 'ndim' numbers are the work items to execute a kernel of 'ndim' dimensions with vectors
	// The next 'ndim' numbers are the work items for each vl_shape number to execute a kernel of 1-D with vectors
	// The next 'ndim' numbers are the work items to execute a kernel of 'ndim' dimensions without vectors
	// The next 'ndim' numbers are the work items for each vl_shape number to execute a kernel of 1-D without vectors
	uint64_t *work_items;
} *wtensor;

void wekuaTensorPrint(wtensor tensor, uint32_t nw, cl_event *be);

// Complex numbers utils
int wekuaTensorEnableComplexNumbers(wtensor tensor, uint32_t nw, cl_event *be);
int wekuaTensorDisableComplexNumbers(wtensor, uint32_t nw, cl_event *be);

// Basic utils

// To alloc a tensor
wtensor wekuaTensorEmpty(wekuaContext ctx, uint8_t dtype, uint8_t com, uint32_t ndim, ...);

// To alloc a null tensor
wtensor wekuaAllocTensor(wekuaContext ctx, uint8_t dtype, uint8_t com, uint32_t ndim, ...);

// To get matrix with random elements
wtensor wekuaTensorRandn(wekuaContext ctx, uint8_t com, uint32_t ndim, ...);

// To get matrix with random numbers in the range [a, b) or [a, b] depending on rounding.
wtensor wekuaTensorRandUniform(wekuaContext ctx, uint8_t dtype, void *ra, void *ia, void *re, void *ie, uint32_t ndim, ...);

// To copy a tensor
wtensor wekuaTensorCopy(wtensor src, uint32_t nw, cl_event *be, cl_event *e);

// To change tensor data type
wtensor wekuaTensorConvert(wtensor old_tensor, uint8_t new_dtype, uint32_t nw, cl_event *be, cl_event *e);

// To copy buffer information to tensor
wtensor wekuaTensorFromBuffer(
	wekuaContext ctx, uint32_t ndim, ...
	// void *buffer, uint8_t dtype, uint8_t com
);

// To reshape a tensor
void wekuaTensorReshape(
	wtensor tensor, uint32_t ndim, ...
	// uint32_t nw, cl_event *be
);

// To fill a tensor with same elements. Alpha is real number and Beta is imaginary number
void wekuaFillTensor(
	wtensor tensor, uint32_t ndim, ...
	// void *alpha, void *beta,
	// uint32_t nw, cl_event *be, cl_event *e
);

#endif