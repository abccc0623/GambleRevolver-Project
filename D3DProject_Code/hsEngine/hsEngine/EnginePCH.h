// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됩니다.
// 여기에 자주 업데이트할 파일을 추가하지 마세요. 그러면 성능이 저하됩니다.

#ifndef ENGINE_H
#define ENGINE_H


// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"


#include "DXTKFont.h"
#include "SpriteFont.h"
#include "SpriteBatch.h"
#include "SimpleMath.h"

#ifdef _DEBUG
#pragma comment( lib, "../../lib/Effects11d.lib" )
#else
#pragma comment( lib, "../../lib/Effects11.lib" )
#endif

#ifdef _DEBUG
#pragma comment(lib, "../../lib/DirectXTexd.lib")
#else
#pragma comment(lib, "../../lib/DirectXTex.lib")
#endif


#ifdef _DEBUG
#pragma comment( lib, "../../lib/DirectXTK.lib" )
#else
#pragma comment( lib, "../../lib/DirectXTKr.lib" )
#endif

//#pragma comment( lib, "libfbxsdk" )
#pragma comment( lib, "libfbxsdk-md") 
#pragma comment( lib, "libxml2-md") 
#pragma comment( lib, "zlib-md") 


#include "fbxsdk.h"



#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dinput8")
#include <DirectXMath.h>
#include "d3dx11effect.h"
#pragma comment(lib,"d3dcompiler.lib") //쉐이더 코드를 불러오는데 필요함

#define  TEXTURE_TYPE_MAIN 0
#define  TEXTURE_TYPE_SUB  1
#define  TEXTURE_TYPE_NOMAL 2

//카메라 관련
#include "DRCamera.h"
#include "IDRCamera.h"
//각종 헬퍼 관련 소스

#include "LightHelper.h"
#include "MathHelper.h"

//타이머
#include "GameTimer.h"
//키인풋
#include "dinput.h"
#include "hsKey.h"
#include "hsD3DKeyinput.h"


using namespace DirectX;
using namespace SimpleMath;
using namespace std;

#include "atlstr.h"
#include "Animation.h"
#include "CParsingDataClass.h";
#include "CASEParser.h"
#include "Lexer.h"
#include "ShaderData.h"
#include "BufferData.h"
#include "LayoutData.h"
#include "DeviceData.h"

//FBX
#include "fbxsdk.h"
#include "FBXParser.h"

//기술
#include "Transform.h"
#include "Collider.h"
#include "Rendering.h"
#include "MeshAnimation.h"
#include "Navigation2D.h"


//임펙트
#include "DDSTextureLoader.h"
#include "Effects.h"
#include "Basic.h"
#include "SkinningFX.h"
#include "SkyEffect.h"
#include "ColorEffect.h"
#include "Post.h"
#include "TextureShader.h"


#include <string>
#include "DX11Define.h"
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <fstream>
#include <vector>
#include <map>
#include <WindowsX.h>
#include <sstream>
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include "hsVector3.h"
#include <DirectXPackedVector.h>
#include <WindowsX.h>

//오브젝트

#include "hsEnum.h"
#include "Debugging.h"
#include "ResoureceManager.h"
#include "GameObject.h"
#include "UI.h"
#include "hsBone.h"
#include "SkinningMesh.h"
#include "Grid.h"
#include "hsMesh.h"
#include "hsMeshList.h"
#include "hsCamera.h"
#include "Box.h"
#include "LightManager.h"
#include "LightObject.h"
#include "DirLight.h"
#include "TextureObj.h"
#include "MultiTextureMap.h"
#include "TextureMap.h"
#include "SkyBox.h"
#include "Image.h"
#include "Slider.h"
#include "Button.h"
#include "Particle.h"



//각종 매니저들
#include "ParticleSystem.h"
#include "ShaderManager.h"
#include "Factory.h"
#include "Screen.h"
#include "ScreenManager.h"
#include "RenderManager.h"
#include "AlphaManager.h"
#include "CamManager.h"
#include "Canvas.h"

//struct 모음집
//#define _WIN32_WINNT 0x600
#include <stdio.h>

#include <d3dcompiler.h>
#include "hsEngine.h"


#endif //PCH_H
