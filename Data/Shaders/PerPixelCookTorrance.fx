////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	CookTorrance, berekend per pixel
*/

PS_OUTPUT PerPixelCookTorrancePS(VS_OUTPUT_PERPIXELSPECULAR In) {
	PS_OUTPUT outPS;

	// Ze kunnen > 1 worden tijdens interpoleren...
	In.toeye = normalize(In.toeye);
	In.normalworld = normalize(In.normalworld);
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Loop de zonnen langs
	float3 diffuse = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		
		diffuse += GetDiffuseValueCookTorrance(color,In.normalworld,lightdir,In.toeye) * GetDirlightShadow(i,In.projcoord);
	}
		
	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = pointlights[i].color;
		
		diffuse += GetDiffuseValueCookTorrance(color/distance(lightpos,In.positionworld),In.normalworld,lightdir,In.toeye) * GetPointlightShadow(i,In.projcoord);
	}
	
	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		
		diffuse += spotvalue * GetDiffuseValueCookTorrance(color/distance(lightpos,In.positionworld),In.normalworld,lightdir,In.toeye) * GetSpotlightShadow(i,In.projcoord);
	}
	
	float fog = GetFogValue(camerapos,In.positionworld);

	// Deze hogerop doen voor eerdere clip?
	float4 texColor = tex2D(DiffuseSampler, In.coord);

	// Interpoleer tussen kleur en fogkleur, afhankelijk van foghoeveelheid
	outPS.color.rgb = lerp(texColor.rgb*(diffuse + lightambient),fogcolor,fog);
	
	// Doorzichtigheid niet verbouwen
	outPS.color.a = texColor.a;
	
	return outPS;
}

technique PerPixelCookTorrance {
	pass P0 {
		VertexShader = compile vs_3_0 PerPixelSpecularVS();
		PixelShader  = compile ps_3_0 PerPixelCookTorrancePS();
	}
}
