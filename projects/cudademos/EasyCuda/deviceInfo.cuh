
#include "cuda_runtime.h"

void dumpDeviceInfo();

__global__ void calcByKernel(int* param1, int* param2, int* outPut);

__host__ cudaError_t streamDemo();