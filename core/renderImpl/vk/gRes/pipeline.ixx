#include <vector>
#include "render/public/gRes/pipeline.h"
#include "vk/presMgr.h"
#include "vk/vkMgr.h"
#include "vulkan/vulkan_enums.hpp"
#include "vulkan/vulkan_structs.hpp"

#include "pipelineHolder.hpp"
#include "shaderHolder.hpp"
#include "buffer/bufferResHolder.hpp"

#include "gResImpl.h"

namespace XD::Render
{
    XD_VK_RENDER_GENERATE_HOLDER_GETTER_IMPL(Pipeline)

    Pipeline::Pipeline(const std::vector<Shader*>& shader, const BufferLayoutBase& layoutBase, const ViewportInfo& viewportInfo, const uuids::uuid uuid)
    {
        auto &holder = XD_RENDER_GENERATE_HOLDER;
        auto pipelineInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo{};
        pipelineInputAssemblyStateCreateInfo
            .setTopology(vk::PrimitiveTopology::eTriangleList)
            .setPrimitiveRestartEnable(false);

        auto &dev = Vk::VkMgr::getDev(uuid);
        auto &shareData = Vk::PresMgr::getShareData();
        auto viewport = vk::Viewport{};
        viewport
            .setHeight(viewportInfo.height <= 0.0f ? static_cast<float>(shareData.swapchainExtent.height) : viewportInfo.height)
            .setWidth(viewport.width <= 0.0f ? static_cast<float>(shareData.swapchainExtent.width) : viewportInfo.width)
            .setMaxDepth(1.0f)
            .setMinDepth(0.0f)
            .setX(viewportInfo.x)
            .setY(viewportInfo.y);

        if (viewportInfo.maxDepth != viewportInfo.minDepth)
        {
            viewport
                .setMaxDepth(viewportInfo.maxDepth)
                .setMinDepth(viewportInfo.minDepth);
        }

        auto scissor = vk::Rect2D{};
        scissor.setOffset({viewportInfo.scissorOffsetX, viewportInfo.scissorOffsetY});
        scissor.setExtent(shareData.swapchainExtent);

        holder.scissor = scissor;
        holder.viewport = viewport;

        auto pipelineViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo{};
        pipelineViewportStateCreateInfo
            .setViewports(viewport)
            .setScissors(scissor);
        
        auto pipelineRasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo{};
        pipelineRasterizationStateCreateInfo
            .setDepthClampEnable(false)
            .setRasterizerDiscardEnable(false)
            .setPolygonMode(vk::PolygonMode::eFill)
            .setLineWidth(1.0f)
            .setCullMode(vk::CullModeFlagBits::eNone)
            .setFrontFace(vk::FrontFace::eClockwise)
            .setDepthBiasClamp(false);
        
        auto pipelineMultisampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo{};
        pipelineMultisampleStateCreateInfo
            .setSampleShadingEnable(false)
            .setRasterizationSamples(vk::SampleCountFlagBits::e1);
        
        auto pipelineDepthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo{};
        pipelineDepthStencilStateCreateInfo
            .setDepthTestEnable(true)
            .setDepthWriteEnable(true)
            .setDepthCompareOp(vk::CompareOp::eLess)
            .setDepthBoundsTestEnable(false)
            .setStencilTestEnable(false);

        auto pipelineColorBlendAttachmentState = vk::PipelineColorBlendAttachmentState{};
        pipelineColorBlendAttachmentState
            .setBlendEnable(false)
            .setColorWriteMask(vk::ColorComponentFlagBits::eA | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eR);

        auto pipelineColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo{};
        pipelineColorBlendStateCreateInfo
            .setLogicOpEnable(false)
            .setAttachments(pipelineColorBlendAttachmentState);

        auto stages = std::vector<vk::PipelineShaderStageCreateInfo>{};
        for (const auto& stage : shader)
            stages.emplace_back(stage->getImplInfo().stageInfo);

        auto &layout = layoutBase.getImplInfo();
        auto pipelineVertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo{};
        pipelineVertexInputStateCreateInfo
            .setVertexBindingDescriptions(layout.binding)
            .setVertexAttributeDescriptions(layout.attrs);

        auto pipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo{};
        holder.layout = dev.dev.createPipelineLayout(pipelineLayoutCreateInfo);

        auto gfxPipelineCreateInfo = vk::GraphicsPipelineCreateInfo{};
        gfxPipelineCreateInfo
            .setStages(stages)
            .setPVertexInputState(&pipelineVertexInputStateCreateInfo)
            .setPInputAssemblyState(&pipelineInputAssemblyStateCreateInfo)
            .setPViewportState(&pipelineViewportStateCreateInfo)
            .setPRasterizationState(&pipelineRasterizationStateCreateInfo)
            .setPMultisampleState(&pipelineMultisampleStateCreateInfo)
            .setPDepthStencilState(&pipelineDepthStencilStateCreateInfo)
            .setPColorBlendState(&pipelineColorBlendStateCreateInfo)
            .setPDynamicState(nullptr)
            .setLayout(holder.layout)
            .setRenderPass(shareData.mainRenderPass)
            .setSubpass(0);
        auto res = dev.dev.createGraphicsPipelines(shareData.pipelineCache, gfxPipelineCreateInfo);
        Vk::VkMgr::checkVkResult(res.result);
        holder.pipeline = res.value[0];
    }

    Pipeline::~Pipeline()
    {
        if (!_holder) return;
        auto &dev = Vk::VkMgr::getDev().dev;
        auto &holder = getResHolder<gResType>(_holder);
        dev.destroy(holder.layout);
        dev.destroy(holder.pipeline);
    }
}