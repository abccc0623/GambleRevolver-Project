//***************************************************************************************
// Effects.h by Frank Luna (C) 2011 All Rights Reserved.
//
// Defines lightweight effect wrappers to group an effect and its variables.
// Also defines a static Effects class from which we can access all of our effects.
//***************************************************************************************

#ifndef EFFECTS_H
#define EFFECTS_H


#include "EnginePCH.h"
#pragma region Effect

class BasicEffect;
class SkyEffect;
class SkinningEffect;
class ColorEffect;
class PostEffect;

class Effect
{
public:
	Effect(ID3D11Device* device, const std::wstring& filename);
	virtual ~Effect();

private:
	Effect(const Effect& rhs);
	Effect& operator=(const Effect& rhs);

protected:
	ID3DX11Effect* mFX;
};
#pragma endregion

#pragma region BasicEffect


#pragma endregion

#pragma region Effects
class Effects
{
public:
	static void InitAll(ID3D11Device* device);
	static void DestroyAll();

	static BasicEffect*		BasicFX;
	static SkinningEffect*	SkinningFX;
	static SkyEffect*		SkyFX;
	static ColorEffect*		ColorFX;
	static PostEffect*		PostFX;
};


#pragma endregion

#endif // EFFECTS_H