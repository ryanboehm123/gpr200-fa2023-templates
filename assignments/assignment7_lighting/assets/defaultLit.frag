#version 450
out vec4 FragColor;

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

uniform sampler2D _Texture;

float vDiffuse;

float vSpecular;
float vShininess;

float vAmbient;

void main(){
	FragColor = texture(_Texture,fs_in.UV);
	vec3 normal = normalize(fs_in.WorldNormal);
	vec3 lightDir[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		lightDir[i] = normalize(_Light[i].position - fs_in.WorldPosition);
	}

	//TODO: Lighting calculations using corrected normal
	float diffuse[MAX_LIGHTS];
	vec3 diff[MAX_LIGHTS];
	for(int i = 0; i < MAX_LIGHTS; i++) {
		diffuse[i] = max(dot(normal, lightDir[i]), 0.0);
		diff[i] = diffuse[i] * _Light[i].color;
	}

	vec3 ref = reflect(lightDir, normal);
	float specular = max(dot(ref, v), 0)^vShininess;
}