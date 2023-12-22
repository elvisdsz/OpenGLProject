#shader vertex
#version 330 core

layout(location = 0) in
vec4 position;

layout(location = 1) in
vec4 colorRGBA;

layout(location = 2) in
vec2 texCoord;

layout(location = 3) in
float texID;

out
vec2 v_TexCoord;

out
vec4 v_ColorRGBA;

out
float v_TexID;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * position;
    v_TexCoord = texCoord;
    v_ColorRGBA = colorRGBA;
    v_TexID = texID;
};

#shader fragment
#version 330 core

layout(location = 0) out
vec4 color;

in
vec2 v_TexCoord;

in
vec4 v_ColorRGBA;

in
float v_TexID;

//uniform sampler2D u_Texture;
uniform sampler2D u_Textures[2];

void main()
{
    // vec4 texColor = texture(u_Texture, v_TexCoord);
    // color = texColor;
    // color = v_ColorRGBA;
    //color = vec4(v_TexID, v_TexID, 0.0f, 1.0f);
    
    int index = int(v_TexID);
    color = texture(u_Textures[index], v_TexCoord);
};