#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;
uniform vec4 color;
uniform int useTexture;

void main()
{

    if (useTexture == 1)
        FragColor = texture(texture1, TexCoord);
    else
        FragColor = color;
}
