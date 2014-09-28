#include "resource.fx"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Screenspace
*/
#include "Stock.fx"
#include "BlackWhite.fx"
#include "BoxFilter.fx"
#include "EdgeDetect.fx"
#include "GaussFilter.fx"
#include "Inverse.fx"
#include "Interface.fx"
#include "Sepia.fx"
#include "Sharpen.fx"
#include "SSAO.fx"
#include "BrightPass.fx"
#include "BrightBlur.fx"
#include "ToneMap.fx"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Per vertex
*/
#include "PerVertex.fx"
#include "PureTexture.fx" // alleen textures, niets
#include "PureTextureMultitexConst.fx" // alleen 3 textures, niets
#include "NoShade.fx" // textures met schaduwen
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Per pixel
*/
#include "PerPixel.fx"
#include "PerPixelSpecular.fx"
#include "PerPixelGooch.fx"
#include "PerPixelMinnaert.fx"
#include "PerPixelMultitexConst.fx"
#include "PerPixelMultitexMixer.fx"
#include "PerPixelOrenNayar.fx"
#include "PerPixelCookTorrance.fx"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Per pixel, tangent space
*/
#include "PerPixelNormal.fx"
#include "PerPixelNormalSpecular.fx"
#include "PerPixelNormalParallax.fx"
#include "PerPixelNormalSpecularAmbient.fx"
#include "PerPixelNormalSpecularParallax.fx"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
	Speciale effecten
*/
#include "ShadowMap.fx"
#include "AmbientMap.fx"
