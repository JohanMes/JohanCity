float4 SepiaPS(float2 coordin : TEXCOORD0) : COLOR0 {
	return mul(tex2D(ScreenSpaceSampler1, coordin),float4(0.299f,0.587f,0.184f,0.0f))+float4(0.314f,0.169f,-0.090f,0.0f);
}

technique Sepia {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 SepiaPS();
	}
}
