uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec4 lightColor2;
uniform vec3 lightPos2;

uniform sampler2D tex0;
uniform sampler2D tex1;

in vec3 Normal;
in vec3 crntPos;
in vec2 texCoord;
in vec3 camPos;

out vec4 FragColor;

vec4 pointLight()
{	
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 0.05;
	float b = 0.01;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.20f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor;
}

vec4 pointLight2()
{	
	vec3 lightVec = lightPos2 - crntPos;
	float dist = length(lightVec);
	float a = 0.05;
	float b = 0.01;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	float ambient = 0.20f;
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 16);
	float specular = specAmount * specularLight;

	return (texture(tex0, texCoord) * (diffuse * inten + ambient) + texture(tex1, texCoord).r * specular * inten) * lightColor2;
}

void main()
{
	FragColor = pointLight() + pointLight2();
}