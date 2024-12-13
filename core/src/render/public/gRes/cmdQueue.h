
#include "buffer/indexBuffer.h"
#include "pipeline.h"
#include "render/public/gRes/buffer/vertexBuffer.hpp"
#include "render/public/gRes/gRes.h"

namespace XD::Render
{
    class CmdQueue : public GRes
    {
        XD_RENDER_GENERATE_HOLDER_GETTER(GResType::CmdQueue);
    public:
        CmdQueue();

        void Begin();
        void SetPipline(const Pipeline& pipeline);
        void Draw(const IndexBuffer& indexBuffer, const VertexBufferBase& vertexBuffer);
        void End();
        void Present();

        ~CmdQueue();

    private:
        bool _recording;
        std::unique_ptr<GResHolderBase> _holder = nullptr;
    };
}