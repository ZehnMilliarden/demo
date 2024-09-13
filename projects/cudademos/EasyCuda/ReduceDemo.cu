#include "ReduceDemo.cuh"
#include "cudaBase.cuh"
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <Windows.h>

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof(arr[0]))

struct HostCallbackData
{
    int* params = nullptr;
    int paramsize = 0;
    int* ret = nullptr;
};

__host__ cudaError_t ReduceMain()
{
    int nStreamSuccessSize = 0;
    cudaStream_t streamMain[2];

    cudaError_t cudaStatus = cudaError::cudaSuccess;

    int nHostEventSize = 0, nDeviceEventSize = 0;
    cudaEvent_t hostEvents[2], deviceEvent[2];

    do
    {
        cudaStatus = cudaStreamArrCreate(streamMain, ARRAY_SIZE(streamMain), nStreamSuccessSize);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = cudaEventArrCreate(hostEvents, ARRAY_SIZE(hostEvents), nHostEventSize);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = cudaEventArrCreate(deviceEvent, ARRAY_SIZE(deviceEvent), nDeviceEventSize);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        int params[1000] = {0};
        int paramCount = sizeof(params) / sizeof(int);
        for (int i = 0; i < paramCount; ++i)
        {
            params[i] = i;
        }
        int outHostRet = 0, outDeviceRet = 0;

        ReduceTestAtDevice(streamMain[0], deviceEvent[0], deviceEvent[1], params, paramCount, outDeviceRet);
        ReduceTestAtHost(streamMain[1], hostEvents[0], hostEvents[1], params, paramCount, outHostRet);

        ::cudaEventSynchronize(deviceEvent[1]);
        

        float elapsedDevice = 0, elapsedHost = 0;
        ::cudaEventElapsedTime(&elapsedDevice, deviceEvent[0], deviceEvent[1]);
        ::cudaEventElapsedTime(&elapsedHost, hostEvents[0], hostEvents[1]);

        std::cout << "Device Result : "<< outDeviceRet << " ,Elaspesd Time : " << elapsedDevice << std::endl;
        std::cout << "Host Result : " << outHostRet << " ,Elaspesd Time : " << elapsedHost << std::endl;


    } while (false);
    
    cudaStreamArrDestroy(streamMain, nStreamSuccessSize);
    cudaEventArrDestroy(hostEvents, nHostEventSize);
    cudaEventArrDestroy(deviceEvent, nDeviceEventSize);

    return cudaStatus;
}

__host__ cudaError_t ReduceTestAtHost(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int* params, int paramCount, int& outRet)
{
    cudaError_t cudaStatus = cudaError_t::cudaSuccess;

    do
    {

        HostCallbackData cbdata;
        cbdata.params = params;
        cbdata.paramsize = paramCount;
        cbdata.ret = &outRet;

        cudaStatus = ::cudaEventRecord(eventStart, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaStreamAddCallback(stream, ReduceHostCallback, &cbdata, 0);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaEventRecord(eventStop, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaEventSynchronize(eventStop);

    } while (false);

    return cudaStatus;
}

__host__ cudaError_t ReduceTestAtDevice(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int* params, int paramCount, int& outRet)
{
    cudaError_t cudaStatus = cudaError_t::cudaSuccess;
    int* deviceParam = nullptr;
    int* deviceOutput = nullptr;

    do
    {
        int paramSize = sizeof(int) * paramCount;
        cudaStatus = ::cudaMallocAsync(&deviceParam, paramSize,stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        const int nThreadCountOfBlock = 32;
        const int nBlockCount = (paramCount + nThreadCountOfBlock - 1) / nThreadCountOfBlock;

        cudaStatus = ::cudaMallocAsync(&deviceOutput, sizeof(int) * nBlockCount, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaMemcpyAsync(deviceParam, params, paramSize, cudaMemcpyKind::cudaMemcpyHostToDevice, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaEventRecord(eventStart, stream);
        
        ReduceDemoKernel<<<nBlockCount, nThreadCountOfBlock, 0, stream>>>(deviceParam, paramCount, deviceOutput);
        ReduceDemoKernel<<<1, nBlockCount, 0, stream >>>(deviceOutput, nBlockCount, deviceOutput);

        cudaStatus = ::cudaEventRecord(eventStop, stream);

        cudaStatus = ::cudaMemcpyAsync(&outRet, deviceOutput, sizeof(outRet), cudaMemcpyKind::cudaMemcpyDeviceToHost, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

    } while (false);

    return cudaStatus;
}

__global__ void ReduceDemoKernel(int* params, int paramCount, int* outputRet)
{
    int* realParams = params + blockIdx.x * blockDim.x;

    if ((blockIdx.x * blockDim.x + threadIdx.x) >= paramCount)
    {
        return;
    }

    for (int offset = blockDim.x >> 1; offset > 0; offset >>= 1)
    {
        if (threadIdx.x < offset)
        {
            realParams[threadIdx.x] += realParams[threadIdx.x + offset];
        }
        
        ::__syncthreads();
    }

    if (outputRet)
    {
        outputRet[blockIdx.x] = realParams[0];
    }
}

void CUDART_CB ReduceHostCallback(cudaStream_t stream, cudaError_t cudaStatus, void* pData)
{
    HostCallbackData* pCbData = (HostCallbackData*)pData;

    if (!pCbData)
    {
        return;
    }

    int nSum = 0;
    for (int i = 0; i < pCbData->paramsize; ++i)
    {
        nSum += pCbData->params[i];
    }

    if (pCbData->ret)
    {
        *(pCbData->ret) = nSum;
    }
}