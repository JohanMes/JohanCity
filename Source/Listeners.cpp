#include "JohanEngine\Dialogs.h"
#include "JohanEngine\Renderer.h"
#include "Listeners.h"
#include "resource.h"

void ToggleFullscreen(void* data) {
	renderer->ScheduleSaveBuffers();
//	renderer->ToggleFullScreen();
}
void ToggleOptions(void* data) {
	OptionsDialog->Toggle();
}
void ToggleConsole(void* data) {
	ConsoleDialog->Toggle();
}
void MouseMove(WPARAM wParam,LPARAM lParam,int dx,int dy) {
//	if(!ui->mmousedown) {
//		return; // rotate when scroll wheel is down
//	}
	
	// implement camera rotation using rotation matrix
//	float4x4 rotation;
//	rotation.EulerRotationDeg(float3(2,0,2));
//	
//	// rotate around custom pivot
//	float3 pivot = renderer->camera->pos + 5.0f*renderer->camera->dir;
//	float3 movedcamera = renderer->camera->pos - pivot;

//	// Apply rotation
//	float3 rotatedmovedcamera = movedcamera.Transform(rotation);
//	renderer->camera->SetPos(rotatedmovedcamera + pivot);
//	renderer->camera->SetDir(pivot - renderer->camera->pos);
}
void MouseDown(WPARAM wParam,LPARAM lParam,bool handled) {
	if(handled) {
		return; // ui reacted to click
	}
	
	float3 worldpos = camera->pos;
	float3 worlddir = renderer->GetPixelWorldRay(ui->mousepos).SetLength(100000.0f);
	
	// Check all batches for an intersect
	if(wParam == MK_LBUTTON) { // drag with left button
		begmousedrag = game->RayToGridPos(worldpos,worlddir);
		mousedragging = true;
	} else if(wParam == MK_MBUTTON) {
		// rotate camera
	} else if(wParam == MK_RBUTTON) { // show info with right button
		POINT gridpos = game->RayToGridPos(worldpos,worlddir);
		game->ShowInfo(gridpos);
		mousedragging = false;
	}
}
void MouseUp(WPARAM wParam,LPARAM lParam,bool handled) {
	if(handled) {
		return; // ui reacted to click
	}
	
	if(!mousedragging) {
		return; // fow now, only do dragging here
	}
	
	// Intersect with mouse again
	float3 worldpos = camera->pos;
	float3 worlddir = renderer->GetPixelWorldRay(ui->mousepos).SetLength(100000.0f);
	
	// Click from to...
	POINT endmousedrag = game->RayToGridPos(worldpos,worlddir);
	game->BuildType(begmousedrag,endmousedrag,menu->selection);
}
void MouseWheel(WPARAM wParam,LPARAM lParam) {
	signed short scroll = HIWORD(wParam);
	if(scroll > 0) {
		camera->Move(10*camera->dir);
	} else {
		camera->Move(-10*camera->dir);
	}
}
void KeyDown(WPARAM wParam,LPARAM lParam,bool handled) {
	switch(wParam) {
		
		// Camera movement
		case VK_UP: {
			camera->Move(float3(5,0,5));
			break;
		}
		case VK_LEFT: {
			camera->Move(float3(-5,0,5));
			break;
		}
		case VK_DOWN: {
			camera->Move(float3(-5,0,-5));
			break;
		}
		case VK_RIGHT: {
			camera->Move(float3(5,0,-5));
			break;
		}
		
		// Zoning
		case 'Q': {
			menu->selection = mtResidential;
			break;
		}
		case 'A': {
			menu->selection = mtCommercial;
			break;
		}
		case 'Z': {
			menu->selection = mtIndustrial;
			break;
		}
		
		// Misc
		case 'R': {
			menu->selection = mtRoad;
			break;
		}
		case 'B': {
			menu->selection = mtBulldoze;
			break;
		}
		case 'P': {
			menu->selection = mtPowerPlant;
			break;
		}
		case 'L': {
			menu->selection = mtPowerLine;
			break;
		}
	}
}
void UpdateTime(float dt) {
	float timesincelastupdate = updateclock->GetTimeSec();
	if(timesincelastupdate > 1.0f) { // update every 1 second
	
		game->UpdateTime(timesincelastupdate);
	
		updateclock->Reset();
	}
}
