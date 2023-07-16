// attribute，只读的顶点数据，只用于顶点着色器中，只能被声明为全局变量，一般用来表示顶点数据（顶点坐标、顶点法线、顶点颜色等）
// varying，用于 vertex 和 fragment 之间做数据传递，在 vs 中可读可写，fs 只读，
// uniform，类似 const，不能被 shader 修改

attribute   vec3 vertexPosition;
attribute   vec2 vertexUV;
varying     vec2 textureCoords;
uniform     mat4 mvpMat;

void main()
{
    gl_Position     = mvpMat * vec4(vertexPosition, 1.0);
    textureCoords   = vertexUV;
}
