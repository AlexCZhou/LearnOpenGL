#version 330 core
#define NR_POINT_LIGHTS 4
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
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
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

struct SpotLight{
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
uniform Light light[NR_POINT_LIGHTS];
uniform DirectionalLight dirLight;
uniform SpotLight spotLight;

uniform vec3 viewPos;

/**
 * 用于计算定向光
 * @param light: 定向光属性
 * @param normal: 片段法线
 * @param viewDir: 视角朝向
 * @return 定向光结果
 */
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

/**
 * 用于计算点光源
 * @param light: 定向光属性
 * @param fragPos: 片段世界坐标
 * @return 点光源结果
 */
vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

/**
 * 用于计算聚光
 * @param light: 聚光属性
 * @return 聚光结果
 */
vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir);

void main(){
    vec3 output;
    vec3 viewDir = normalize(viewPos - FragPos);

    //output += calcDirLight(dirLight, Normal, viewDir);
    for(int i=0;i<NR_POINT_LIGHTS;i++){
        output += calcPointLight(light[i],Normal,FragPos,viewDir);
    }
    //output += calcSpotLight(spotLight,Normal,FragPos,viewDir);
	FragColor = vec4(output, 1.0);
} 

vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir){
    vec3 lightDir = normalize(-light.direction);
    // 环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoord)) * spec;
    // 合并结果
    return (ambient + diffuse + specular);
    
}

vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    // 计算衰减
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic *(distance * distance));
    // 环境光照
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)) * attenuation;
    // 合并结果
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(spotLight.position - FragPos);
    // 计算衰减
    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic *(distance * distance));
    // 聚光
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 
    // 环境光照
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // 漫反射
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, TexCoord)) * attenuation * intensity;
    // 镜面反射
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TexCoord)) * attenuation * intensity;
    // 合并结果
    return (ambient + diffuse + specular);
}