#version 330 core
layout (location = 0) in vec3 aPos; // 顶点位置
layout (location = 1) in vec3 aColor; 
layout (location = 2) in vec2 aTexCoord; // 纹理坐标
layout (location = 3) in vec3 aNormal; // 法向量

out vec3 ourColor; 
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model; //变换矩阵
uniform mat4 view; //视角矩阵
uniform mat4 projection; //投影矩阵

void main(){
    gl_Position = projection * view * model * vec4(aPos, 1.0);
	ourColor = aColor;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(model))) * aNormal;
    FragPos = vec3(model * vec4(aPos, 1.0)); //片段位置的世界空间坐标
}