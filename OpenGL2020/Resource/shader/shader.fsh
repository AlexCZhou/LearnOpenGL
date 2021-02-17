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

struct Spotlight{
    vec3  position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

uniform MaterialWithMap material;
uniform Light light;
uniform DirectionalLight dirLight;
uniform Spotlight spotlight;

uniform vec3 objectColor;
uniform vec3 viewPos;


void main(){
    //平行光
    //vec3 lightDir = normalize(-dirLight.direction);

    //点光源计算衰减
    vec3 lightDir = normalize(spotlight.position - FragPos);
    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic *(distance * distance));

    //聚光
    
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon   = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0); 
    
    //环境光照（即使完全黑暗也会有的
    vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * spotlight.ambient * attenuation;

    //漫反射光照
	vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * diff * spotlight.diffuse * attenuation * intensity;

	//镜面反射
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * spotlight.specular * attenuation * intensity;
        
    //结果
    vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
} 