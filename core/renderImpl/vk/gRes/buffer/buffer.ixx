#include <memory>

#include "bufferResHolder.hpp"
#include "render/public/gRes/buffer/buffer.h"
#include "render/public/gRes/gRes.h"
#include "vk/gRes/gResImpl.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

namespace XD::Render
{
    Buffer::Buffer(const uuids::uuid& devId)
    {
        _devId = devId;
        XD_RENDER_GENERATE_HOLDER;
    }

    XD_VK_RENDER_GENERATE_HOLDER_GETTER_IMPL(Buffer)
}
#pragma clang diagnostic pop