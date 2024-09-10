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

        int params[] = {0,1,2,3,4,5};
        int paramSize = sizeof(params) / sizeof(int);
        int outHostRet = 0, outDeviceRet = 0;

        ReduceTestAtDevice(streamMain[0], deviceEvent[0], deviceEvent[1], params, paramSize, outDeviceRet);
        ReduceTestAtHost(streamMain[1], hostEvents[0], hostEvents[1], params, paramSize, outHostRet);

        ::cudaEventSynchronize(deviceEvent[1]);
        ::cudaEventSynchronize(hostEvents[1]);

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

__host__ cudaError_t ReduceTestAtHost(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int* params, int paramSize, int& outRet)
{
    cudaError_t cudaStatus = cudaError_t::cudaSuccess;

    do
    {

        HostCallbackData cbdata;
        cbdata.params = params;
        cbdata.paramsize = paramSize;
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

    } while (false);

    return cudaStatus;
}

__host__ cudaError_t ReduceTestAtDevice(const cudaStream_t& stream, const cudaEvent_t& eventStart, const cudaEvent_t& eventStop, int* params, int paramSize, int& outRet)
{
    cudaError_t cudaStatus = cudaError_t::cudaSuccess;
    int* deviceParam = nullptr;
    int* deviceOutput = nullptr;

    do
    {
        cudaStatus = ::cudaMallocAsync(&deviceParam, paramSize,stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

        cudaStatus = ::cudaMallocAsync(&deviceOutput, sizeof(int), stream);
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

        ReduceDemoKernel<<<1, paramSize, 0, stream>>>(deviceParam, paramSize, deviceOutput);

        cudaStatus = ::cudaEventRecord(eventStop, stream);

        cudaStatus = ::cudaMemcpyAsync(&outRet, deviceOutput, sizeof(outRet), cudaMemcpyKind::cudaMemcpyDeviceToHost, stream);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            break;
        }

    } while (false);

    return cudaStatus;
}

__global__ void ReduceDemoKernel(int* params, int paramSize, int* outputRet)
{
    printf("ReduceDemoKernel >> Current thread ID: %d\n", threadIdx.x);
}

void CUDART_CB ReduceHostCallback(cudaStream_t stream, cudaError_t cudaStatus, void* pData)
{
    HostCallbackData* pCbData = (HostCallbackData*)pData;
    printf("ReduceHostCallback >> Current thread ID: %d\n", ::GetCurrentThreadId());
}