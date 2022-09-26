#pragma once
/// <summary>
/// ������Ʈ�� ������� �׸��� ���� ��ü Rendering Ŭ������ ����ϴ�
/// </summary>
class Debugging
{
public:
	Debugging(float x =0,float y = 0,float z = 0);
	~Debugging();

	
	void init(hsEngine* _ENGINE);
	void Update(XMMATRIX World);

	///����밴ü�� ��ӹ޴� init �κп� �־���� ��

	//�� �������� ����� ���ý��� ����
	void Create_2D_Y_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_2D_X_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_2D_Z_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});

	//�簢���������� �������ý� ����
	void Create_2D_X_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });
	void Create_2D_Y_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });
	void Create_2D_Z_Rect_Buffer(XMFLOAT3 _Centor, XMFLOAT2 _Extents, XMFLOAT4 Color = { 1,0,0 ,1 });


	//�ڽ� �������� ����� ���ý��� ������
	void Create_Box_Buffers(XMFLOAT3 _Centor, XMFLOAT3 _Extents, XMFLOAT4 Color = { 1,0,0 ,1});
	void Create_Circle_Buffers(XMFLOAT3 _Centor,float Radius, XMFLOAT4 Color = { 1,0,0 ,1});


	//���� �������� ����� ���ý��� ������
	void Create_Line_Buffers(XMFLOAT3 start,XMFLOAT3 end, XMFLOAT4 Color = { 1,0,0 ,1 });

	///�� �ɼ����� ������� ���ý���� ���۸� ���� 
	void CreateDebugBuffers();


	//����� ���� ���̴��� �Ѱ���
	virtual void SetDebugShader(ShaderData Data);
private:
	std::vector<Debug32>	vertices;
	std::vector<UINT>		indices;


	ID3D11Buffer* mVB;	//���ؽ� ����
	ID3D11Buffer* mIB;	//�ε��� ����

	ShaderData m_ShaderData;
	ID3D11Buffer* DebugObjbuffer;

	hsEngine*				m_ENGINE;		// ����
	ID3D11Device*			m_Device;		// D3D11 ����̽�
	ID3D11DeviceContext*	m_Context;		// ����̽� ���ؽ�Ʈ


	int vcount;
	int icount;
};