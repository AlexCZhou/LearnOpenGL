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
    //ƽ�й�
    //vec3 lightDir = normalize(-dirLight.direction);

    //���Դ����˥��
    vec3 lightDir = normalize(spotlight.position - FragPos);
    float distance = length(spotlight.position - FragPos);
    float attenuation = 1.0 / (spotlight.constant + spotlight.linear * distance + spotlight.quadratic *(distance * distance));

    //�۹�
    
    float theta = dot(lightDir, normalize(-spotlight.direction));
    float epsilon   = spotlight.cutOff - spotlight.outerCutOff;
    float intensity = clamp((theta - spotlight.outerCutOff) / epsilon, 0.0, 1.0); 
    
    //�������գ���ʹ��ȫ�ڰ�Ҳ���е�
    vec3 ambient = vec3(texture(material.diffuse, TexCoord)) * spotlight.ambient * attenuation;

    //���������
	vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoord)) * diff * spotlight.diffuse * attenuation * intensity;

	//���淴��
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = vec3(texture(material.specular, TexCoord)) * spec * spotlight.specular * attenuation * intensity;
        
    //���
    vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0);
} 