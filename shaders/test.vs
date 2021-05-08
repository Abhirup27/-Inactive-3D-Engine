#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
//out vec4 vertexColor;
out vec3 ourColor;
void main()
{
gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);
//vertexColor = vec4(0, 0.5, 0.2, 1.0);
ourColor = aColor;
}

