#pragma once

#include "Foundation/Foundation.hpp"
#include "Metal/Metal.hpp"
#include "QuartzCore/QuartzCore.hpp"
#include "observer.hpp"

class Model : public Observable {
public:
    Model(unsigned int width, unsigned int height)
        : _width(width), _height(height) {
        // to catch errors
        NS::Error* err;

        // Get the device
        _device = MTL::CreateSystemDefaultDevice();

        // Can use default library since Xcode is not retarded
        _defLib = _device->newLibrary(NS::String::string("default.metallib", NS::UTF8StringEncoding), &err);

        // Select the add function from the library.
        // This is just a proxy, not actual executable code
        _GOLFunc = _defLib->newFunction(NS::String::string("add_arrays", NS::UTF8StringEncoding));

        // A pipeline specifies the steps that the GPU performs to complete a specific task.
        // In Metal, a pipeline is represented by a pipeline state object.
        // PSO == Pipeline State Object.
        // This does also compile the function code for the specified GPU.
        // That takes a lil bit of time, so don't create state objects mid sensitive tasks.
        _GOLFuncPSO = _device->newComputePipelineState(_GOLFunc, &err);

        // You need a command queue to send work to the GPU.
        // Metal uses command queues to schedule commands
        _queue = _device->newCommandQueue();

        // Create some buffers for data. Buffers don't have a type, so care about that manually.
        // Floats are 4 bytes, so quadruple your size for allocation.
        _ABuf = _device->newBuffer(width * height * sizeof(bool), MTL::ResourceStorageModeShared);
        _BBuf = _device->newBuffer(width * height * sizeof(bool), MTL::ResourceStorageModeShared);

        // Data pointers for better access
        _aDataPtr = (bool*)_ABuf->contents();
        _bDataPtr = (bool*)_BBuf->contents();
    }

    ~Model() {
        // release all Metal objects
        _device->release();
        _defLib->release();
        _GOLFunc->release();
        _GOLFuncPSO->release();
        _queue->release();
        _ABuf->release();
        _BBuf->release();
    }

    // advance the game one step
    void update() {
        // to manage object lifecycle
        NS::AutoreleasePool* pPool = NS::AutoreleasePool::alloc()->init();

        // make buffer and encoder, to create command
        MTL::CommandBuffer* commandBuffer = _queue->commandBuffer();
        MTL::ComputeCommandEncoder* encoder = commandBuffer->computeCommandEncoder();

        // shader settings
        encoder->setComputePipelineState(_GOLFuncPSO);
        encoder->setBuffer(_ABuf, 0, 0);
        encoder->setBuffer(_BBuf, 0, 1);
        encoder->setBytes(&_width, sizeof(_width), 2);
        encoder->setBytes(&_height, sizeof(_height), 3);
        encoder->setBytes(&_AtoB, sizeof(_AtoB), 4);

        // make grid
        MTL::Size gridSize(_width * _height, 1, 1);
        NS::UInteger nThreads = _GOLFuncPSO->maxTotalThreadsPerThreadgroup();
        if (nThreads > _width * _height)
            nThreads = _width * _height;
        MTL::Size threadGroupSize(nThreads, 1, 1);
        encoder->dispatchThreads(gridSize, threadGroupSize);

        // execute
        encoder->endEncoding();
        commandBuffer->commit();

        // switch direction
        _AtoB = !_AtoB;

        // wait for completion
        commandBuffer->waitUntilCompleted();

        // update observers
        notifyUpdate();

        // release pool and therefore all allocated objects
        pPool->release();
    }

    bool getData(int x, int y) {
        return _data()[y * _width + x];
    }

    void setData(int x, int y, bool state) {
        _data()[y * _width + x] = state;
        notifyUpdate();
    }

    void printData() {
        for (int y = 0; y < _height; ++y) {
            for (int x = 0; x < _width; ++x)
                std::cout << (_data()[y * _width + x] ? 'X' : '-') << ' ';
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    unsigned int getWidth() {
        return _width;
    }

    unsigned int getHeight() {
        return _height;
    }

private:
    const unsigned int _width, _height;
    bool _AtoB = true;

    MTL::Device* _device = nullptr;
    MTL::Library* _defLib = nullptr;
    MTL::Function* _GOLFunc = nullptr;
    MTL::ComputePipelineState* _GOLFuncPSO = nullptr;
    MTL::CommandQueue* _queue = nullptr;

    MTL::Buffer* _ABuf = nullptr;
    MTL::Buffer* _BBuf = nullptr;
    bool* _aDataPtr = nullptr;
    bool* _bDataPtr = nullptr;

    bool* _data() {
        if (_AtoB)
            return _aDataPtr;
        else
            return _bDataPtr;
    }
};
