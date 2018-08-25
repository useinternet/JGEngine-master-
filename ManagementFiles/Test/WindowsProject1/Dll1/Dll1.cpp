// Dll1.cpp: DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//

#include "stdafx.h"
#include "Dll1.h"
 void InitD3D(HWND hWnd, int Width, int Height) 
 { 
	 // create a struct to hold information about the swap chain 
	 DXGI_SWAP_CHAIN_DESC scd; 
	 // clear out the struct for use 
	 ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));
	 HRESULT hr = S_OK; 
	 // fill the swap chain description struct
	 scd.BufferDesc.Width = Width;
	 scd.BufferDesc.Height = Height;
	 scd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	 scd.BufferDesc.RefreshRate.Numerator = 0;
	 scd.BufferDesc.RefreshRate.Denominator = 1;
	 scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	 scd.OutputWindow = hWnd;
	 scd.SampleDesc.Count = 1;
	 scd.SampleDesc.Quality = 0;
     scd.Windowed = true;
	 scd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	 scd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	 scd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	 scd.Flags = 0;
	 scd.BufferCount = 2; 
	 // one back buffer 
	/* scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	 // use 32-bit color 
	 scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	 // how swap chain is to be used 
	 scd.OutputWindow = hWnd; 
	 // the window to be used 
	 scd.SampleDesc.Count = 1; 
	 // how many multisamples 
	 scd.SampleDesc.Quality = 0; 
	 scd.Windowed = TRUE; 
	 scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; */
	 
	 // allow full-screen switching
	 // windowed/full-screen mode 
	 // create a device, device context and swap chain using the information in the scd struct
	 hr = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &scd, &swapchain, &dev, NULL, &devcon); 
	 // get the address of the back buffer 
	 ID3D11Texture2D *pBackBuffer; 
	 swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer); 
	 // use the back buffer address to create the render target 
	 dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
	 pBackBuffer->Release(); 
	 // set the render target as the back buffer
	 devcon->OMSetRenderTargets(1, &backbuffer, NULL); 
	 // Set the viewport
	 D3D11_VIEWPORT viewport; 
	 ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
	 viewport.TopLeftX = 0;
	 viewport.TopLeftY = 0; 
	 viewport.Width = Width; 
	 viewport.Height = Width; 
	 devcon->RSSetViewports(1, &viewport); 
 } 
 
 void RenderFrame(void)
 { 
	 float color[4] = { 0.0f, 1.0f, 1.0f, 1.0f };
	 // clear the back buffer to a deep blue 
	 devcon->ClearRenderTargetView(backbuffer, color);
	 // do 3D rendering on the back buffer here 
	 // switch the back buffer and the front buffer
	 swapchain->Present(0, 0); 
 }
 void CleanD3D(void) 
 { 
	 // close and release all existing COM objects 
	 swapchain->Release(); 
	 backbuffer->Release(); 
	 dev->Release(); 
	 devcon->Release();
 }