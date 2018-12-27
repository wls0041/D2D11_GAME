#pragma once

#include <Windows.h>
#include <assert.h>
#include <time.h>

//STL
#include <vector>
#include <map>
#include <functional>
#include <chrono>
using namespace std;

//DirectX
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX10math.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")  //lib.동적 라이브러리, dll.정적 라이브러리
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

typedef unsigned int uint;

//macro function
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; }}
#define SAFE_DELETE(p)       { if (p) { delete (p); (p) = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p) = nullptr; }}


//MainSystem
#include "./Framework/Core/Settings.h"
#include "./Framework/Core/Context.h"

//SubSystem
#include "./Framework/Core/Subsystem/Graphics.h"
#include "./Framework/Core/Subsystem/Input.h"

//Wrapper Class
#include "./Rendering/Vertex.h"
#include "./Framework/Core/D3D11/IAStage/VertexBuffer.h"
#include "./Framework/Core/D3D11/IAStage/IndexBuffer.h"

//Utility
#include "./Utility/Geometry.h"
#include "./Utility/GeometryUtility.h"