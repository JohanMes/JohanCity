////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	PerPixel, maar dan met meerdere diffuse-textures
*/

PS_OUTPUT PerPixelMultitexConstPS(VS_OUTPUT_PERPIXEL In) {
	PS_OUTPUT outPS;

	// Ze kunnen > 1 worden tijdens interpoleren...
	In.normalworld = normalize(In.normalworld);
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Loop de zonnen langs
	float3 diffuse = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		
		diffuse += GetDiffuseValuePhong(color,In.normalworld,lightdir) * GetDirlightShadow(i,In.projcoord);
	}
	
	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = pointlights[i].color;
		
		diffuse += GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir) * GetPointlightShadow(i,In.projcoord);
	}
	
	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		
		diffuse += spotvalue * GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),In.normalworld,lightdir) * GetSpotlightShadow(i,In.projcoord);
	}
		
	float fog = GetFogValue(camerapos,In.positionworld);

	// We halen mixing weights uit een variabele...
	float4 tex1 = materialmixer.x*tex2D(DiffuseSampler, In.coord*materialtiling);
	float4 tex2 = materialmixer.y*tex2D(SpecularSampler,In.coord*materialtiling);
	float4 tex3 = materialmixer.z*tex2D(NormalSampler,  In.coord*materialtiling);

	float4 texColor = tex1+tex2+tex3;

	// Interpoleer tussen kleur en fogkleur, afhankelijk van foghoeveelheid
	outPS.color.rgb = lerp(texColor.rgb*(diffuse + lightambient),fogcolor,fog);
	
	// Doorzichtigheid niet verbouwen
	outPS.color.a = texColor.a; // 1.0f?
	
	return outPS;
}

technique PerPixelMultitexConst {
	pass P0 {
		VertexShader = compile vs_3_0 PerPixelVS();
		PixelShader  = compile ps_3_0 PerPixelMultitexConstPS();
	}
}
