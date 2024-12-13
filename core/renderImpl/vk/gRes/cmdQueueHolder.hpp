#include "vulkan/vulkan.hpp"
#include "render/public/gRes/gRes.h"
#include "vulkan/vulkan_handles.hpp"

namespace XD::Render
{
    template<>
    class GResHolder<GResType::CmdQueue> : public GResHolderBase
    {
    public:
        vk::CommandBuffer cmdBuffer;
    };
}