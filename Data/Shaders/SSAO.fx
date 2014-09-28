float4 SSAOPS(float2 coordin : TEXCOORD0) : COLOR0 {
	
	float3 viewpospixel = GetPosition(coordin);
	float3 viewnompixel = tex2D(SSAOSampler,coordin).rgb;
	float rad = ssaorad/viewpospixel.z;
	float2 random = 1;//GetRandomValue(coordin).xy;

	float occlusion = 0.0f;
	for(int a = 0;a < ssaosamples;a++) {
		float2 coord1 = reflect(ssao[a],random)*rad;
		float2 coord2 = float2(coord1.x - coord1.y, coord1.x + coord1.y) * 0.707f;
		occlusion += GetOcclusion(coordin+coord1*0.25f,viewpospixel,viewnompixel);
		occlusion += GetOcclusion(coordin+coord2*0.5f, viewpospixel,viewnompixel);
		occlusion += GetOcclusion(coordin+coord1*0.75f,viewpospixel,viewnompixel);
		occlusion += GetOcclusion(coordin+coord2,      viewpospixel,viewnompixel);
	}
	occlusion /= (float)ssaosamples*4.0f;
	return tex2D(ScreenSpaceSampler1,coordin) - occlusion;
}

technique SSAO {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 SSAOPS();
	}
}
