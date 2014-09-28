////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Simpelweg niets anders dan de texture renderen, maar met schaduw eroverheen
*/

struct VS_OUTPUT_NOSHADE {
	float4 position  : POSITION0;
	float2 coord     : TEXCOORD0;
	float4 projcoord : TEXCOORD1;
};

VS_OUTPUT_NOSHADE NoShadeVS(float3 positionin : POSITION0, float2 coordin : TEXCOORD0) {
	VS_OUTPUT_NOSHADE outVS;
	
	// Bereken pixelcoordinaat (xy, z = diepte, w negeren)
	outVS.position = mul(float4(positionin,1.0f),WorldViewProj);
	
	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = coordin;
	
	// Mul met lightpos, dan coords verschuiven, zodat we coords relatief aan licht hebben
	outVS.projcoord = mul(mul(float4(positionin,1.0f), LightWorldViewProj),ShadowOffset);

	return outVS;
}
PS_OUTPUT NoShadePS(VS_OUTPUT_NOSHADE In) {
	PS_OUTPUT outPS;
	
	// Voor perspectief, maak homogeen
	In.projcoord.xyz /= In.projcoord.w;
	
	// Laat schaduwen de RGB beinvloeden
	float shadow = GetShadowValue(In.projcoord);
	
	float4 texColor = tex2D(DiffuseSampler, In.coord);
	outPS.color.rgb = shadow*texColor.rgb;
	
	// Maar de alpha niet
	outPS.color.a = texColor.a;
	
	return outPS;
}

technique NoShade {
	pass P0 {
		VertexShader = compile vs_3_0 NoShadeVS();
		PixelShader  = compile ps_3_0 NoShadePS();
	}
}
