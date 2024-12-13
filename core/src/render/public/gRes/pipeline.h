#pragma once

#include <vector>
#include "fwd.hpp"
#include "render/public/gRes/buffer/bufferLayout.hpp"
#include "render/public/gRes/gRes.h"
#include "shader.h"
#include "uuid.h"

namespace XD::Render
{
    class Pipeline final : public GRes
    {
    public:
        XD_RENDER_GENERATE_HOLDER_GETTER(GResType::Pipeline);

        struct ViewportInfo
        {
            glm::f32 x;
            glm::f32 y;
            glm::f32 width;
            glm::f32 height;
            glm::f32 minDepth;
            glm::f32 maxDepth;

            glm::i32 scissorOffsetX;
            glm::i32 scissorOffsetY;
        };

    public:
        Pipeline(const std::vector<Shader*>& shader, const BufferLayoutBase& layoutBase, const ViewportInfo& viewportInfo = {}, const uuids::uuid uuid = {});
        ~Pipeline();

    private:
        std::unique_ptr<GResHolderBase> _holder = nullptr;
    };
}