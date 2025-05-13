#version 450 core

// Diffuse texture - directional light

// Texture sampler (for diffuse surface colour)
layout(binding = 0) uniform sampler2D basetexture;

// Directional light model
uniform vec3 DIR1Dir;
uniform vec3 DIR1Col;
uniform vec3 DIR1Amb;

uniform vec3 DIR2Dir;
uniform vec3 DIR2Col;
uniform vec3 DIR2Amb;

uniform vec3 DIR3Dir;
uniform vec3 DIR3Col;
uniform vec3 DIR3Amb;


float alpha;

in SimplePacket {
	
	vec3 surfaceWorldPos;
	vec3 surfaceNormal;
	vec2 texCoord;

} inputFragment;


layout (location=0) out vec4 fragColour;

void main(void) {

	// calculate lambertian (l)
	vec3 N = normalize(inputFragment.surfaceNormal);
	float l1 = dot(N, DIR1Dir);
	float l2 = dot(N, DIR2Dir);
	float l3 = dot(N, DIR3Dir);

	
	// Calculate diffuse brightness / colour for fragment
	vec4 surfaceColour = texture2D(basetexture, inputFragment.texCoord);
	vec3 diffuseColour1 = surfaceColour.rgb * DIR1Col * l1;
	vec3 diffuseColour2 = surfaceColour.rgb * DIR2Col * l2;
	vec3 diffuseColour3 = surfaceColour.rgb * DIR3Col * l3;

	 alpha = surfaceColour.a;

	fragColour += vec4(DIR1Amb, alpha)+vec4(diffuseColour1, alpha);
	fragColour += vec4(DIR2Amb, alpha)+vec4(diffuseColour2, alpha);
	fragColour += vec4(DIR3Amb, alpha)+vec4(diffuseColour3, alpha);
	//fragColour = vec4(vec3(l, l, l), 1.0);
}
