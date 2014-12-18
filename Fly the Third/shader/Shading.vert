uniform mat4 ShadowMatrix;

varying vec4 shadowCoordinates, vertex;
varying vec3 normal, direction;

void main(){

	vertex = gl_ModelViewMatrix * gl_Vertex;
	shadowCoordinates = ShadowMatrix * vertex;
	normal = gl_NormalMatrix * gl_Normal;
	direction = gl_LightSource[0].position.xyz - vertex.xyz;
	
	// Coloring
	gl_FrontColor = gl_Color;

	// Texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Modelview * Project * Vertex
	gl_Position = gl_ProjectionMatrix * vertex;

	/*
	vec4 Position = gl_ModelViewMatrix * gl_Vertex;
	ShadowMapTexCoord = ShadowMatrix * Position;
	Normal = gl_NormalMatrix * gl_Normal;
	LightDirection = gl_LightSource[0].position.xyz - Position.xyz;
	gl_FrontColor = gl_Color;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = gl_ProjectionMatrix * Position;
	*/
}