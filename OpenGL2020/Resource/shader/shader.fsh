#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct MaterialWithMap {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 

struct Light{
    vec3 position;

    //三种类型的强度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirectionalLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform MaterialWithMap material;
uniform Light light;
uniform DirectionalLight dirLight;

uniform vec3 objectColor;
uniform vec3 viewPos;


void main(){
    //平行光
    //vec3 lightDir = normalize(-dirLight.direction);

    //点光源
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic *(distance * distance));


    //环境光照（即使完全黑暗也会有的
    vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * light.ambient * attenuation;

	//漫反射光照
	vec3 norm = normalize(Normal);
    
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * diff * light.diffuse * attenuation;

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * light.specular * attenuation;

	//结果
    vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
} 