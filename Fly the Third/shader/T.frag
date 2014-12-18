#version 120

uniform sampler2D Texture;
uniform sampler2DShadow ShadowMap;
uniform int Shading;

varying vec4 ShadowMapTexCoord;
varying vec3 Normal, LightDirection;

// This define the value to move one pixel left or right
uniform float OffsetX;

// This define the value to move one pixel up or down
uniform float OffsetY;

float lookup( vec2 offSet)
{
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return shadow2DProj(ShadowMap, ShadowMapTexCoord + vec4(offSet.x * OffsetX * ShadowMapTexCoord.w, offSet.y * OffsetY * ShadowMapTexCoord.w, 0.05, 0.0) ).w;
}

void main()
{
	float LightDistance2 = dot(LightDirection, LightDirection);
	float LightDistance = sqrt(LightDistance2);
	//float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);

	//float NdotLD = max(dot(normalize(Normal), LightDirection / LightDistance), 0.0);

	float Attenuation = gl_LightSource[0].constantAttenuation;
	Attenuation += gl_LightSource[0].linearAttenuation * LightDistance;
	Attenuation += gl_LightSource[0].quadraticAttenuation * LightDistance2;
	
	float NdotLD = shadow2DProj(ShadowMap, ShadowMapTexCoord).r;
	
	// SHADOW MAP
	float visbility = 1.0;
	
	// Avoid counter shadow
	if (ShadowMapTexCoord.w > 1.0){
		vec3 ShadowMapTexCoordProj = ShadowMapTexCoord.xyz / ShadowMapTexCoord.w;

		if(ShadowMapTexCoordProj.x >= 0.0 && ShadowMapTexCoordProj.x < 1.0 &&
			ShadowMapTexCoordProj.y >= 0.0 && ShadowMapTexCoordProj.y < 1.0 &&
			ShadowMapTexCoordProj.z >= 0.0 && ShadowMapTexCoordProj.z < 1.0)
		{
			// 8x8 kernel PCF
					
			float x,y;
			for (y = -3.5 ; y <=3.5 ; y+=1.0)
				for (x = -3.5 ; x <=3.5 ; x+=1.0)
					visbility += lookup(vec2(x,y));
					
			visbility /= 64.0 ;
		}
	}
	gl_FragColor = (visbility + 0.2) * gl_Color;
}



