uniform sampler2D ShadowMap;

varying vec4 ShadowCoord;

varying vec3 V, N;   

void main(){	

	// PER PIXEL LIGHTING
	vec3 L = normalize(gl_LightSource[0].position.xyz - V);   
	vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,N));  
 
	vec4 ambient = gl_FrontLightProduct[0].ambient;    
 
	vec4 diffuse = gl_FrontLightProduct[0].diffuse * max(dot(N,L), 0.0);
	diffuse = clamp(diffuse, 0.0, 1.0);     
   
	vec4 specular = gl_FrontLightProduct[0].specular 
				* pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
	specular = clamp(specular, 0.0, 1.0); 
	// write Total Color:  
	vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;  
	//vec4 color = gl_Color + ambient + diffuse + specular;






	// SHADOW MAP
	// Normalized
	vec4 shadowN = ShadowCoord / ShadowCoord.w;

	// Original bias = 0.0005
	float bias = 0.0005;

	shadowN.z += bias;
	
	float distance = texture2D(ShadowMap, shadowN.st).z;

	float visibility = 1.0;
	
	if( ShadowCoord.w > 0.0 ){
		if( distance < shadowN.z ){
			visibility = 0.5;
		}
	}

	//gl_FragColor = visibility * gl_Color
	gl_FragColor = visibility * color * gl_Color;
}

