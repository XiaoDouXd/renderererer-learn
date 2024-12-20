#pragma once

#ifndef XD__TEMPLATE_HXX
#include "render/public/gRes/buffer/vertexBuffer.hpp"   // 这个只是为了保证语法提示的准确性...
#endif

#ifndef XD__TEMPLATE_EXTERN
#define XD__TEMPLATE_EXTERN extern
#endif

namespace XD::Render
{
    // basic layout
    XD__TEMPLATE_EXTERN template class BufferLayout<
        Format::SF32_3D,      // vertex pos       ... 0
        Format::UN8_4D        // vertex color     ... 1
    >;
    using BasicBufferLayout = BufferLayout<
        Format::SF32_3D,
        Format::UN8_4D
    >;

    // uv layout
    XD__TEMPLATE_EXTERN template class BufferLayout<
        Format::SF32_3D,      // vertex pos       ... 0
        Format::SF32_2D,      // vertex uv        ... 1
        Format::UN8_4D        // vertex color     ... 2
    >;
    using UvBufferLayout = BufferLayout<
        Format::SF32_3D,
        Format::SF32_2D,
        Format::UN8_4D
    >;

    // full layout
    XD__TEMPLATE_EXTERN template class BufferLayout<
        Format::SF32_3D,      // vertex pos       ... 0
        Format::SF32_3D,      // vertex normal    ... 1
        Format::SF32_3D,      // vertex tangent   ... 2
        Format::SF32_3D,      // vertex bitangent ... 3
        Format::SF32_2D,      // vertex uv        ... 4
        Format::UN8_4D        // vertex color     ... 5
    >;
    using FullBufferLayout = BufferLayout<
        Format::SF32_3D,
        Format::SF32_3D,
        Format::SF32_3D,
        Format::SF32_3D,
        Format::SF32_2D,
        Format::UN8_4D
    >;
}