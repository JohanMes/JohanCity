float4 BrightBlurPS(float2 coordin : TEXCOORD0) : COLOR0 {
	float4 blurredbrightpass = 0;
	for(int a = 0;a < 7;a++) {
		for(int b = 0;b < 7;b++) {
			float2 coord = coordin + 2.0f * float2(gausspartone[a].x*invwidth,gaussparttwo[b].y*invheight); // half size buffer
			blurredbrightpass += gausspartone[a].z*gaussparttwo[b].z*tex2D(ScreenSpaceSampler1,coord);
		}
	}
	return blurredbrightpass;
}
// Ga een doos rond de huidige pixel af
technique BrightBlur {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 BrightBlurPS();		
	}
}
