#pragma once

/// <summary>
/// 엔진 안에있는 최상위 오브젝트
/// </summary>


class Debugging;
///Mesh를 한개만 가지고있는 Mesh
class HsMesh :public GameObject, public Rendering,public Debugging
{
public:
	HsMesh(float x = 0,float y = 0,float z= 0);
	virtual ~HsMesh();

public:
	// 초기화
	virtual void init(hsEngine* ENGINE, LoadData* Data);
	virtual void Update()override;
	virtual void Render(const hsViewData& view) override;
	virtual void Delete()override;
	virtual void Debug()override;

	///텍스쳐 넣어줌
	void SetTextures(ID3D11ShaderResourceView* Textures);
	void SetNomalTextures(ID3D11ShaderResourceView* Textures);
	void SetSubTextures(ID3D11ShaderResourceView* Textures);
	void SetMaterial(Material mt);
	void SetRasterizerState(ID3D11RasterizerState* render);
	bool isParent;


	ID3D11Buffer* Objbuffer;
protected:	

	Material Mat;

	ShaderData m_ShaderData;
	bool isNomal = false;
	bool isTextures = false;
	bool isSubTextures = false;
};