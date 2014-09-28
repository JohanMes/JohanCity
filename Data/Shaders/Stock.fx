struct VS_OUTPUT_SCREEN {
	float4 position			: POSITION0;
	float2 coord			: TEXCOORD0;
};

struct PS_OUTPUT {
	float4 color			: COLOR0;
};

VS_OUTPUT_SCREEN ScreenSpaceVS(float4 positionin : POSITION0,float2 coordin : TEXCOORD0) {
	VS_OUTPUT_SCREEN outVS;

	// Geef een fake float4 mee, de w is altijd leeg (wordt niet geset door CPU, scheelt 1 instructie
	outVS.position = positionin;

	// Coords met screenpixel/texelcorrectie van een halve pixel
	outVS.coord.x = coordin.x + 0.5f * invwidth;
	outVS.coord.y = coordin.y + 0.5f * invheight;

	return outVS;
}

float4 StockPS(float2 coordin : TEXCOORD0) : COLOR0 {
	return tex2D(ScreenSpaceSampler1,coordin);
}

technique Stock {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 StockPS();
	}
}
