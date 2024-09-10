#include "deviceInfo.cuh"
#include "device_launch_parameters.h"
#include "cuda_runtime.h"
#include "cuda.h"

#include <iostream>

void dumpDeviceInfo()
{
    int deviceCount = 0;
    ::cudaGetDeviceCount(&deviceCount);

    for (int i = 0; i < deviceCount; ++i)
    {
        cudaDeviceProp deviceProp;
        cudaError_t err = ::cudaGetDeviceProperties(&deviceProp, i);

        std::cout << "Device Name : " << deviceProp.name << std::endl;
        std::cout << "SM Version: " << deviceProp.major << "." << deviceProp.minor << std::endl;
        std::cout << "Total Mem: " << deviceProp.totalGlobalMem << std::endl;
        std::cout << "Num Of Sms: " << deviceProp.multiProcessorCount << std::endl;
        std::cout << "Total Constant Mem: " << deviceProp.totalConstMem << std::endl;
        std::cout << "Max Thread Count of SM: " << deviceProp.maxThreadsPerMultiProcessor << std::endl;
        std::cout << "Max Thread Count of Block: " << deviceProp.maxThreadsPerBlock << std::endl;

        int nDeviceSupportsVmm = 0;
        err =  ::cudaDeviceGetAttribute(&nDeviceSupportsVmm, static_cast<cudaDeviceAttr>(CU_DEVICE_ATTRIBUTE_VIRTUAL_MEMORY_MANAGEMENT_SUPPORTED), i);
        if (nDeviceSupportsVmm != 0)
        {
            printf("CUDA Support UVM\n");
        }

        int nDeviceSupportsCompressMem = 0;
        err = ::cudaDeviceGetAttribute(&nDeviceSupportsCompressMem, static_cast<cudaDeviceAttr>(CU_DEVICE_ATTRIBUTE_GENERIC_COMPRESSION_SUPPORTED), i);
        if (nDeviceSupportsCompressMem != 0)
        {
            printf("CUDA Support Compress Memory\n");
        }
    }
}

__global__ void calcByKernel(int* param1, int* param2, int* outPut)
{
    int nIndex = threadIdx.x;
    outPut[nIndex] = param1[nIndex] * param2[nIndex];
}

__host__ cudaError_t streamDemo()
{

    const int arraySize = 5;
    int a[arraySize] = { 1, 2, 3, 4, 5 };
    int b[arraySize] = { 10, 20, 30, 40, 50 };
    int c[arraySize] = { 0 };

    cudaError_t errRet = cudaError_t::cudaSuccess;
    int* pDeviceParam1 = nullptr;
    int* pDeviceParam2 = nullptr;
    int* pDeviceParam3 = nullptr;

    cudaStream_t arrStream[1] = { nullptr };

    do
    {
        int deviceCount = 0;
        errRet = cudaStreamCreate(&arrStream[0]);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }


        errRet = ::cudaMalloc(&pDeviceParam1, arraySize);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }
        
        errRet = ::cudaMalloc(&pDeviceParam2, arraySize);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        errRet = ::cudaMalloc(&pDeviceParam3, arraySize);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        errRet = ::cudaMemcpyAsync(pDeviceParam1, a, arraySize, cudaMemcpyHostToDevice, arrStream[0]);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        errRet = ::cudaMemcpyAsync(pDeviceParam2, b, arraySize, cudaMemcpyHostToDevice, arrStream[0]);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        calcByKernel<<<1, arraySize, 0, arrStream[0] >>>(pDeviceParam1, pDeviceParam2, pDeviceParam3);

        errRet = ::cudaStreamSynchronize(arrStream[0]);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        errRet = ::cudaMemcpyAsync(c, pDeviceParam3, arraySize, cudaMemcpyDeviceToHost, arrStream[0]);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

        errRet = cudaError_t::cudaSuccess;

    } while (false);
    
    if (pDeviceParam1)
    {
        ::cudaFree(pDeviceParam1);
        pDeviceParam1 = nullptr;
    }
    if (pDeviceParam2)
    {
        ::cudaFree(pDeviceParam2);
        pDeviceParam2 = nullptr;
    }
    if (pDeviceParam3)
    {
        ::cudaFree(pDeviceParam3);
        pDeviceParam3 = nullptr;
    }
    if (arrStream[0])
    {
        ::cudaStreamDestroy(arrStream[0]);
        arrStream[0] = nullptr;
    }

    printf("{1,2,3,4,5} + {10,20,30,40,50} = {%d,%d,%d,%d,%d}\n",
        c[0], c[1], c[2], c[3], c[4]);

    return errRet;
}