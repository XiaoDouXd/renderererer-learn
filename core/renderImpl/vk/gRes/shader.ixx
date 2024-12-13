#include <cstdint>
#include "util/public/blob.h"
#include "util/public/exce.h"
#include "vulkan/vulkan.hpp"
#include "render/public/gRes/shader.h"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "shaderHolder.hpp"

#include "vk/vkMgr.h"
#include "vulkan/vulkan_structs.hpp"

#include "gResImpl.h"

namespace XD::Render
{
    static vk::ShaderStageFlagBits ToVkStageFlag(const Shader::StageInfo& info)
    {
        auto r = (vk::ShaderStageFlagBits)0x0;
        if ((uint32_t)info.stage & (uint32_t)Shader::Stage::Vertex)
            r = vk::ShaderStageFlagBits::eVertex;
        if ((uint32_t)info.stage & (uint32_t)Shader::Stage::Pixel)
            r = (vk::ShaderStageFlagBits)((uint32_t)r | VK_SHADER_STAGE_FRAGMENT_BIT);
        if ((uint32_t)info.stage & (uint32_t)Shader::Stage::Compute)
            r = (vk::ShaderStageFlagBits)((uint32_t)r | VK_SHADER_STAGE_COMPUTE_BIT);
        if ((uint32_t)info.stage & (uint32_t)Shader::Stage::Geometry)
            r = (vk::ShaderStageFlagBits)((uint32_t)r | VK_SHADER_STAGE_GEOMETRY_BIT);
        if ((uint32_t)info.stage == (uint32_t)Shader::Stage::Max)
            r = vk::ShaderStageFlagBits::eAllGraphics;
        return r;
    }

    Shader::Shader(const Blob& data, StageInfo type, const uuids::uuid& devId) : _shaderCode(data), _stageInfo(type)
    {
        auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo{};
        shaderModuleCreateInfo
            .setPCode(reinterpret_cast<const uint32_t*>(_shaderCode.data()))
            .setCodeSize(_shaderCode.size());

        auto& devPtr = Vk::VkMgr::getDev(devId);
        if (!devPtr) throw Exce(__LINE__, __FILE__, "XD::Shader Error: dev not found.");

        auto shaderModule = devPtr.dev.createShaderModule(shaderModuleCreateInfo);
        if (!shaderModule) throw Exce(__LINE__, __FILE__, "XD::Shader Error: failure to create shader module");

        auto& holder = XD_RENDER_GENERATE_HOLDER;
        holder.shaderModule = shaderModule;

        auto stageInfo = vk::PipelineShaderStageCreateInfo{};
        stageInfo
            .setModule(shaderModule)
            .setPName(_stageInfo.entranceFunction.c_str())
            .setStage(ToVkStageFlag(_stageInfo));
        holder.stageInfo = stageInfo;
    }

    Shader::Shader(Blob&& data, StageInfo type, const uuids::uuid& devId) : _shaderCode(data), _stageInfo(type)
    {
        auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo{};
        shaderModuleCreateInfo
            .setPCode(reinterpret_cast<const uint32_t*>(_shaderCode.data()))
            .setCodeSize(_shaderCode.size());

        auto& devPtr = Vk::VkMgr::getDev(devId);
        if (!devPtr) throw Exce(__LINE__, __FILE__, "XD::Shader Error: dev not found.");

        auto shaderModule = devPtr.dev.createShaderModule(shaderModuleCreateInfo);
        if (!shaderModule) throw Exce(__LINE__, __FILE__, "XD::Shader Error: failure to create shader module");

        auto& holder = XD_RENDER_GENERATE_HOLDER;
        holder.shaderModule = shaderModule;

        auto stageInfo = vk::PipelineShaderStageCreateInfo{};
        stageInfo
            .setModule(shaderModule)
            .setPName(_stageInfo.entranceFunction.c_str())
            .setStage(ToVkStageFlag(_stageInfo));
        holder.stageInfo = stageInfo;
    }

    Shader::Shader(const std::tuple<uint8_t*, size_t>& data, StageInfo type, const uuids::uuid& devId) : Shader(Blob(data), type, devId)
    {}

    XD_VK_RENDER_GENERATE_HOLDER_GETTER_IMPL(Shader)

    Shader::~Shader()
    {
        if (!_holder) return;
        auto& devPtr = Vk::VkMgr::getDev(_devId);
        auto& dev = devPtr.dev;
        auto& holder = getResHolder<gResType>(_holder);
        dev.destroyShaderModule(holder.shaderModule);
    }
}