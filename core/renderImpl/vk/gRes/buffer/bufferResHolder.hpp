
#pragma once

#include <vulkan/vulkan.hpp>

#include "render/public/gRes/gRes.h"

namespace XD::Render
{
    template<>
    class GResHolder<GResType::Buffer> : public GResHolderBase
    {
    public:
        vk::DeviceMemory    devMem;
        vk::Buffer          buf;
    };

    template<>
    class GResHolder<GResType::BufferLayout> : public GResHolderBase
    {
    public:
        std::vector<vk::VertexInputAttributeDescription> attrs;
        vk::VertexInputBindingDescription binding;
    };
}