// Used for shadow lookup
varying vec4 ShadowCoord;
varying vec3 V, N;

void main(){

	// PER PIXEL COLOR
	V = vec3(gl_ModelViewMatrix * gl_Vertex);       
	N = normalize(gl_NormalMatrix * gl_Normal);
     


  
	gl_Position = ftransform();

	gl_FrontColor = gl_Color;
}