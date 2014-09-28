////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Blinn/Phong, berekend per pixel, met specularmap
*/

struct VS_OUTPUT_PERPIXELSPECULAR {
	float4 position      : POSITION0;
	float3 positionworld : TEXCOORD0;
	float3 normalworld   : TEXCOORD1;
	float2 coord         : TEXCOORD2;
	float4 projcoord     : TEXCOORD3;
	float3 toeye         : TEXCOORD4;
};

VS_OUTPUT_PERPIXELSPECULAR PerPixelSpecularVS(float3 positionin : POSITION0,float2 coordin : TEXCOORD0,float3 normalin : NORMAL0) {
	VS_OUTPUT_PERPIXELSPECULAR outVS;

	// Bereken pixelcoordinaat (xy, z = diepte,w negeren)
	outVS.position = mul(float4(positionin,1.0f),WorldViewProj);
	
	// Bereken coordinaat relatief aan nulpunt wereld
	outVS.positionworld = mul(float4(positionin,1.0f),World).xyz;
	
	// Bereken normaal van vlak relatief aan nulpunt wereld
	outVS.normalworld = mul(normalin,(float3x3)World);
	
	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = coordin;
	
	// Deze berekenen per vertex, scheelt tijd
	outVS.toeye = camerapos - outVS.positionworld;

	// Mul met lightpos, dan coords verschuiven, zodat we coords relatief aan licht hebben
	outVS.projcoord = mul(mul(float4(positionin,1.0f),LightWorldViewProj),ShadowOffset);

	// En doorgeven aan de pixelshader, die per pixel de kleur berekent
	return outVS;
}

PS_OUTPUT PerPixelSpecularPS(VS_OUTPUT_PERPIXELSPECULAR In) {
	PS_OUTPUT outPS;

	// Ze kunnen > 1 worden tijdens interpoleren...
	In.toeye = normalize(In.toeye);
	In.normalworld = normalize(In.normalworld);
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Loop de zonnen langs
	float3 diffuse = 0;
	float3 specular = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		float shadow = GetDirlightShadow(i,In.projcoord);
		
		diffuse += shadow * GetDiffuseValuePhong(color,In.normalworld,lightdir);
		specular += shadow * GetSpecularValuePhong(color,In.normalworld,lightdir,In.toeye,In.coord);
	}
	
	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = pointlights[i].color;
		float shadow = GetPointlightShadow(i,In.projcoord);
		
		diffuse += shadow * GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir);
		specular += shadow * GetSpecularValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir,In.toeye,In.coord);
	}
	
	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		float shadow = GetSpotlightShadow(i,In.projcoord);
		
		diffuse += shadow * spotvalue * GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir);
		specular += shadow * spotvalue * GetSpecularValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir,In.toeye,In.coord);
	}
	
	float fog = GetFogValue(camerapos,In.positionworld);

	// Deze hogerop doen voor eerdere clip?
	float4 texColor = tex2D(DiffuseSampler, In.coord);
	
	// Interpoleer tussen kleur en fogkleur, afhankelijk van foghoeveelheid
	outPS.color.rgb = lerp(texColor.rgb*(specular + diffuse + lightambient),fogcolor,fog);
	
	// Doorzichtigheid niet verbouwen
	outPS.color.a = texColor.a;
	
	return outPS;
}

technique PerPixelSpecular {
	pass P0 {
		VertexShader = compile vs_3_0 PerPixelSpecularVS();
		PixelShader  = compile ps_3_0 PerPixelSpecularPS();
	}
}
