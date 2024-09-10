#include "cudaBase.cuh"

__host__ cudaError_t cudaSelectDevice()
{
    cudaError_t errRet = cudaError::cudaSuccess;
    int deviceCount = 0;

    do
    {
        errRet = ::cudaGetDeviceCount(&deviceCount);
        if (errRet != cudaError_t::cudaSuccess || deviceCount <= 0)
        {
            errRet = cudaError_t::cudaErrorNoDevice;
            break;
        }

        errRet = ::cudaSetDevice(0);
        if (errRet != cudaError_t::cudaSuccess)
        {
            break;
        }

    } while (false);

    return errRet;
}

__host__ cudaError_t cudaStreamArrCreate(cudaStream_t* streams, int size, int& outSize)
{
    for (int i = 0; i < size; ++i)
    {
        cudaError_t cudaStatus = ::cudaStreamCreate(streams + i);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            outSize = i;
            return cudaStatus;
        }
    }

    outSize = size;
    return cudaError::cudaSuccess;
}

__host__ cudaError_t cudaStreamArrDestroy(cudaStream_t* streams, int size)
{
    for (int i = 0; i < size; ++i)
    {
        ::cudaStreamDestroy(streams[i]);
    }

    return cudaError_t::cudaSuccess;
}

__host__ cudaError_t cudaEventArrCreate(cudaEvent_t* events, int size, int& outSize)
{
    for (int i = 0; i < size; ++i)
    {
        cudaError_t cudaStatus = ::cudaEventCreate(events + i);
        if (cudaStatus != cudaError_t::cudaSuccess)
        {
            outSize = i;
            return cudaStatus;
        }
    }

    outSize = size;
    return cudaError::cudaSuccess;
}

__host__ cudaError_t cudaEventArrDestroy(cudaEvent_t* events, int size)
{
    for (int i = 0; i < size; ++i)
    {
        ::cudaEventDestroy(events[i]);
    }

    return cudaError_t::cudaSuccess;
}