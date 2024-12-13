#pragma once

#include <string>
#include "render/public/gRes/gRes.h"
#include "util/public/blob.h"

namespace XD::Render
{
    class Shader : public GRes
    {
    public:
        enum class Stage : uint32_t
        {
            Undefined       = 0x0,

            // -----------------------------
            Vertex          = 0x00000001,
            Geometry        = 0x00000002,
            Pixel           = 0x00000004,
            Compute         = 0x00000008,
            // -----------------------------

            Max             = 0x7FFFFFFF,
        };

        struct StageInfo
        {
            Stage stage;
            std::string entranceFunction;
        };

    public:
        XD_RENDER_GENERATE_HOLDER_GETTER(GResType::Shader);

    public:
        Shader(const Blob& data, StageInfo stage, const uuids::uuid& devId = {});
        Shader(Blob&& data, StageInfo stage, const uuids::uuid& devId = {});
        Shader(const std::tuple<uint8_t*, size_t>& data, StageInfo stage, const uuids::uuid& devId = {});
        ~Shader();

    private:
        uuids::uuid     _devId;
        Blob            _shaderCode;
        StageInfo       _stageInfo;
        std::unique_ptr<GResHolderBase> _holder = nullptr;
    };
}