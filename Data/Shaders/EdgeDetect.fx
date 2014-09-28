float4 EdgeDetectPS(float2 coordin : TEXCOORD0) : COLOR0 {
	float2 derivate = 0.0f;
	float luminance = 0.0f;
	for(int x = 0;x < 3;x++) {
		for(int y = 0;y < 3;y++) {
			luminance = mul(tex2D(ScreenSpaceSampler1,coordin+float2(invwidth*x,invheight*y)).rgb,float3(0.299f,0.587f,0.184f));
			derivate.x += edgex[x][y]*luminance;
			derivate.y += edgey[x][y]*luminance;
		}
	}
	return length(derivate)*2.0f;
}

technique EdgeDetect {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 EdgeDetectPS();
	}
}
