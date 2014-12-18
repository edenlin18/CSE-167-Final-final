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
    float LightDistance2 = dot(LightDirection, LightDirection);
    float LightDistance = sqrt(LightDistance2);
    float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);
    float Attenuation = gl_LightSource[0].constantAttenuation;
    Attenuation += gl_LightSource[0].linearAttenuation * LightDistance;
    Attenuation += gl_LightSource[0].quadraticAttenuation * LightDistance2;
    
    // NdotLD *= shadow2DProj(ShadowMap, ShadowMapTexCoord).r;
    
	float visibility = 1.0;
    if(ShadowMapTexCoord.w > 0.0){

        vec3 ShadowMapTexCoordProj = ShadowMapTexCoord.xyz / ShadowMapTexCoord.w;
        
        if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 &&
           ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 &&
           ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0)
        {
            if(texture2D(ShadowMap, ShadowMapTexCoordProj.xy).r <= ShadowMapTexCoordProj.z){
                NdotLD = 0.0;
				visibility = 0.5;
            }
        }
    }
    
    gl_FragColor = gl_Color;
    if(Shading >= 1){
		gl_FragColor *= texture2D(Texture, gl_TexCoord[0].st);
	}

    //gl_FragColor.rgb *= (gl_LightSource[0].ambient.rgb + gl_LightSource[0].diffuse.rgb * NdotLD) / Attenuation;

	//gl_FragColor *= visibility;

	//gl_FragColor.rgb *= (gl_LightSource[0].ambient.rgb + gl_LightSource[0].diffuse.rgb * visibility);

	gl_FragColor.rgb *= visibility;
}



