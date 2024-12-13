#version 450 core

// 从顶点着色器接收的输入
layout(location = 0) in vec2 fragUV;  // 纹理坐标
layout(location = 1) in vec4 vertexColor; // 顶点颜色

// 输出的颜色
layout(location = 0) out vec4 finalColor; // 最终输出的颜色

void main()
{
    // 从纹理中获取颜色（如果使用纹理）
    // vec4 texColor = texture(textureSampler, fragUV);
    
    // 计算最终颜色（可以选择使用纹理颜色或顶点颜色）
    // finalColor = texColor * fragCol; // 结合纹理颜色和顶点颜色
    // 或者直接使用顶点颜色
    finalColor = vertexColor;
}