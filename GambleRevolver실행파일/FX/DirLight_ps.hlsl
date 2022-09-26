#include "MainHeader.hlsli"

struct PixelInputType
{
    float4 posH : SV_POSITION;
    float3 PosL : POSITION;
    float2 Tex  : TEXCOORD0;
};
struct PixelOutputType
{
    float4 FINAL : SV_Target0;
};

float4 main(PixelInputType input) : SV_TARGET
{  
    float4 colors;
    float4 normals;
    float4 position;
    float4 Light = float4(0, 0, 0, 0);
    float4 shadow;
    
    float3 lightDir = float3(0, 0, 1);
    float lightIntensity;
    float4 outputColor;
    float4 textureColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    //���� �ؽ��� ���ø�
   // colors      = D_COLOR.Sample(SampleType, input.Tex);
   // normals     = D_NOMAL.Sample(SampleType, input.Tex);
   // position    = D_POS.Sample(SampleType, input.Tex);
    //Light       = D_LIGHT.Sample(SampleType, input.Tex);
    //Light       = D_LIGHT.Sample(SampleType, input.Tex);
    //shadow      = D_Shadow.Sample(SampleType, input.Tex);
           
    
    //���� ������ ���͸� ����
   // float3 toEye = camPos - float3(position.xyz);
   // float distToEye = length(toEye);
   // toEye /= distToEye;
   // 
   //
   // //�𷺼� ����Ʈ ���
   // float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
   // float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
   // float4 spec = float4(0.0f, 0.0f, 0.0f, 0.0f);
   // 
   // 
   // // The light vector aims opposite the direction the light rays travel.
   // float3 lightVec = -DirLight.Direction;
   //
	//    // Add ambient term.
   // ambient = Mat.Ambient * DirLight.Ambient;
   //
   // lightIntensity = dot(lightVec, normals.xyz);
   //
	//    // Flatten to avoid dynamic branching.
   // float3 v = reflect(-lightVec, normals.xyz);
   // float specFactor = pow(max(dot(v, toEye), 0.0f), Mat.Specular.w);
	//				
   // diffuse = lightIntensity * Mat.Diffuse * DirLight.Diffuse;
   // spec = specFactor * Mat.Specular * DirLight.Specular;
   //
	//    // ������ ���� �� �ȼ��� ������ ������� ���� ���� Ȯ�� ���� �����մϴ�.
   // Light = lightIntensity;                
    //Light += lightIntensity;                
    //Light += lightIntensity;
   
    //output.color    = float4(0, 0, 0, 1.0f);
    //output.normal   = float4(0, 0, 0, 1.0f);
    //output.Pos      = float4(0, 0, 0, 1.0f);
    //output.tangent  = float4(0, 0, 0, 1.0f);
   
    return Light.xxxw;
}