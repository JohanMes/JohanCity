#ifndef RENDERER_INCLUDE
#define RENDERER_INCLUDE

#include "Object.h"
#include "FXShader.h"
#include "Camera.h"
#include "Component.h"
#include "State.h"
#include "Clock.h"
#include "FXVariable.h"
#include "Scene.h"
#include "OptionsDialog.h"
#include "Interface.h"
#include "float2.h"

#if BUILDING_DLL
#define DLLIMPORT __declspec(dllexport)
#else
#define DLLIMPORT __declspec(dllimport)
#endif

class DLLIMPORT Renderer {

	// Statemachines
	State* cullmodestate;
	State* fillmodestate;
	State* alphateststate;
	State* alphablendstate;
	
	// Frametijdmeter
	Clock* frameclock;

	LPDIRECT3DVERTEXDECLARATION9 standard; // Inhoud per vertex
	LPDIRECT3DVERTEXDECLARATION9 normalmap;
	
	// Size editing
	RECT oldwindowsize;
	
	// SSAO
	LPDIRECT3DTEXTURE9 AmbientTex;
	LPDIRECT3DSURFACE9 AmbientTexTopSurface;
	
	// De backbuffer, maar dan nog in floatformaat
	LPDIRECT3DTEXTURE9 FloatTex;
	LPDIRECT3DSURFACE9 FloatTexTopSurface;

	// Pingpongbuffers op halve grootte (o.a. voor bloom)
	LPDIRECT3DTEXTURE9 SmallFloatTex1;
	LPDIRECT3DSURFACE9 SmallFloatTex1TopSurface;
	LPDIRECT3DTEXTURE9 SmallFloatTex2;
	LPDIRECT3DSURFACE9 SmallFloatTex2TopSurface;
	
	// Met dit model passen we postprocessing toe
	Model* screenspacequad;
	
	// tooltipspul
	int tooltipmode;
	char tooltip[500];
	
	// Counters
	int drawnfaces;
	int drawnvertices;
	int drawcalls;
	
	// Zooi die voor bufferopslag zorgt
	bool schedulesave;

	// Rendering stuff
	void SetCameraTransforms(Object* object);
	void SetLightTransforms(Object* object,float4x4 lightprojection);
	void SetMaterial(Object* object);
	void SetSSAOVariables(Object* object);
	void SetShadowMapVariables(Object* object,float4x4 lightprojection);
	void SetGenericVariables(Object* object,float4x4 lightprojection);
	void PassShader(FXShader* shader);
	
	// Tooltipspul
	void PrintTooltip();

	// Misc.
	void UpdateTime();
	
	// Frametijden enzo
	std::vector<float> frametimes;
	float framerate;
	float frameratechecktime;
	int framecount;
	float lotime;
	float hitime;
	bool saveframetimes;
	int timemulti;
	
	// Variabelen van GPU
	FXVariable* FXMatWorld;
	FXVariable* FXMatWorldView;
	FXVariable* FXMatWorldViewProj;
	FXVariable* FXMatLightWorldViewProj;
	
	FXVariable* FXMaterialdiffuse;
	FXVariable* FXMaterialspecular;
	FXVariable* FXMaterialshininess;
	FXVariable* FXMaterialTiling;
	FXVariable* FXMaterialMixer;
	
	FXVariable* FXWidth;
	FXVariable* FXHeight;
	FXVariable* FXInvwidth;
	FXVariable* FXInvheight;
	
	FXVariable* FXTimevar;
	FXVariable* FXInterfacecolor;
	
	// Texturepointers naar GPU
	FXVariable* FXScreentex1;
	FXVariable* FXScreentex2;
	FXVariable* FXDiffusetex;
	FXVariable* FXSpeculartex;
	FXVariable* FXNormaltex;
	FXVariable* FXParallaxtex;
	FXVariable* FXAmbienttex;
	FXVariable* FXShadowtex;
	FXVariable* FXSSAOtex;
	
	LPD3DXFONT tooltipfontwhite; // Het GPU-font (wit)
	LPD3DXFONT tooltipfontshadow; // Het GPU-font (zwartborder)
	LPD3DXFONT interfacefont; // Het GPU-font (ui)
	LPD3DXSPRITE interfacesprite;
	LPD3DXSPRITE tooltipfontsprite;
	
	public:
		Renderer();
		~Renderer();
		
		// Vaste shaders
		FXShader* TechInterface;
		FXShader* TechShadowMap;
		FXShader* TechAmbientMap;
		FXShader* TechBrightPass;
		FXShader* TechBrightBlur;
		FXShader* TechToneMap;
		FXShader* TechStock;
		
		// Deze mogen de objecten gebruiken
		std::vector<FXShader*> shaders;
		
		// Pointers naar de automatisch gemaakte buffers
		LPDIRECT3DSURFACE9 backbuffercolor;
		LPDIRECT3DSURFACE9 backbufferdepth;
		
		// Alle opties, worden na een Reset toegepast
		LPDIRECT3D9 d3d; // COM-ding
		D3DADAPTER_IDENTIFIER9 properties; // huidige videokaarteigenschappen
		D3DPRESENT_PARAMETERS presentparameters;
	
		// Handig voor de ui e.d.
		float2 GetUITextExtent(const char* text);
		float2 GetTooltipTextExtent(const char* text);
		float2 GetCenteringCorner(float2 windowsize);

		// pause
		bool paused;
		
		void UpdateVisibility(); // when camera moves or when scene changes, update culling
		void UpdateBufferSize(); // adapt buffer to window
		void UpdateShaders(); // recreate batches
		
		void OnObjectsChange();
		void OnLightsChange();
		void OnCameraChange();
		
		// Klok
		float time;
		int GetTimeMulti();
		void SetTimeMulti(int value);
		int GetTimeMins();
		int GetTimeHours();
		void SetTime(int hours,int mins);
		void AddTime(int hours,int mins);
		
		// Tooltipzooi
		void ShowTooltip(int mode);
		void ToggleTooltip();
		void FlushFrames();
		void ScheduleSaveFrameTimes();
		void SaveFrameTimes();
		
		// Fullscreen
		void SetFullScreen(bool value);
		void ToggleFullScreen();
		
		// Drawing
		void DrawTextLine(const char* text,int left,int top);
		void DrawTextBlock(const char* text,int left,int top,int right,int bottom);
		void DrawTexture(Texture* texture,float left,float top,float right,float bottom);
		void DrawTexture(LPDIRECT3DTEXTURE9 texture,float left,float top,float right,float bottom);
		void DrawTextureFullScreen(LPDIRECT3DTEXTURE9 texture);
		void DrawScene(Scene* scene);
		void DrawModel(Model* thismodel);
		void DrawInterface(Interface* thisinterface);
		void DrawComponent(Component* component);
	
		// Macro's voor passes
		void Begin(bool clear);
		void End();
		void EndScene();
		void Present();
		void BeginTechnique(D3DXHANDLE tech);
		void BeginTechnique(FXShader* shader);
		void EndTechnique();
		
		// Device lost rescue en dergelijke
		void ReleaseResources();
		void OnLostDevice();
		void OnResetDevice();
			
		// Save render targets to HDD
		void ScheduleSaveBuffers();
		void SaveBuffers();
		
		// Misc.
		void GetResolutions(std::vector<D3DDISPLAYMODE>& list);
		float3 GetPixelWorldRay(float2 pixelpos);
		float2 PixelsToProjection(float2 pixelpos);
		float2 ProjectionToPixels(float2 projectionpos);
		
		// Executes before rendering each frame
		void (*OnUpdateTime)(float dt);
};

extern Renderer* renderer;

#endif
