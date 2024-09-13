#pragma once

#include "cuda_runtime.h"

__host__ cudaError_t ReduceMain();

__host__ cudaError_t ReduceTestAtHost(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int * params, int paramCount, int& outRet);
__host__ cudaError_t ReduceTestAtDevice(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int* params, int paramCount, int& outRet);

__global__ void ReduceDemoKernel(int* params, int paramCount, int* outputRet);

void CUDART_CB ReduceHostCallback(cudaStream_t stream, cudaError_t cudaStatus, void* pData);