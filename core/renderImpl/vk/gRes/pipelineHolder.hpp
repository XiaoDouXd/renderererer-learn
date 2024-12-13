#include "vulkan/vulkan.hpp"
#include "render/public/gRes/gRes.h"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace XD::Render
{
    template<>
    class GResHolder<GResType::Pipeline> : public GResHolderBase
    {
    public:
        vk::Pipeline    pipeline;
        vk::PipelineLayout layout;

        vk::Rect2D scissor;
        vk::Viewport viewport;
    };
}