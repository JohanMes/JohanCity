////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Blinn/Phong, berekend per vertex
*/

struct VS_INPUT {
	float3 position : POSITION0;
	float2 coord    : TEXCOORD0;
	float3 normal   : NORMAL0;
};

struct VS_OUTPUT_PERVERTEX {
	float4 position  : POSITION0;
	float2 coord     : TEXCOORD0;
	float4 projcoord : TEXCOORD1;
	float3 color     : TEXCOORD2;
	float  fog       : TEXCOORD3;
};

VS_OUTPUT_PERVERTEX PerVertexVS(VS_INPUT In) {
	VS_OUTPUT_PERVERTEX outVS;

	// Bereken pixelcoordinaat (xy, z = diepte,w negeren)
	outVS.position = mul(float4(In.position,1.0f),WorldViewProj);
	
	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = In.coord;

	// Mul met lightpos, dan coords verschuiven, zodat we coords relatief aan licht hebben
	outVS.projcoord = mul(mul(float4(In.position,1.0f), LightWorldViewProj),ShadowOffset);

	// Bereken de kleur per vertex, dus geef niks door
	float3 positionworld = mul(float4(In.position,1.0f),World).xyz;
	float3 normalworld = normalize(mul(In.normal,(float3x3)World));

	// Loop de zonnen langs
	float3 diffuse = 0;
	for(int i = 0;i < numdirlights;i++) {
		
		float3 lightdir = dirlights[i].direction;
		float3 color = dirlights[i].color;		
		
		diffuse += GetDiffuseValuePhong(color,normalworld,lightdir);
	}
	
	// Loop de lampen langs
	for(i = 0;i < numpointlights;i++) {
		
		float3 lightpos = pointlights[i].position;
		float3 lightdir = normalize(lightpos-positionworld);
		float3 color = pointlights[i].color;
		
		diffuse += GetDiffuseValuePhong(color/distance(lightpos,positionworld),normalworld,lightdir);
	}
	
	// Loop de zaklampen langs
	for(i = 0;i < numspotlights;i++) {
		
		float3 lightpos = spotlights[i].position;
		float3 lightdir = normalize(lightpos-positionworld);
		float3 color = spotlights[i].color;
		float spotvalue = GetSpotValue(lightdir,spotlights[i].direction,spotlights[i].angle);
		
		diffuse += spotvalue * GetDiffuseValuePhong(color/distance(lightpos,positionworld),normalworld,lightdir);
	}

	// Geef de kleuren per vertex dooor aan de pixel shader
	outVS.color = diffuse + lightambient;
	outVS.fog = GetFogValue(camerapos,positionworld);
	
	// En stuur kleur en positie naar Pixel FXShader
	return outVS;
}
PS_OUTPUT PerVertexPS(VS_OUTPUT_PERVERTEX In) {
	PS_OUTPUT outPS; // gewoon een float4...
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;

	// Laat schaduwen de RGB beinvloeden
	float shadow = GetShadowValue(In.projcoord); // doe dit per pixel, dus schaduw beïnvloedt alle lichten
	
	// Vul elke pixel in
	float4 texColor = tex2D(DiffuseSampler, In.coord);
	outPS.color.rgb = lerp(shadow*texColor.rgb*In.color,fogcolor,In.fog);
	outPS.color.a = texColor.a;
	
	return outPS;
}

technique PerVertex {
	pass P0 {
		VertexShader = compile vs_3_0 PerVertexVS();
		PixelShader  = compile ps_3_0 PerVertexPS();
	}
}
