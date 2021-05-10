#version 420 core
out vec4 FragColor;
//in vec4 vertexColor;

uniform float GreenVal;
uniform float RedVal;
uniform float BluVal;
in vec3 ourColor;

void main()
{
//FragColor = vec4(1.0f,0.5f,0.2f, 1.0f);
//FragColor =vertexColor;
//FragColor = ourColor;

FragColor = vec4(ourColor.x * RedVal,ourColor.y *GreenVal ,ourColor.z *BluVal, 1.0f);
}

