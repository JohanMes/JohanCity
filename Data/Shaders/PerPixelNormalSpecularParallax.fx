PS_OUTPUT PerPixelNormalSpecularParallaxPS(VS_OUTPUT_PERPIXELNORMALSPECULAR In) {
	PS_OUTPUT outPS;
	
	// Ze kunnen > 1 worden tijdens interpoleren...
	In.toeye = normalize(In.toeye);
	
	// We hebben deze en inverse nodig, dus van tevoren berekenen
	float3x3 TBN = GetTBN(In.tangent,In.binormal,In.normal);

	// Verschuif coord gebaseerd op hoogte
	In.coord = GetParallaxCoord(In.coord,In.toeye,TBN);
	
	// Converteer de normal terug naar world i.p.v. alles naar tangent
	float3 normalworld = GetWorldNormal(In.coord,transpose(TBN));
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Loop de zonnen langs
	float3 diffuse = 0;
	float3 specular = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		float shadow = GetDirlightShadow(i,In.projcoord);
		
		diffuse += shadow * GetDiffuseValuePhong(color,normalworld,lightdir);
		specular += shadow * GetSpecularValuePhong(color,normalworld,lightdir,In.toeye,In.coord);
	}

	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = pointlights[i].color;
		float shadow = GetPointlightShadow(i,In.projcoord);
		
		diffuse += GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir) * shadow;
		specular += GetSpecularValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir,In.toeye,In.coord) * shadow;
	}

	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		float shadow = GetSpotlightShadow(i,In.projcoord);
		
		diffuse += spotvalue * GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir) * shadow;
		specular += spotvalue * GetSpecularValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir,In.toeye,In.coord) * shadow;
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

technique PerPixelNormalSpecularParallax {
	pass P0 {
		VertexShader = compile vs_3_0 PerPixelNormalSpecularVS();
		PixelShader  = compile ps_3_0 PerPixelNormalSpecularParallaxPS();
	}
}
