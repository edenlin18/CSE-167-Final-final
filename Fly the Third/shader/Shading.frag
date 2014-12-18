uniform sampler2D Texture;
uniform sampler2D/*Shadow*/ ShadowMap;

uniform int Shading;

varying vec4 ShadowMapTexCoord;
varying vec3 Normal, LightDirection;

varying vec3 lightDir, normal;

varying vec4 Position;

varying vec4 diffuse, ambient;
varying vec3 halfVector;

void main(){

	// Shadow visibility
	float visibility = 1.0;

	// Texture color
	vec4 texture = vec4( 1, 1, 1, 1 );


	// PER PIXEL LIGHTING
	vec3 L = LightDirection; 
	vec3 E = normalize(-Position); // we are in Eye Coordinates, so EyePos is (0,0,0)  
	vec3 R = normalize(-reflect(L,Normal));  
 
	vec4 ambient = gl_LightSource[0].ambient;    
 
	vec4 diffuse = gl_LightSource[0].diffuse * max(dot(Normal,L), 0.0);
	diffuse = clamp(diffuse, 0.0, 1.0);     
   
	vec4 specular = gl_LightSource[0].specular 
				* pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
	specular = clamp(specular, 0.0, 1.0); 
	// write Total Color:  
	vec4 color = gl_FrontLightModelProduct.sceneColor + ambient + diffuse + specular;  

	vec3 ct,cf;
    vec4 texel;
    float intensity,at,af;
    intensity = max(dot(LightDirection,normalize(normal)),0.0);
 
    cf = (gl_LightSource[0].diffuse).rgb +
                  gl_LightSource[0].ambient.rgb;
    af = gl_LightSource[0].diffuse.a;
    texel = texture2D(Texture,gl_TexCoord[0].st);
 
    ct = texel.rgb;
    at = texel.a;
    //gl_FragColor = vec4(ct * cf, at * af);



	float LightDistance2 = dot(LightDirection, LightDirection);
	float LightDistance = sqrt(LightDistance2);
	//float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);

	float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);

	float Attenuation = gl_LightSource[0].constantAttenuation;
	Attenuation += gl_LightSource[0].linearAttenuation * LightDistance;
	Attenuation += gl_LightSource[0].quadraticAttenuation * LightDistance2;
	
	// NdotLD *= shadow2DProj(ShadowMap, ShadowMapTexCoord).r;
	




	// Shading constants
	// 0 - Light only
	// 1 - Textures + Light
	// 2 - Shadows + Textures + Light

	// COLORATION INITIALIZATION
	gl_FragColor = gl_Color;
	




	// PER PIXEL TEXTURING
	if(Shading >= 1){
		//texture = texture2D(Texture, gl_TexCoord[0].st);
	}





	// SHADOW MAP
	if(Shading >= 2){
	float bias = 0;
		if(ShadowMapTexCoord.w > 0.0){
			vec3 ShadowMapTexCoordProj = ShadowMapTexCoord.xyz / ShadowMapTexCoord.w;
		
			if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 &&
			   ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 &&
			   ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0)
			{
				if(texture2D(ShadowMap, ShadowMapTexCoordProj.xy).r <= ShadowMapTexCoordProj.z - bias)
				{
					visibility = 0.5;
				}
			}
		}
	}

	// FINAL COLOR
	
	gl_FragColor *= visibility;
	

	gl_FragColor *= (
		gl_LightSource[0].ambient
		+ gl_LightSource[0].diffuse * visibility
		);

	gl_FragColor = gl_Color * (gl_LightSource[0].ambient + gl_LightSource[0].diffuse * visibility) * texture;

}



