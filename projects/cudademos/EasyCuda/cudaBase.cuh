#pragma once

#include "cuda_runtime.h"

__host__ cudaError_t cudaSelectDevice();

__host__ cudaError_t cudaStreamArrCreate(cudaStream_t* streams, int size, int& outSize);
__host__ cudaError_t cudaStreamArrDestroy(cudaStream_t* streams, int size);

__host__ cudaError_t cudaEventArrCreate(cudaEvent_t* events, int size, int& outSize);
__host__ cudaError_t cudaEventArrDestroy(cudaEvent_t* events, int size);