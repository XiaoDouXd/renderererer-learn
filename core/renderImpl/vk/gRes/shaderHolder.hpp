
#include "vulkan/vulkan.hpp"
#include "render/public/gRes/gRes.h"

namespace XD::Render
{
    template<>
    class GResHolder<GResType::Shader> : public GResHolderBase
    {
    public:
        vk::ShaderModule    shaderModule;
        vk::PipelineShaderStageCreateInfo stageInfo;
    };
}