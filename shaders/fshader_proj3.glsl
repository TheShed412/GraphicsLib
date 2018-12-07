#version 120

varying vec4 color;

varying vec4 N;
varying vec4 L;
varying vec4 E;
varying float dist;

uniform mat4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 model_view;
uniform float Shininess;
uniform float attenuation_constant, attenuation_linear, attenuation_quadratic;

vec4 ambient, diffuse, specular;

void main()
{
	gl_FragColor = color;
}
