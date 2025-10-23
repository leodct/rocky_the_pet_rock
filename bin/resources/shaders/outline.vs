#version 330 core

layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec2 aTexCoord;
out vec2 fragTexCoord;

uniform mat4 mvp;

void main() {
    fragTexCoord = aTexCoord;
    gl_Position = mvp * vec4(aPosition, 0.0, 1.0);
}