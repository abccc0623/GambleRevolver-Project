#pragma once
/// <summary>
/// 쉐이더를 관리할 쉐이더 매니저
/// D3D튜토리얼을 참고하여 만들었음
/// 버택스 쉐이더 픽셀쉐이더 샘플러 인풋레이아웃을 각각 따로 만들어서 합치는방식
/// </summary>

class  ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void init(hsEngine* ENGINE);
	void Delete();


	//쉐이더 로드
	bool LoadShader(string vsFileName,string psFileName, string ShaderName);

	//레이아웃을 만들어준다

	//샘플러를 만들어준다
	bool CreateSamPler(string shaderName);

	//쉐이더를 불러올 경로를 미리 저장한다
	void SetShaderFilePath(string path);



	static ID3D11ClassInstance** g_DynamicLinkageArray;
	ShaderData GetShader(string ShaderName);
	ShaderData GetContextBufferList();
private:
	//로드 버택스 쉐이더
	bool LoadVertexShader(string vsFileName, string ShaderName);
	//로드 픽셀 쉐이더
	bool LoadPixelShader(string psFileName, string ShaderName);




	/// <summary>
	/// 인풋레이아웃을 만든다
	/// </summary>
	//쉐이더에서 인풋레이아웃 정보를 읽어온다
	void CreateInputLayout(string shaderName,ID3D10Blob* vertexBuffer);

	//레이아웃을 만들때 필요한 포멧정보를 찾는다
	DXGI_FORMAT FindFormat(int number, BYTE size, D3D11_SIGNATURE_PARAMETER_DESC desc);



	/// <summary>
	/// 상수버퍼를 만든다
	/// </summary>
	void CreateConstantBuffer();

private:
	//엔진 정보들
	hsEngine*				ENGINE;		// 엔진
	ID3D11Device*			Device;		// D3D11 디바이스
	ID3D11DeviceContext*	mContext;	// 디바이스 컨텍스트

	//불러올 쉐이더 파일의 경로
	string ShaderFilePath;
	///버택스,픽셀,샘플러,레이아웃을 같은이름의 map으로 묶어서 쉐이더를 조립함
	
	//리플렉션 객체
	ID3D11ShaderReflection* ReFlector;

	//셈플리스트
	map<string, ID3D11SamplerState*> samplerList;

	//상수 버퍼 리스트
	map<string, ID3D11Buffer*> CbufferList;

	
	map<string, ShaderData> ShaderList;
};

