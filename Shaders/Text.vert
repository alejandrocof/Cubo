#version 330 core

in vec2 coords;
in vec2 texcoords;

out vec2 texcoords2D;

void main(void)
{
    texcoords2D=texcoords;
    gl_Position = vec4(coords,0.0,1.0);
}
