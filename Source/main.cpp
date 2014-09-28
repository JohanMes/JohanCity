#include <windows.h>
#include "JohanEngine\Renderer.h"
#include "JohanEngine\Dialogs.h"
#include "Listeners.h"
#include "JohanCity.h"
#include "resource.h"

void InitGame() {
	// Hide FPS meter?
	renderer->ShowTooltip(3);
	
	// Set time to afternoon
	renderer->SetTimeMulti(0);
	renderer->SetTime(14,0);
	
	// Create menu to interact with JohanCity
	menu = new Menu();
	
	// Add a 10X * 10X m grid to base environment
	game = new JohanCity(128,10.0f);
	
	// Add sunlight
	Dirlight* sun = new Dirlight(
		float3(1,1,1),
		float3(1.4,1.4,1.2),
		true);
	sun->SetCastShadows(true);
	
	// Add some standard keys
	ui->AddKey(new Key(VK_F2,ToggleFullscreen));
	ui->AddKey(new Key(VK_F11,ToggleOptions));
	ui->AddKey(new Key(VK_F12,ToggleConsole));
	
	// Add simple camera movement
	ui->OnMouseMove = MouseMove;
	ui->OnMouseDown = MouseDown;
	ui->OnMouseUp = MouseUp;
	ui->OnMouseWheel = MouseWheel;
	ui->OnKeyDown = KeyDown;
	
	// Update game logic every some period when this fires
	renderer->OnUpdateTime = UpdateTime;
	updateclock = new Clock(true);
}

void DeleteGame() {
	delete updateclock;
	delete game;
	delete menu;
}

// This is where Windows sends user input messages
LRESULT CALLBACK WndProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam) {
	
	// send message to 3D interface
	ui->OnMessage(hwnd,Message,wParam,lParam);
	
	// Perform more handling
	switch(Message) {
		case WM_DESTROY: {
			PostQuitMessage(0); // we are asked to close: kill main thread
			break;
		}
		default: {
			return DefWindowProc(hwnd, Message, wParam, lParam);
		}
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASSEXA wc = {0};
	MSG Msg = {0};

	// Create a window with these properties
	wc.cbSize        = sizeof(WNDCLASSEX);
	wc.lpfnWndProc   = WndProc;
	wc.hInstance     = hInstance;
	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpszClassName = "WindowClass";
	wc.hIcon         = LoadIcon(hInstance,"A"); // laad projecticoon
	wc.hIconSm       = LoadIcon(hInstance,"A");
	
	// Say hi to Windows
	if(!RegisterClassEx(&wc)) {
		MessageBox(NULL, "Window Registration Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Set up a window with 1024x768 usable pixels
	RECT result = {0,0,1024,768};
	AdjustWindowRect(&result,WS_VISIBLE|WS_OVERLAPPEDWINDOW,false);

	// Create a window with a border and 'client rect' of 1024x768
	hwnd = CreateWindow("WindowClass","JohanCity",WS_VISIBLE|WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, // x
		CW_USEDEFAULT, // y
		result.right - result.left, // width
		result.bottom - result.top, // height
		NULL,NULL,hInstance,NULL);
	if(hwnd == NULL) {
		MessageBox(NULL,"Window Creation Failed!","Error!",MB_ICONEXCLAMATION|MB_OK);
		return 0;
	}
	
	// Init render loop
	InitEngine();
	
	InitGame();
	
	// Handle user input. If done, render a frame. Goto 1
	while(Msg.message != WM_QUIT) {
		while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&Msg);
			DispatchMessage(&Msg);
		}
		
		if(!renderer->paused) {
			renderer->Begin(false);			
			renderer->DrawScene(scene);
		//	renderer->DrawTextLine(game->timingreport,5,5);
			renderer->End();
		} else {
			Sleep(100);
		}
	}
	
	DeleteGame();
	
	// Render loop stopped due to Alt+F4 etc? Delete everything
	DeleteEngine();

	return Msg.wParam;
}
