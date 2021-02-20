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

    //�������͵�ǿ��
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
 * ���ڼ��㶨���
 * @param light: ���������
 * @param normal: Ƭ�η���
 * @param viewDir: �ӽǳ���
 * @return �������
 */
vec3 calcDirLight(DirectionalLight light, vec3 normal, vec3 viewDir);

/**
 * ���ڼ�����Դ
 * @param light: ���������
 * @param fragPos: Ƭ����������
 * @return ���Դ���
 */
vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

/**
 * ���ڼ���۹�
 * @param light: �۹�����
 * @return �۹���
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
    // ��������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord));
    // ������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord));
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, TexCoord)) * spec;
    // �ϲ����
    return (ambient + diffuse + specular);
    
}

vec3 calcPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(light.position - FragPos);
    // ����˥��
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic *(distance * distance));
    // ��������
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // ������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoord)) * attenuation;
    // �ϲ����
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight spotLight, vec3 normal, vec3 fragPos, vec3 viewDir){
    vec3 lightDir = normalize(spotLight.position - FragPos);
    // ����˥��
    float distance = length(spotLight.position - FragPos);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic *(distance * distance));
    // �۹�
    float theta = dot(lightDir, normalize(-spotLight.direction));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0); 
    // ��������
    vec3 ambient = spotLight.ambient * vec3(texture(material.diffuse, TexCoord)) * attenuation;
    // ������
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = spotLight.diffuse * diff * vec3(texture(material.diffuse, TexCoord)) * attenuation * intensity;
    // ���淴��
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = spotLight.specular * spec * vec3(texture(material.specular, TexCoord)) * attenuation * intensity;
    // �ϲ����
    return (ambient + diffuse + specular);
}