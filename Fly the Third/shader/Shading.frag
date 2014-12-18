uniform sampler2D Texture;
uniform sampler2D/*Shadow*/ ShadowMap;

uniform int Shading;

varying vec4 shadowCoordinates, vertex;
varying vec3 normal, direction;

void main(){    

	// PER PIXEL LIGHTING
	vec3 L = normalize(direction);
	vec3 E = normalize(-vertex); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,normal));  
 
	vec4 ambient = gl_LightSource[0].ambient;    
 
	vec4 diffuse = gl_LightSource[0].diffuse * max(dot(normal,L), 0.0);
	diffuse = clamp(diffuse, 0.0, 1.0);     
   
	vec4 specular = gl_LightSource[0].specular 
				* pow(max(dot(R,E),0.0), 0.3 * gl_FrontMaterial.shininess);
	specular = clamp(specular, 0.0, 1.0); 
	// write Total Color:  
	vec4 color = ambient + diffuse + specular;  




	// COLORING
	gl_FragColor = gl_Color;// * color;




	// TEXTURING
    if(Shading >= 1){
		gl_FragColor *= texture2D(Texture, gl_TexCoord[0].st);
	}




	// SHADOW MAP
	float visibility = 1.0;
	if(Shading >= 2){
		float bias = 0.0001;
		if(shadowCoordinates.w > 0.0){

			vec3 ShadowMapTexCoordProj = shadowCoordinates.xyz / shadowCoordinates.w;
        
			if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 &&
			   ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 &&
			   ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0)
			{
				if(texture2D(ShadowMap, ShadowMapTexCoordProj.xy).r <= ShadowMapTexCoordProj.z - bias){
					visibility = 0.5;
				}
			}
		}
	}




	// FINAL
	gl_FragColor.rgb *= visibility;
}