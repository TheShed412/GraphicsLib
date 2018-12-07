#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform vec4 light_point;
uniform mat4 ctm;
uniform mat4 scale_ctm;
uniform mat4 projection;
uniform mat4 model_view;

uniform int isShadow;

void main()
{
	vec4 shadow_color = vec4(0.0, 0.0, 0.0, 1.0);

	if(isShadow == 1) {
		color = shadow_color;
		float x = light_point.x - (light_point.y * ((light_point.x-vPosition.x)/(light_point.y-vPosition.y)));
		float y = 0.001;
		float z = light_point.z - (light_point.y * ((light_point.z-vPosition.z)/(light_point.y-vPosition.y)));

		gl_Position = (projection * model_view * ctm * vec4(x, y, z, 1.0) / vPosition.w);
	} else {
		color = vColor;
		gl_Position = projection * model_view * scale_ctm * ctm * vPosition / vPosition.w;
	}
}
