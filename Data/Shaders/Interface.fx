float4 InterfacePS() : COLOR0 {
	return interfacecolor;
}

technique Interface {
	pass P0 {
		VertexShader = compile vs_3_0 ScreenSpaceVS();
		PixelShader  = compile ps_3_0 InterfacePS();
	}
}
