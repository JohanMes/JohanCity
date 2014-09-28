float4 GaussFilterPS(float2 coordin : TEXCOORD0) : COLOR0 {
    float4 outcolor0 = 0;
	for(int a = 0;a < 7;a++) {
		for(int b = 0;b < 7;b++) {
			outcolor0 += gausspartone[a].z*gaussparttwo[b].z*tex2D(ScreenSpaceSampler1, coordin+float2(gausspartone[a].x*invwidth,gaussparttwo[b].y*invheight));
		}
	}
	return outcolor0;
}

technique GaussFilter {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 GaussFilterPS();
	}
}
