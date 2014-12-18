uniform sampler2DShadow ShadowMap;

varying vec4 ShadowCoord;

// This define the value to move one pixel left or right
uniform float xPixelOffset ;

// This define the value to move one pixel up or down
uniform float yPixelOffset ;

varying vec3 V, N;   

float lookup( vec2 offSet)
{
	// Values are multiplied by ShadowCoord.w because shadow2DProj does a W division for us.
	return shadow2DProj(ShadowMap, ShadowCoord + vec4(offSet.x * xPixelOffset * ShadowCoord.w, offSet.y * yPixelOffset * ShadowCoord.w, 0.05, 0.0) ).w;
}

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
	float visbility = 1.0;
	
	// Avoid counter shadow
	if (ShadowCoord.w > 1.0)
	{
		// Simple lookup, no PCF
		/*
		visbility = lookup(vec2(0.0,0.0));
		*/

		// USING THIS
		// 8x8 kernel PCF
					
		float x,y;
		for (y = -3.5 ; y <=3.5 ; y+=1.0)
			for (x = -3.5 ; x <=3.5 ; x+=1.0)
				visbility += lookup(vec2(x,y));
					
		visbility /= 64.0 ;
					
		// 8x8 PCF wide kernel (step is 10 instead of 1)
		/*
		float x,y;
		for (y = -30.5 ; y <=30.5 ; y+=10.0)
			for (x = -30.5 ; x <=30.5 ; x+=10.0)
				visbility += lookup(vec2(x,y));
					
		visbility /= 64.0 ;
		*/
	
		// 4x4 kernel PCF
		/*
		float x,y;
		for (y = -1.5 ; y <=1.5 ; y+=1.0)
			for (x = -1.5 ; x <=1.5 ; x+=1.0)
				visbility += lookup(vec2(x,y));
		
		visbility /= 16.0 ;
		*/
		
		// 4x4  PCF wide kernel (step is 10 instead of 1)
		/*
		float x,y;
		for (y = -10.5 ; y <=10.5 ; y+=10.0)
			for (x = -10.5 ; x <=10.5 ; x+=10.0)
				visbility += lookup(vec2(x,y));
					
		visbility /= 16.0 ;
		*/
		
		// 4x4  PCF dithered
		/*
		// use modulo to vary the sample pattern
		vec2 o = mod(floor(gl_FragCoord.xy), 2.0);
				
		visbility += lookup(vec2(-1.5, 1.5) + o);
		visbility += lookup(vec2( 0.5, 1.5) + o);
		visbility += lookup(vec2(-1.5, -0.5) + o);
		visbility += lookup(vec2( 0.5, -0.5) + o);
		visbility *= 0.25 ;
		*/
	}

  	gl_FragColor = (visbility + 0.2) * color * gl_Color;
}

