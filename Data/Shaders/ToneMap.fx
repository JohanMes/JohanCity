float4 ToneMapPS(float2 coordin : TEXCOORD0) : COLOR0 {
	
	// Doe een echte Camera na door vignetting toe te passen
	float2 halftexcoord = float2(coordin - 0.5f);
	float vignette = pow(1.0f-0.4f*dot(halftexcoord,halftexcoord),2.0f);
	
	float3 hdrcolor = tex2D(ScreenSpaceSampler1,coordin).rgb;
	float3 ldrcolor = JohnHable(hdrcolor);
	float3 bloom = tex2D(ScreenSpaceSampler2,coordin).rgb;
	
	return float4((ldrcolor + bloom * bloommultiplier) * exposure * vignette,1.0f);
}

//	// Hier gaan we op aanpassen (werkt niet?)
//	float3 avgcolor = tex2Dbias(ScreenSpaceSampler,float4(coordin,0.0f,100.0f)).rgb;
//	float avgintensity = (avgcolor.r + avgcolor.g + avgcolor.b)/3.0f;
//	
//	// TODO: adjust exposure value itself?
//	float irisexposure = lerp(exposure,0.5f/avgintensity,0.1f);

technique ToneMap {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 ToneMapPS();		
	}
}
