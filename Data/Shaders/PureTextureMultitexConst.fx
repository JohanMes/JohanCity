////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Simpelweg niets anders dan 3 textures gemixt renderen, en ook zonder schaduwen
*/

PS_OUTPUT PureTextureMultitexConstPS(VS_OUTPUT_PURETEXTURE In) {
	PS_OUTPUT outPS;
	
	outPS.color = tex2D(DiffuseSampler,In.coord);
	outPS.color.rgb *= materialdiffuse;
	
	// We halen mixing weights uit een variabele...
	float4 tex1 = materialmixer.x*tex2D(DiffuseSampler, In.coord*materialtiling);
	float4 tex2 = materialmixer.y*tex2D(SpecularSampler,In.coord*materialtiling);
	float4 tex3 = materialmixer.z*tex2D(NormalSampler,  In.coord*materialtiling);

	float4 texColor = tex1+tex2+tex3;

	// Geen fog of schaduwen of shit
	outPS.color = texColor;
	
	return outPS;
}

technique PureTextureMultitexConst {
	pass P0 {
		VertexShader = compile vs_3_0 PureTextureVS();
		PixelShader  = compile ps_3_0 PureTextureMultitexConstPS();
	}
}
