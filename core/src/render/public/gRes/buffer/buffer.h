#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

#include <memory>
#include "render/public/gRes/gRes.h"

namespace XD::Render
{
    class Buffer : public GRes
    {
    public:
        XD_RENDER_GENERATE_HOLDER_GETTER(GResType::Buffer);

    public:
        /// @brief 总大小
        [[nodiscard]] virtual size_t size() const = 0;

        /// @brief 设备 id
        [[nodiscard]] uuids::uuid getDevId() const { return _devId; }

    protected:
        explicit Buffer(const uuids::uuid& devId);
        ~Buffer() override = default;

        std::unique_ptr<GResHolderBase> _holder = nullptr;
        size_t                          _size{};
        uuids::uuid                     _devId;
    };
} // namespace XD::Render

#pragma clang diagnostic pop