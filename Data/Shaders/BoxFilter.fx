float4 BoxFilterPS(float2 coordin : TEXCOORD0) : COLOR0 {
	float4 outcolor0 = 0;
	for(int a = 0;a < 4;a++) {
		
		// Ga een doos rond de huidige pixel af
		float2 coord = coordin+float2(box[a].x*invwidth,box[a].y*invheight);
		outcolor0 += box[a].z*tex2D(ScreenSpaceSampler1,coord);
	}
	return outcolor0;
}

technique Box {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 BoxFilterPS();
	}
}
