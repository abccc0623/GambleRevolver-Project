#pragma once
/// <summary>
/// 오브젝트의 디버깅을 그리기 위한 객체 Rendering 클래스와 비슷하다
/// </summary>
class Debugging
{
public:
	Debugging(float x =0,float y = 0,float z = 0);
	~Debugging();

	
	void init(hsEngine* _ENGINE);
	void Update(XMMATRIX World);

	///디버깅객체를 상속받는 init 부분에 넣어줘야 함

	//원 모형으로 디버깅 버택스를 생성
	void Create_2D_Y_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_2D_X_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_2D_Z_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});

	//사각형모형으로 디버깅버택스 생성
	void Create_2D_X_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });
	void Create_2D_Y_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });
	void Create_2D_Z_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });


	//박스 모형으로 디버깅 버택스를 생성함
	void Create_Box_Buffers(XMFLOAT3 _Centor, XMFLOAT3 _Extents, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});


	//라인 모형으로 디버깅 버택스를 생성함
	void Create_Line_Buffers(XMFLOAT3 start,XMFLOAT3 end, XMFLOAT4 Color = { 1,0,0 ,1 });

	///위 옵션으로 만들어진 버택스들로 버퍼를 생성 
	void CreateDebugBuffers();


	//디버깅 전용 쉐이더를 넘겨줌
	virtual void SetDebugShader(ShaderData Data);
private:
	std::vector<Debug32>	vertices;
	std::vector<UINT>		indices;


	ID3D11Buffer* mVB;	//버텍스 버퍼
	ID3D11Buffer* mIB;	//인덱스 버퍼

	ShaderData m_ShaderData;
	ID3D11Buffer* DebugObjbuffer;

	hsEngine*				m_ENGINE;		// 엔진
	ID3D11Device*			m_Device;		// D3D11 디바이스
	ID3D11DeviceContext*	m_Context;		// 디바이스 컨텍스트


	int vcount;
	int icount;
};