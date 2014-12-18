uniform mat4 ShadowMatrix;

varying vec4 ShadowMapTexCoord;
varying vec3 Normal, LightDirection;

varying vec4 shadowCoordinates;

varying vec3 lightDir, normal;

varying vec4 Position;

varying vec4 diffuse, ambient;
varying vec3 halfVector;

varying mat4 MV;

void main(){

	halfVector = gl_LightSource[0].halfVector.xyz;
    diffuse = gl_LightSource[0].diffuse;
    ambient = gl_LightSource[0].ambient;

	MV = gl_ModelViewMatrix;

	Position = gl_ModelViewMatrix * gl_Vertex;
	ShadowMapTexCoord = ShadowMatrix * Position;
	Normal = gl_NormalMatrix * gl_Normal;
	LightDirection = gl_LightSource[0].position.xyz - Position.xyz;
	
	normal = normalize(gl_NormalMatrix * gl_Normal);
 
    lightDir = normalize(vec3(gl_LightSource[0].position));
	
	gl_FrontColor = gl_Color;


	// Texture coordinates
	gl_TexCoord[0] = gl_MultiTexCoord0;

	// Modelview * Project * Vertex
	gl_Position = gl_ProjectionMatrix * Position;

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
