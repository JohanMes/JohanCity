struct VS_OUTPUT_SHADOWMAP {
	float4 position	: POSITION0;
	float2 coord	: TEXCOORD0;
	float2 depth	: TEXCOORD1;
};

VS_OUTPUT_SHADOWMAP ShadowMapVS(VS_INPUT In) {
	VS_OUTPUT_SHADOWMAP outVS;

	// Bereken pixelcoordinaat relatief aan zon (xy, z = diepte,w negeren)
	outVS.position = mul(float4(In.position, 1.0f), LightWorldViewProj);
	
	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = In.coord;

	// De diepte van de vertex moet uiteindelijk in de pixel terecht komen
	outVS.depth = outVS.position.zw;
	return outVS;
}

float4 ShadowMapPS(VS_OUTPUT_SHADOWMAP In) : COLOR0 {
	clip(tex2D(DiffuseSampler, In.coord).a-0.5f);
	return In.depth.x/In.depth.y; // sla in bereik [0..1] op
}

technique ShadowMap {
	pass P0 {
		VertexShader = compile vs_3_0 ShadowMapVS();
		PixelShader  = compile ps_3_0 ShadowMapPS();
	}
}
