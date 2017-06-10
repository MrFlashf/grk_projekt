#version 120

uniform vec3 objectColor;
uniform vec3 lightDir;
uniform sampler2D objectTex;

varying vec3 interpNormal;
varying vec2 texPosition;

void main(void)
{
	vec3 normal = normalize(interpNormal);
	float diffuse = max(dot(normal, -lightDir), 0.0);
	vec4 textureColor = texture2D(objectTex, vec2(texPosition.x, 1 - texPosition.y));
	gl_FragColor = vec4(textureColor.rgb  * diffuse, 1.0);
	
}
