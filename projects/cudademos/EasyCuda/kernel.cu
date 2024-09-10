
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>

#include "cudaBase.cuh"
#include "deviceInfo.cuh"
#include "ReduceDemo.cuh"

int main()
{     
    dumpDeviceInfo();

    cudaError_t cudaStatus = cudaError_t::cudaSuccess;
    // Add vectors in parallel.
    do
    {
        cudaStatus = cudaSelectDevice();
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cudaSelectDevice failed!");
            break;
        }

        cudaStatus = streamDemo();
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "streamDemo failed!");
            break;
        }

        cudaStatus = ReduceMain();
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "ReduceMain failed!");
            break;
        }

        cudaStatus = cudaDeviceReset();
        if (cudaStatus != cudaSuccess) {
            fprintf(stderr, "cudaDeviceReset failed!");
            break;
        }

    } while (false);

    return 0;
}