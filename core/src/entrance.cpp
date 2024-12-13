// #define XD_VK_IMPL

#include <iostream>
#include <ostream>
#include <cstdint>
#include <vector>

#include "app/public/appMgr.h"
#include "app/public/event/staticEventMgr.hpp"
#include "app/public/util/timeMgr.h"
#include "render/private/gRes/buffer/vertexBuffer.hxx"
#include "render/public/renderMgr.h"
#include "util/public/exce.h"

namespace XD
{
    const uint8_t* xdWndInitConf_iconPngData = nullptr;
    size_t xdWndInitConf_iconPngSize = 0;

    const char* xdWndInitConf_wndName = "xdWnd";
    int xdWndInitConf_defWndWidth = 640;
    int xdWndInitConf_defWndHeight = 360;
    int xdWndInitConf_loadingWidth = 512;
    int xdWndInitConf_loadingHeight = 288;

    [[maybe_unused]] std::u8string xdAssetInitConf_rootResMapPath = u8"./appRes/res-map-default.json";
    [[maybe_unused]] const std::locale defaultLocale = std::locale();
}

namespace XD
{
    void Test(int8_t);

    static bool _inited = false;
    bool inited() { return _inited; }
    void init()
    {
        XD::AppMgr::init();
        XD::Render::Mgr::init();

        // 初始化事件
        XD::Event::StaticEventMgr::init();

        // 初始化工具
        XD::Util::TimeMgr::init();
        _inited = true;

        Test(0);
    }

    void update(bool& quit)
    {
        if (!_inited) throw Exce(__LINE__, __FILE__, "XD::entrance: 未初始化实例");

        // 更新各个管理器
        XD::Util::TimeMgr::update();
        XD::Event::StaticEventMgr::update();

        // 切换交换链
        XD::Render::Mgr::begUpdate();
        XD::AppMgr::update(quit, true);
        Test(1);
        XD::Render::Mgr::endUpdate();
    }

    void destroy()
    {
        if (!_inited) throw Exce(__LINE__, __FILE__, "XD::entrance: 未初始化实例");

        Test(-1);
        XD::Render::Mgr::destroy();
        XD::AppMgr::destroy();

        XD::Event::StaticEventMgr::destroy();
        XD::Util::TimeMgr::destroy();

        _inited = false;
    }
} // namespace XD

// 临时的 main 函数
int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
    try
    {
        XD::init();
        bool done = false;
        while (!done) XD::update(done);
        XD::destroy();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        system("pause");
    }
    return 0;
}