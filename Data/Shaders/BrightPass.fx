float4 BrightPassPS(float2 coordin : TEXCOORD0) : COLOR0 {
	float3 hdrcolor = tex2D(ScreenSpaceSampler1,coordin).rgb;
	return float4(min(hdrcolor,max(0,(hdrcolor - bloomrampstart) / (bloomrampend - bloomrampstart))),1);  // smoothly ramp up to threshold and keep increasing as usual
}

technique BrightPass {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 BrightPassPS();		
	}
}
