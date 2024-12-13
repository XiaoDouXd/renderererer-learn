
#include <array>
#include "cmdQueueHolder.hpp"
#include "render/public/gRes/cmdQueue.h"
#include "util/public/exce.h"
#include "util/public/log.h"
#include "vk/presMgr.h"
#include "vk/vkMgr.h"
#include "vulkan/vulkan.hpp"
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

#include "pipelineHolder.hpp"
#include "buffer/bufferResHolder.hpp"

#include "gResImpl.h"

namespace XD::Render
{
    XD_VK_RENDER_GENERATE_HOLDER_GETTER_IMPL(CmdQueue)

    CmdQueue::CmdQueue()
    {
        auto &holder = XD_RENDER_GENERATE_HOLDER;
        auto &dev = Vk::VkMgr::getDev();
        auto &shareData = Vk::PresMgr::getShareData();

        auto &commandPool = shareData.commandPool;
        auto commandBufferAllocateInfo = vk::CommandBufferAllocateInfo{};
        commandBufferAllocateInfo
            .setCommandBufferCount(1)
            .setCommandPool(commandPool)
            .setLevel(vk::CommandBufferLevel::ePrimary);
        
        auto commmandBuffer = dev.dev.allocateCommandBuffers(commandBufferAllocateInfo);
        
        if (commmandBuffer.empty()) throw Exce(__LINE__, __FILE__, "XD::Render::CmdQueue Exce: faliure to allocate cmd buffer");
        holder.cmdBuffer = commmandBuffer[0];
        _recording = false;
    }

    void CmdQueue::Begin()
    {
        if (_recording)
        {
            XD::Log::logWarning("XD::Render::CmdQueue Warning: recall begin");
            return;
        }
        _recording = true;
        auto &holder = getResHolder<gResType>(_holder);
        auto beginInfo = vk::CommandBufferBeginInfo{};
        beginInfo.setPInheritanceInfo({});
        holder.cmdBuffer.begin(beginInfo);

        auto &shareData = Vk::PresMgr::getShareData();
        auto beginRenderPassInfo = vk::RenderPassBeginInfo{};
        auto renderArea = vk::Rect2D{};
        std::array<vk::ClearValue, 2> clearValues = {};
        clearValues[0].setColor({0.0f, 0.0f, 0.0f, 0.0f});
        clearValues[1].setDepthStencil({1.0f, 0});

        beginRenderPassInfo
            .setRenderPass(shareData.mainRenderPass)
            .setFramebuffer(shareData.currFrameData->frameBuf)
            .setRenderArea(renderArea.setOffset({0, 0}).setExtent(shareData.swapchainExtent))
            .setClearValueCount(clearValues.size())
            .setClearValues(clearValues);
        holder.cmdBuffer.beginRenderPass(&beginRenderPassInfo, vk::SubpassContents::eInline);
    }

    void CmdQueue::End()
    {
        if (!_recording)
        {
            XD::Log::logWarning("XD::Render::CmdQueue Warning: recall end");
            return;
        }
        _recording = false;
        auto &holder = getResHolder<gResType>(_holder);
        holder.cmdBuffer.endRenderPass();
        holder.cmdBuffer.end();
    }

    void CmdQueue::SetPipline(const Pipeline& pipeline)
    {
        if (!_recording)
        {
            XD::Log::logWarning("XD::Render::CmdQueue Warning: try set cmd while not begin");
            return;
        }
        auto &holder = getResHolder<gResType>(_holder);
        auto &pipelineHolder = pipeline.getImplInfo();

        holder.cmdBuffer.setViewport(0, pipelineHolder.viewport);
        holder.cmdBuffer.setScissor(0, 1, &pipelineHolder.scissor);
        holder.cmdBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, pipelineHolder.pipeline);
    }

    void CmdQueue::Draw(const IndexBuffer& indexBuffer, const VertexBufferBase& vertexBuffer)
    {
        if (!_recording)
        {
            XD::Log::logWarning("XD::Render::CmdQueue Warning: try set cmd while not begin");
            return;
        }
        auto &holder = getResHolder<gResType>(_holder);
        auto &vertexBufferHolder = vertexBuffer.getImplInfo();
        auto &indexBufferHolder = indexBuffer.getImplInfo();

        vk::DeviceSize offsets[] { 0 };
        vk::Buffer buf[] { vertexBufferHolder.buf };
        holder.cmdBuffer.bindIndexBuffer(indexBufferHolder.buf, 0, vk::IndexType::eUint32);
        holder.cmdBuffer.bindVertexBuffers(0, 1, buf, offsets);
        // holder.cmdBuffer.draw(vertexBuffer.vertCnt(), 1, 0, 0);
        holder.cmdBuffer.drawIndexed(indexBuffer.size(), 1, 0, 0, 0);
    }

    void CmdQueue::Present()
    {
        if (_recording)
        {
            XD::Log::logWarning("XD::Render::CmdQueue Warning: try to present none end queue");
            return;
        }
        auto &holder = getResHolder<gResType>(_holder);
        Vk::PresMgr::pushCommandBuffer(holder.cmdBuffer);
    }

    CmdQueue::~CmdQueue()
    {   
        if (!_holder) return;
        auto &dev = Vk::VkMgr::getDev();
        auto &shareData = Vk::PresMgr::getShareData();
        auto &holder = getResHolder<gResType>(_holder);
        dev.dev.freeCommandBuffers(shareData.commandPool, 1, &holder.cmdBuffer);
    }
}