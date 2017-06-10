#version 120
uniform mat4 modelViewProjectionMatrix;
uniform mat4 modelMatrix;
attribute vec3 vertexPosition;
attribute vec2 vertexTexCoord;
attribute vec3 vertexNormal;
varying vec3 interpNormal;
varying vec2 texPosition;

void main(void)
{
	gl_Position = modelViewProjectionMatrix * vec4(vertexPosition, 1.0);
	interpNormal = (modelMatrix * vec4(vertexNormal, 0.0)).xyz;
	texPosition = vertexTexCoord;
}
