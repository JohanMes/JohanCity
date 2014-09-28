struct VS_OUTPUT_AMBIENTMAP {
	float4 position 		: POSITION0;
	float3 normalview 		: TEXCOORD0;
	float2 depthview 		: TEXCOORD1;
	float2 coord			: TEXCOORD2;
};

VS_OUTPUT_AMBIENTMAP AmbientMapVS(VS_INPUT In) {
	VS_OUTPUT_AMBIENTMAP outVS;
	
	// Bereken pixelcoordinaat (xy, z = diepte,w negeren)
	outVS.position = mul(float4(In.position, 1.0f), WorldViewProj);

	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = In.coord;

	// Bereken randen op scherm m.b.v. normaalverschillen
	outVS.normalview = mul(In.normal, (float3x3)WorldView);
	outVS.depthview = outVS.position.zw;
	return outVS;
}

float4 AmbientMapPS(VS_OUTPUT_AMBIENTMAP In) : COLOR0 {
	clip(tex2D(DiffuseSampler, In.coord).a-0.5f);
	return float4(In.normalview,In.depthview.x/In.depthview.y);
}

technique AmbientMap {
	pass P0 {
		VertexShader = compile vs_3_0 AmbientMapVS();
		PixelShader  = compile ps_3_0 AmbientMapPS();
	}
}
