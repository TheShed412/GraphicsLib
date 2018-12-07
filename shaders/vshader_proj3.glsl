#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;

varying vec4 color;

uniform vec4 light_point;
uniform mat4 ctm;
uniform mat4 scale_ctm;
uniform mat4 projection;
uniform mat4 model_view;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform float shininess;
uniform float attenuation_constant, attenuation_linear, attenuation_quadratic;

vec4 ambient, diffuse, specular;

uniform int isShadow;

void main()
{
	vec4 shadow_color = vec4(0.0, 0.0, 0.0, 1.0);

	if(isShadow == 1) {
		color = shadow_color;
		float x = light_point.x - (light_point.y * ((light_point.x-vPosition.x)/(light_point.y-vPosition.y)));
		float y = 0.001;
		float z = light_point.z - (light_point.y * ((light_point.z-vPosition.z)/(light_point.y-vPosition.y)));

		gl_Position = (projection * model_view * ctm * vec4(x, y, z, 1.0));
	} else {
		ambient = AmbientProduct;
		vec4 N = normalize(model_view * ctm * vNormal);
		vec4 L_temp = model_view * (light_point - vPosition);
		vec4 L = normalize(L_temp);
		diffuse = max(dot(L,N), 0.0) * DiffuseProduct;
		vec4 EyePoint = vec4(0.0, 0.0, 0.0, 1.0);
		vec4 V = normalize(EyePoint - (model_view * ctm * vPosition));
		vec4 H = normalize(L+V);
		specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;
		float distance = length(L_temp);
		float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + 
							(attenuation_quadratic * distance * distance));
		color = ambient + (attenuation * (diffuse + specular));
		//color = vColor;
		gl_Position = projection * model_view * scale_ctm * ctm * vPosition;
	}
}
