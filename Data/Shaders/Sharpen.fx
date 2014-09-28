float4 SharpenPS(float2 coordin : TEXCOORD0) : COLOR0 {
	float4 outcolor0 = 0;
	for(int a = 0;a < 5;a++)
		outcolor0 += edge[a].z*tex2D(ScreenSpaceSampler1, coordin+float2(edge[a].x*invwidth,edge[a].y*invheight));

	return outcolor0;
}

technique Sharpen {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 SharpenPS();
	}
}
