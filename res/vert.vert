#version 450 core

// 输入的顶点属性
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec4 col;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec4 vertexColor; // 传递的颜色

void main()
{
    // 计算最终的顶点位置
    gl_Position = vec4(pos, 1.0);
    
    // 传递纹理坐标和颜色到片段着色器
    fragUV = uv;
    vertexColor = col / 255;
}