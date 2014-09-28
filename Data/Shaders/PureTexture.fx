////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Simpelweg niets anders dan 1 texture renderen, en ook zonder schaduwen
*/

struct VS_OUTPUT_PURETEXTURE {
	float4 position  : POSITION0;
	float2 coord     : TEXCOORD0;
};

VS_OUTPUT_PURETEXTURE PureTextureVS(float3 positionin : POSITION0, float2 coordin : TEXCOORD0) {
	VS_OUTPUT_PURETEXTURE outVS;
	
	// Bereken pixelcoordinaat (xy, z = diepte, w negeren)
	outVS.position = mul(float4(positionin,1.0f),WorldViewProj);
	
	// Geef texturecoord door, want we samplen in de pixel shader
	outVS.coord = coordin;
	
	return outVS;
}

PS_OUTPUT PureTexturePS(VS_OUTPUT_PURETEXTURE In) {
	PS_OUTPUT outPS;
	
	outPS.color = tex2D(DiffuseSampler,In.coord);
	outPS.color.rgb *= materialdiffuse;
	
	return outPS;
}

technique PureTexture {
	pass P0 {
		VertexShader = compile vs_3_0 PureTextureVS();
		PixelShader  = compile ps_3_0 PureTexturePS();
	}
}
