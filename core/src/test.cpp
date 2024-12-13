
#include <cstdint>
#include <initializer_list>
#include <vector>

#include "render/private/gRes/buffer/vertexBuffer.hxx"
#include "render/public/gRes/buffer/indexBuffer.h"
#include "render/public/gRes/buffer/vertexBuffer.hpp"
#include "render/public/gRes/pipeline.h"
#include "render/public/gRes/cmdQueue.h"
#include "render/public/gRes/shader.h"
#include "trigonometric.hpp"
#include "util/public/blob.h"

using namespace XD::Render;

namespace XD
{
    // -------------------------------------
    using v = Vertex<UvBufferLayout>;
    using vBuf = VertexBuffer<UvBufferLayout>;
    vBuf* vertBuf = nullptr;
    IndexBuffer* indBuf = nullptr;

    Shader* pixelShader = nullptr;
    Shader* vertexShader = nullptr;

    Pipeline* pipeline = nullptr;
    CmdQueue* cmdQueue = nullptr;
    uint64_t t = 0;
    // -------------------------------------

    /// @param state 0-init | 1-update | destory
    void Test(int8_t state)
    {

        if (state == 0) // init
        {
            // 测试顶点缓冲用的块
            auto vertList = std::list<v>{};
            vertList.insert(vertList.end(), v{}.set<0>({-0.5f, 0.5f, 0.1f}).set<1>({0.0f, 1.0f}).set<2>({0, 255, 255, 255}));
            vertList.insert(vertList.end(), v{}.set<0>({0.5f, 0.5f, 0.1f}).set<1>({1.0f, 1.0f}).set<2>({255, 0, 255, 255}));
            vertList.insert(vertList.end(), v{}.set<0>({0.5f, -0.5f, 0.1f}).set<1>({1.0f, 0.0f}).set<2>({255, 255, 0, 255}));
            vertList.insert(vertList.end(), v{}.set<0>({-0.5f, -0.5f, 0.1f}).set<1>({0.0f, 0.0f}).set<2>({255, 255, 255, 255}));
            vertBuf = new vBuf(vertList, true);
            auto indices = std::vector<uint32_t>{0, 1, 2, 0, 2, 3};
            indBuf = new IndexBuffer(indices);

            vertexShader = new Shader(Blob("../res/vert.spv"), {
                .stage = Shader::Stage::Vertex,
                .entranceFunction = "main",
            });
            pixelShader = new Shader(Blob("../res/frag.spv"), {
                .stage = Shader::Stage::Pixel,
                .entranceFunction = "main",
            });
            auto shaderList = std::vector<Shader*>({vertexShader, pixelShader});
            auto layout = UvBufferLayout::getLayout();
            pipeline = new Pipeline(shaderList, layout);

            cmdQueue = new CmdQueue();
        }
        else if (state == 1) // update
        {
            t++;
            auto x1 = glm::cos(t * 0.3f + 1) * 0.01f;
            auto y1 = glm::sin(t * 0.4f + 2) * 0.01f;
            auto x2 = glm::cos(t * 0.3f - 2) * 0.01f;
            auto y2 = glm::cos(t * 0.3f + 8) * 0.01f;
            auto x3 = glm::cos(t * 0.4f + 4) * 0.01f;
            auto y3 = glm::cos(t * 0.3f + 2) * 0.01f;
            auto x4 = glm::cos(t * 0.4f - 7) * 0.01f;
            auto y4 = glm::cos(t * 0.3f + 6) * 0.01f;

            auto vertList = std::vector<v>{};
            vertList.push_back(v{}.set<0>({-0.5f + x1, 0.5f + y1, 0.1f}).set<1>({0.0f, 1.0f}).set<2>({0, 255, 255, 255}));
            vertList.push_back(v{}.set<0>({0.5f + x2, 0.5f + y2, 0.1f}).set<1>({1.0f, 1.0f}).set<2>({255, 0, 255, 255}));
            vertList.push_back(v{}.set<0>({0.5f + x3, -0.5f + y3, 0.1f}).set<1>({1.0f, 0.0f}).set<2>({255, 255, 0, 255}));
            vertList.push_back(v{}.set<0>({-0.5f + x4, -0.5f + y4, 0.1f}).set<1>({0.0f, 0.0f}).set<2>({255, 255, 255, 0}));
            vertBuf->update(vertList);

            cmdQueue->Begin();
            cmdQueue->SetPipline(*pipeline);
            cmdQueue->Draw(*indBuf, *vertBuf);
            cmdQueue->End();
            cmdQueue->Present();
        }
        else
        {
            delete vertBuf;
            delete indBuf;
            delete pipeline;
            delete vertexShader;
            delete pixelShader;
            delete cmdQueue;
        }
    }
}