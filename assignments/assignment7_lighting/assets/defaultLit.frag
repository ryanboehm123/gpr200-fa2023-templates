#version 450

in Surface{
	vec2 UV; //Per-fragment interpolated UV
	vec3 WorldPosition; //Per-fragment interpolated world position
	vec3 WorldNormal; //Per-fragment interpolated world normal
}fs_in;

struct Light {
	vec3 position;
	vec3 color;
};
#define MAX_LIGHTS 4
uniform Light _Light[MAX_LIGHTS];

out vec4 FragColor[MAX_LIGHTS];

uniform sampler2D _Texture;

float vDiffuse;

float vSpecular;
float vShininess;
vec3 viewPosition;

float vAmbient;

void main(){
	for(int i = 0; i < MAX_LIGHTS; i++) {
		FragColor[i] = texture(_Texture,fs_in.UV);
	}
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 lightDir[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		lightDir[i] = normalize(_Light[i].position - fs_in.WorldPosition);
	}

	//Diffuse Lighting
	float diff[MAX_LIGHTS];
	vec3 diffuse[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		diff[i] = max(dot(normal, lightDir[i]), 0.0);
		diffuse[i] = diff[i] * _Light[i].color;
	}

	//Specular Lighting
	vec3 ref[MAX_LIGHTS]; 
	for(int i = 0; i < MAX_LIGHTS; i++) {
		ref[i] = reflect(-lightDir[i], normal);
	}
	float spec[MAX_LIGHTS];
	vec3 specular[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		spec[i] = pow(max(dot(ref[i], viewPosition), 0.0), vShininess);
		specular[i] = vSpecular * spec[i] * _Light[i].color;
	}

	//Ambient Lighting
	vec3 ambient[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		ambient[i] = vAmbient * _Light[i].color;
	}

	//Finished Lighting
	vec3 lightColor[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		lightColor[i] = (ambient[i] + diffuse[i] + specular[i]);
	}

	for(int i = 0; i < MAX_LIGHTS; i++) {
		FragColor[i] = vec4(lightColor[i], 1.0);
	}
}