float4 InversePS(float2 coordin : TEXCOORD0) : COLOR0 {
	return 1.0f - tex2D(ScreenSpaceSampler1, coordin);
}

technique Inverse {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 InversePS();
	}
}
