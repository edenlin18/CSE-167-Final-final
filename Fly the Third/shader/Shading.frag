uniform sampler2D Texture;
uniform sampler2D/*Shadow*/ ShadowMap;

uniform int Shading;

varying vec4 shadowCoordinates, vertex;
varying vec3 normal, direction;

void main(){    

	



	// COLORING
	gl_FragColor = gl_Color;





	// SHADOW MAP
	float visibility = 1.0;
	if(Shading >= 2){
		if(shadowCoordinates.w > 0.0){

			vec3 ShadowMapTexCoordProj = shadowCoordinates.xyz / shadowCoordinates.w;
        
			if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 &&
			   ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 &&
			   ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0)
			{
				if(texture2D(ShadowMap, ShadowMapTexCoordProj.xy).r <= ShadowMapTexCoordProj.z){
					visibility = 0.5;
				}
			}
		}
	}





    // TEXTURING
    if(Shading >= 1){
		gl_FragColor *= texture2D(Texture, gl_TexCoord[0].st);
	}





	// FINAL
	gl_FragColor.rgb *= visibility;
}



