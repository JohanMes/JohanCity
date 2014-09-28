float4 BlackWhitePS(float2 coordin : TEXCOORD0) : COLOR0 {
	return mul(tex2D(ScreenSpaceSampler1,coordin).rgb,float3(0.299f,0.587f,0.184f));
}

technique BlackWhite {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 BlackWhitePS();
	}
}
