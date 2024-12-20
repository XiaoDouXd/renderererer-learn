#pragma once

#include <list>
#include <vulkan/vulkan.hpp>
#include "vulkan/vulkan_handles.hpp"
#include "vulkan/vulkan_structs.hpp"

namespace XD::Render::Vk { class Semaphore; }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace XD::Render::Vk::PresMgr
{
    struct FrameData
    {
    public:
        vk::Fence                   fence;
        vk::Framebuffer             frameBuf;
        vk::Image                   img;
        vk::ImageView               imgView;
        vk::Semaphore               imgAcquired;
    };

    struct ShareData
    {
        FrameData*          currFrameData;
        vk::CommandPool     commandPool;
        vk::PipelineCache   pipelineCache;
        vk::RenderPass      mainRenderPass;
        vk::Extent2D        swapchainExtent;
    };

    /// @brief 获得开放数据
    /// @return shareData
    const ShareData& getShareData();

    /// @brief 初始化呈示管理器
    /// @param isClear 颜色缓冲是否清空
    void init(bool isClear);

    /// @brief 渲染前缀
    void begRender();

    /// @brief 上传指令队列
    void pushCommandBuffer(const vk::CommandBuffer& cmd);

    /// @brief 渲染后缀
    /// @param isRebuild 是否强制重建交换链
    void endRender(bool isRebuild = false);

    /// @brief 是否已初始化
    /// @return 是否已初始化
    bool inited() noexcept;

    /// @brief 获取下一帧缓冲成功后的信号量
    /// @return 下一帧缓冲成功的信号量
    vk::Semaphore getFrameAcquireSemaphore();

    /// @brief 获得信号量
    /// @return 信号量
    Semaphore requestSemaphore();

    /// @brief 销毁
    void destroy();
} // namespace XD

namespace XD::Render::Vk
{
    class Semaphore
    {
        friend Semaphore PresMgr::requestSemaphore();
    public:
        operator vk::Semaphore() const { return *_itr; } // NOLINT(google-explicit-constructor)
        std::strong_ordering operator<=>(const Semaphore& o) const { return (*_itr)<=>(*o._itr); }
        Semaphore& operator=(Semaphore&& o) noexcept { _itr = o._itr; return *this; }
        Semaphore(Semaphore && o) noexcept { _itr = o._itr; }
        operator bool() const; // NOLINT(google-explicit-constructor)
        void del();
        void set(const uint64_t& val = 0);

        Semaphore(const Semaphore&) = delete;
        Semaphore& operator=(const Semaphore&) = delete;

    private:
        Semaphore() = default;
        std::list<vk::Semaphore>::const_iterator _itr;
    };
}

#pragma clang diagnostic pop