////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Transforms
*/
uniform extern float4x4 World; // object -> world
uniform extern float4x4 WorldView; // object -> world -> view
uniform extern float4x4 WorldViewProj; // object -> world -> view -> proj
uniform extern float4x4 ProjInverse; // proj -> view
uniform extern float4x4 ShadowOffset; // sample center of pixels
uniform extern float4x4 LightWorldViewProj;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Materialtemps
*/
uniform extern float materialdiffuse;
uniform extern float materialspecular;
uniform extern float materialshininess;
uniform extern float materialtiling;
uniform extern float3 materialmixer; // use this or a texture for mixing purposes
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Alle directionele lampen (waaronder zon)
*/
struct dirlight { // can cast shadows
	float3 direction;
	float3 color;
};
uniform extern int dirlightshadowindex; // -1 for unused
uniform extern int numdirlights;
uniform extern dirlight dirlights[16];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Alle lampen
*/
struct pointlight { // can cast shadows
	float3 position;
	float3 color;
};
uniform extern int pointlightshadowindex;
uniform extern int numpointlights;
uniform extern pointlight pointlights[16];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Alle zaklampen
*/
struct spotlight { // can cast shadows
	float3 position;
	float3 direction;
	float3 color;
	float angle;
};
uniform extern int spotlightshadowindex;
uniform extern int numspotlights;
uniform extern spotlight spotlights[16];
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Texturingopties
*/
uniform extern int filter;
uniform extern int numaniso;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Allerhande resolutieconstanten
*/
uniform extern int width;
uniform extern int height;
uniform extern float invwidth;
uniform extern float invheight;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Schaduwspulletjes
*/
uniform extern float shadowcoeff;
uniform extern float shadowbias;
uniform extern float invshadowmapsize;
uniform extern bool enableshadows;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	SSAO-opties
*/
uniform extern int ssaosamples;
uniform extern float ssaorad;
uniform extern float ssaomulti;
uniform extern float ssaoscale;
uniform extern float ssaobias;
uniform extern float ssaodepth;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	HDR-opties
*/
uniform extern float exposure;
uniform extern float bloomrampstart;
uniform extern float bloomrampend;
uniform extern float bloommultiplier;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	textures
*/
uniform extern texture diffusetex;
uniform extern texture speculartex;
uniform extern texture normaltex;
uniform extern texture parallaxtex;
uniform extern texture ambienttex;
uniform extern texture shadowtex;
uniform extern texture ssaotex;
uniform extern texture screentex1;
uniform extern texture screentex2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Misc.
*/
uniform extern float timevar;
uniform extern float fogcolor;
uniform extern float3 camerapos;
uniform extern float minviewdistance;
uniform extern float maxviewdistance;
uniform extern float4 interfacecolor;
uniform extern float lightambient;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Locals
static const int fogstart = 0.8f*maxviewdistance; // Plek vanaf Camera waar fog begint
static const int fogrange = 0.2f*maxviewdistance; // Breedte begin tot eind fade
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Filters
static const float3 box[4] = { // delta X, delta Y, weight
	-1.0f,  0.0f, 0.25f,
	 1.0f,  0.0f, 0.25f,
	 0.0f,  1.0f, 0.25f,
	 0.0f, -1.0f, 0.25f,
};
static const float3 gausspartone[7] = {
	-3.0f,  0.0f, 0.015625f,
	-2.0f,  0.0f, 0.09375f,
	-1.0f,  0.0f, 0.234375f,
	 0.0f,  0.0f, 0.3125f,
	 1.0f,  0.0f, 0.234375f,
	 2.0f,  0.0f, 0.09375f,
	 3.0f,  0.0f, 0.015625f,
};
static const float3 gaussparttwo[7] = {
	 0.0f, -3.0f, 0.015625f,
	 0.0f, -2.0f, 0.09375f,
	 0.0f, -1.0f, 0.234375f,
	 0.0f,  0.0f, 0.3125f,
	 0.0f,  1.0f, 0.234375f,
	 0.0f,  2.0f, 0.09375f,
	 0.0f,  3.0f, 0.015625f,
};
static const float3 edgex[3] = {
	-1.0f,  0.0f,  1.0f,
	-2.0f,  0.0f,  2.0f,
	-1.0f,  0.0f,  1.0f,
};
static const float3 edgey[3] = {
	-1.0f, -2.0f, -1.0f,
	 0.0f,  0.0f,  0.0f,
	-1.0f, -2.0f, -1.0f,
};
static const float3 edge[5] = {
	 0.0f,  0.0f,  4.0f,
	 0.0f,  1.0f, -1.0f,
	 1.0f,  0.0f, -1.0f,
	 0.0f, -1.0f, -1.0f,
	-1.0f,  0.0f, -1.0f,
};
static const float2 ssao[8] = {
     1.0f,  0.0f,
	-1.0f,  0.0f,
	 0.0f,  1.0f,
	 0.0f, -1.0f,
	 1.0f,  1.0f,
	-1.0f, -1.0f,
	 1.0f, -1.0f,
	-1.0f,  1.0f,
};
static const float2 pcf[4] = {
	 -0.5f, -0.5f,
	 -0.5f,  0.5f,
	  0.5f, -0.5f,
	  0.5f,  0.5f,
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Vijf dezelfde samplers omdat we ze tegelijk nodig hebben
*/
sampler DiffuseSampler = sampler_state {
	Texture = <diffusetex>;
	MinFilter = filter; // bilinear, trilinear, anisotropic
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	MaxAnisotropy = numaniso; // ignored when not anisotropic
};
sampler SpecularSampler = sampler_state {
	Texture = <speculartex>;
	MinFilter = filter;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	MaxAnisotropy = numaniso;
};
sampler NormalSampler = sampler_state {
	Texture = <normaltex>;
	MinFilter = filter;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	MaxAnisotropy = numaniso;
};
sampler ParallaxSampler = sampler_state {
	Texture = <parallaxtex>;
	MinFilter = filter;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	MaxAnisotropy = numaniso;
};
sampler AmbientSampler = sampler_state {
	Texture = <ambienttex>;
	MinFilter = filter;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Wrap;
	AddressV = Wrap;
	MaxAnisotropy = numaniso;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Speciale samplers, ook tegelijk nodig
*/
sampler ScreenSpaceSampler1 = sampler_state {
	Texture = <screentex1>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};
sampler ScreenSpaceSampler2 = sampler_state {
	Texture = <screentex2>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};
sampler SSAOSampler = sampler_state {
	Texture = <ssaotex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Clamp;
	AddressV = Clamp;
};
sampler ShadowMapSampler = sampler_state {
	Texture = <shadowtex>;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	AddressU = Border;
	AddressV = Border;
	BorderColor = 0xFFFFFFFF;
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Etc.
*/
float GetFogValue(float3 camerapos,float3 position) {
	float dist = distance(camerapos,position);
	return saturate((dist - fogstart) / fogrange);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Diffuse, doen niets met lichtstructs
*/
float3 GetDiffuseValuePhong(float3 color,float3 normal,float3 lightdir) {
	float factorD = max(dot(lightdir, normal), 0.0f);
	return factorD*materialdiffuse*color;
}
float3 GetDiffuseValueMinnaert(float3 color,float3 normal,float3 lightdir,float3 toeye) {
	float VdotN = max(dot(toeye,normal), 0.0f);
	float LdotN = max(dot(lightdir,normal), 0.0f);
	float factorD = pow(max(VdotN * LdotN,0.0f),1.5f);
	return factorD*materialdiffuse*color;
}
float3 GetDiffuseValueOrenNayar(float3 color,float3 normal,float3 lightdir,float3 toeye) {
	
	// Roughness
	float ssquare = 0.2f;
	
	// Dot products
	float VdotN = max(dot(toeye,normal), 0.0f);
	float LdotN = max(dot(lightdir,normal), 0.0f);
	
	// Voor de derde factor
	float3 Ca = normalize(toeye - normal * VdotN);
	float3 Cb = normalize(lightdir - normal * LdotN);
		
	// Magic factors
	float A = 1.0f - 0.5f*(ssquare/(ssquare + 0.33f));
	float B = 0.45f*(ssquare/(ssquare + 0.09f));
	float C = max(0.0f,dot(Ca,Cb));
	
	// More magic factors
	float a = max(acos(LdotN),acos(VdotN));
	float b = min(acos(LdotN),acos(VdotN));
	
	float factorD = LdotN * (A + B * sin(a) * tan(b) * max(cos(C),0.0f));
	return factorD*materialdiffuse*color;
}
float3 GetDiffuseValueCookTorrance(float3 color,float3 normal,float3 lightdir,float3 toeye) {

	float VdotN = max(dot(toeye,normal), 0.0f);
	float LdotN = max(dot(lightdir,normal), 0.0f);
	
	float F = 0.01f; // nog uitwerken
	float D = 0.01f;
	float G = 0.5f;
	
	float factorD = F*D*G/(VdotN * LdotN);
	
	return factorD*materialdiffuse*color;
}
float3 GetDiffuseValueGooch(float3 color,float3 normal,float3 lightdir) {

	float LdotN = dot(lightdir,normal);
	
	float3 cool = float3(0.0f,0.0f,1.0f);
	float3 warm = float3(1.0f,1.0f,0.0f);
	
	float3 factorD = (1.0f + LdotN)/2.0f * cool + (1.0f - (1.0f + LdotN)/2.0f) * warm;

	return factorD*materialdiffuse;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Specular
*/
float3 GetSpecularValuePhong(float3 color,float3 normal,float3 lightdir,float3 toeye) {
	float3 r = reflect(-lightdir, normal);
	float factorS = pow(max(dot(r, toeye),0.0f),materialshininess);
	return factorS*materialspecular*color;
}
float3 GetSpecularValuePhong(float3 color,float3 normal,float3 lightdir,float3 toeye,float2 coord) {
	return tex2D(DiffuseSampler,coord).rgb*GetSpecularValuePhong(color,normal,lightdir,toeye);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Shadows
*/
float GetShadowValue(float4 projcoordin) {
	if(enableshadows) {
		float pcffactor = 0.0f;
		for(int i = 0;i < 4;i++) { // TODO: use hardware PCF
			if(tex2D(ShadowMapSampler,projcoordin.xy + pcf[i] * invshadowmapsize).r + shadowbias < projcoordin.z) {
				pcffactor += shadowcoeff;
			} else {
				pcffactor += 1.0f;
			}
		}
		return pcffactor/4.0f;
	} else {
		return 1.0f;
	}
}
float GetDirlightShadow(int index,float4 projcoord) {
	if(index == dirlightshadowindex) {
		return GetShadowValue(projcoord);
	} else {
		return 1.0f;
	}
}
float GetPointlightShadow(int index,float4 projcoord) { // TODO
//	if(index == pointlightshadowindex) {
//		return GetShadowValue(projcoord);
//	} else {
		return 1.0f;
//	}
}
float GetSpotlightShadow(int index,float4 projcoord) {
	if(index == spotlightshadowindex) {
		return GetShadowValue(projcoord);
	} else {
		return 1.0f;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Functies voor SSAO
*/
float3 GetPosition(float2 coordin) {
	float x = 2.0f*coordin.x - 1.0f;
	float y = 2.0f*(1.0f - coordin.y) - 1.0f;
	float z = tex2Dlod(ScreenSpaceSampler1,float4(coordin,0.0f,0.0f)).a;
	float4 viewpos = mul(float4(x,y,z,1.0f), ProjInverse);
	return viewpos.xyz/viewpos.w;
}
float GetOcclusion(float2 coordin,float3 viewpospixel,float3 viewnompixel) {
	float3 normal = GetPosition(coordin) - viewpospixel; // als z negatief is, hebben we occlusion
	float3 normalnormaled = normalize(normal);
	float occdepth = length(normal)*ssaoscale;
	return max(0.0f,dot(viewnompixel,normalnormaled)-ssaobias) * (1.0f/(ssaodepth+occdepth)) * ssaomulti;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	HDR
*/
float3 JohnHable(float3 x) {
//	float A = 0.15;
//	float B = 0.50;
//	float C = 0.10;
//	float D = 0.20;
//	float E = 0.02;
//	float F = 0.30;
	
	float A = 0.22;
	float B = 0.30;
	float C = 0.10;
	float D = 0.20;
	float E = 0.01;
	float F = 0.30;
	
	return ((x*(A*x+C*B)+D*E)/(x*(A*x+B)+D*F))-E/F;
}
float3 Reinhard(float3 x) {
	return x/(x + 1.0f);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Misc.
*/
float GetSpotValue(float3 spotdirection,float3 objectdirection,float spotangle) {
	return pow(max(dot(spotdirection,-objectdirection), 0.0f), spotangle);
}
float3x3 GetTBN(float3 tangent,float3 binormal,float3 normal) {
	
	// Ze kunnen > 1 worden tijdens interpoleren...
	tangent = normalize(tangent);
	binormal = normalize(binormal);
	normal = normalize(normal);
	
	// Converteer hiermee van world naar tangent, is alleen geldig bij mul(vec,matrix)
	return transpose(float3x3(tangent,binormal,normal)); // kolommen
}
float3x3 GetInvTBN(float3 tangent,float3 binormal,float3 normal) {
	
	// Ze kunnen > 1 worden tijdens interpoleren...
	tangent = normalize(tangent);
	binormal = normalize(binormal);
	normal = normalize(normal);
	
	// Converteer hiermee van tangent naar world, is alleen geldig bij mul(vec,matrix)
	return float3x3(tangent,binormal,normal); // rijen
}
float3 GetWorldNormal(float2 coord,float3x3 InvTBN) {
	
	// Converteer deze normal in tangent space van [0..1] naar [-1..1]
	float3 normaltangent = 2.0f*tex2D(NormalSampler,coord).rgb-1.0f;

	return normalize(mul(normaltangent,InvTBN)); // rijvectoren...
}
float2 GetParallaxCoord(float2 coord,float3 toeye,float3x3 TBN) {
	
	// Maak een genormaliseerde toeye in tangent space
	float3 toeyetangent = normalize(mul(toeye,TBN)); // we gebruiken overal rijvectoren, dus vec*matrix gebruiken
	
	// Verschuif coord gebaseerd op hoogte
	float biasedheight = tex2D(ParallaxSampler,coord).x * 0.05f - 0.025f;
	
	// Coord.xy ligt vlak op het, eh, vlak (z = normal)
	return coord + toeyetangent.xy * biasedheight;
}
