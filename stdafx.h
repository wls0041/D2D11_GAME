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

//Direct Write
#include <d2d1_1.h>
#include <dwrite.h>

#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

//DirectX
#include <dxgi1_2.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <d3d11shader.h>
#include <d3d11.h>
#include <D3DX11async.h>

#pragma comment(lib, "dxgi.lib")  //lib.동적 라이브러리, dll.정적 라이브러리
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "d3dx10.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

//FMOD(사운드 라이브러리)
#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#pragma comment(lib, "fmod_vc.lib")

typedef unsigned int uint;

//macro function
#define SAFE_RELEASE(p)      { if (p) { (p)->Release(); (p) = nullptr; }}
#define SAFE_DELETE(p)       { if (p) { delete (p); (p) = nullptr; }}
#define SAFE_DELETE_ARRAY(p) { if (p) { delete[] (p); (p) = nullptr; }}

//Math
#include "./Framework/Math/Math.h"
#include "./Framework/Math/Vector2.h"
#include "./Framework/Math/Vector3.h"
#include "./Framework/Math/Matrix.h"
#include "./Framework/Math/Color.h"
#include "./Framework/Math/BoundBox.h"

//MainSystem
#include "./Framework/Core/Settings.h"
#include "./Framework/Core/Context.h"

//BasicData
#include "./Framework/Core/D3D11/BasicData/Config.h"
#include "./Framework/Core/D3D11/BasicData/Vertex.h"
#include "./Framework/Core/D3D11/BasicData/Geometry.h"

//Utility
#include "./Framework/Utility/GeometryUtility.h"
#include "./Framework/Utility/FileSystem.h"
#include "./Framework/Utility/Xml.h"

//SubSystem
#include "./Framework/Core/Subsystem/Timer.h"
#include "./Framework/Core/Subsystem/Input.h"
#include "./Framework/Core/Subsystem/Audio.h"
#include "./Framework/Core/Subsystem/Graphics.h"
#include "./Framework/Core/Subsystem/DirectWrite.h"
#include "./Framework/Core/Subsystem/ResourceManager.h"
#include "./Framework/Core/Subsystem/SceneManager.h"

//Wrapper Class
#include "./Framework/Core/D3D11/IAStage/VertexBuffer.h"
#include "./Framework/Core/D3D11/IAStage/IndexBuffer.h"
#include "./Framework/Core/D3D11/IAStage/InputLayout.h"
#include "./Framework/Core/D3D11/ShaderStage/VertexShader.h"
#include "./Framework/Core/D3D11/ShaderStage/PixelShader.h"
#include "./Framework/Core/D3D11/ShaderStage/ConstantBuffer.h"

//Resource
#include "./Framework/Resource/Texture.h"
#include "./Framework/Resource/Animation.h"
#include "./Framework/Resource/AudioClip.h"
