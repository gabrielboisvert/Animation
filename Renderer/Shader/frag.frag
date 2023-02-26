#version 450

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 norm;
layout(location = 2) in vec2 UV;
layout(location = 3) in vec3 view;
layout(location = 4) in vec4 infragColor;
layout(location = 5) in mat3 TBN;

layout(location = 0) out vec4 fragColor;

layout(set = 2, binding = 0) uniform sampler2D texSampler;


void main() 
{
    
	fragColor = texture(texSampler, UV);
	
}