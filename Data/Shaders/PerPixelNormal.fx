////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Blinn/Phong, berekend per pixel, met normalmap
*/

struct VS_INPUT_NORMALMAP {
	float3 position : POSITION0;
	float2 coord    : TEXCOORD0;
	float3 normal   : NORMAL0;
	float3 tangent  : TANGENT0;
	float3 binormal : BINORMAL0;
};

struct VS_OUTPUT_PERPIXELNORMAL {
	float4 position      : POSITION0;
	float3 tangent       : TEXCOORD0;
	float3 binormal      : TEXCOORD1;
	float3 normal        : TEXCOORD2;
	float3 positionworld : TEXCOORD3;
	float2 coord         : TEXCOORD4;
	float4 projcoord     : TEXCOORD5;
};

VS_OUTPUT_PERPIXELNORMAL PerPixelNormalVS(VS_INPUT_NORMALMAP In) {
	VS_OUTPUT_PERPIXELNORMAL outVS;
	
	// Bereken pixelcoordinaat (xy, z = diepte,w negeren)
	outVS.position = mul(float4(In.position,1.0f),WorldViewProj);

	outVS.tangent  = mul(In.tangent, (float3x3)World); // undo scaling by normalizing in the PS
	outVS.binormal = mul(In.binormal,(float3x3)World);
	outVS.normal   = mul(In.normal,  (float3x3)World);

	outVS.positionworld = mul(float4(In.position,1.0f),World).xyz;
	outVS.coord = In.coord;

	// Mul met lightpos, dan coords verschuiven, zodat we coords relatief aan licht hebben
	outVS.projcoord = mul(mul(float4(In.position,1.0f), LightWorldViewProj),ShadowOffset);

	// En doorgeven aan de pixelshader, die per pixel de kleur berekent
	return outVS;
}

PS_OUTPUT PerPixelNormalPS(VS_OUTPUT_PERPIXELNORMAL In) {
	PS_OUTPUT outPS;

	// Converteer de normal terug naar world i.p.v. alles naar tangent
	float3 normalworld = GetWorldNormal(In.coord,GetInvTBN(In.tangent,In.binormal,In.normal));
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Loop de zonnen langs
	float3 diffuse = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		
		diffuse += GetDiffuseValuePhong(color,normalworld,lightdir) * GetDirlightShadow(i,In.projcoord);
	}
	
	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = pointlights[i].color;
		
		diffuse += GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir) * GetPointlightShadow(i,In.projcoord);
	}

	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-In.positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		
		diffuse += spotvalue * GetDiffuseValuePhong(color/distance(lightpos,In.positionworld),normalworld,lightdir) * GetSpotlightShadow(i,In.projcoord);
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

technique PerPixelNormal {
	pass P0 {
		VertexShader = compile vs_3_0 PerPixelNormalVS();
		PixelShader  = compile ps_3_0 PerPixelNormalPS();
	}
}
