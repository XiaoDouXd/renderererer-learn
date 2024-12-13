#include <cstddef>
#include <cstdint>
namespace XD::Render
{
    class RenderProcess
    {
    public:
        RenderProcess();
        ~RenderProcess();

    public:
        void Enqueue();
        void SetTarget(const class RenderTarget& target);

    public:
        void Do();

    private:

        // ------------- waiting
        bool _locked;
        uint64_t _version;
    };
}