#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
varying vec4 color;

uniform mat4 ctm;
uniform mat4 scale_ctm;
uniform mat4 projection;
uniform mat4 model_view;

void main()
{
	color = vColor;
	gl_Position = projection * model_view * scale_ctm * ctm * vPosition / vPosition.w;
}
