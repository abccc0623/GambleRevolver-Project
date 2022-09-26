#include "MainHeader.hlsli"
#include "Function.hlsli"


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

void ComputePointLight(Material mat, PointLight L, float3 pos, float3 normal, float3 toEye,
				   out float4 ambient, out float4 diffuse, out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The vector from the surface to the light.
    float3 lightVec = L.Position - pos;
		
	// The distance from surface to light.
    float d = length(lightVec);
	
	// Range test.
    if (d > L.Range)
        return;
		
	// Normalize the light vector.
    lightVec /= d;
	
	// Ambient term.
    ambient = Mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.

    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), Mat.Specular.w);
					
        diffuse = diffuseFactor * Mat.Diffuse * L.Diffuse;
        spec = specFactor*Mat.Specular * L.Specular;
    }

	// Attenuate
    float att = 1.0f / dot(L.Att, float3(1.0f, d, d * d));
    diffuse *= att;
    spec *= att;
}

void ComputeDirectionalLight(Material mat, DirectionalLight L,
                             float3 normal, float3 toEye,
					         out float4 ambient,
						     out float4 diffuse,
						     out float4 spec)
{
	// Initialize outputs.
    ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// The light vector aims opposite the direction the light rays travel.
    float3 lightVec = -L.Direction;

	// Add ambient term.
    ambient = mat.Ambient * L.Ambient;

	// Add diffuse and specular term, provided the surface is in 
	// the line of site of the light.
	
    float diffuseFactor = dot(lightVec, normal);

	// Flatten to avoid dynamic branching.
	[flatten]
    if (diffuseFactor > 0.0f)
    {
        float3 v = reflect(-lightVec, normal);
        float specFactor = pow(max(dot(v, toEye), 0.0f), mat.Specular.w);
					
        diffuse = diffuseFactor * mat.Diffuse * L.Diffuse;
        spec = specFactor * mat.Specular * L.Specular;
    }
}

float4 main(PixelInputType input) : SV_Target
{ 
   
    float4 colors;
    float4 normals;
    float4 position;
    float4 tangent;
    float4 ShadowPos;
    
    float lightIntensity;
    float4 outputColor;
    float4 textureColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    //각종 텍스쳐 샘플링
    colors          = D_COLOR.Sample(SampleType, input.Tex);
    normals         = D_NOMAL.Sample(SampleType, input.Tex);
    position        = D_POS.Sample(SampleType, input.Tex);
    tangent         = D_TANGENT.Sample(SampleType, input.Tex);
    ShadowPos        = D_ShadowPos.Sample(SampleType, input.Tex);
    
    //보는 방향의 벡터를 구함
    //float3 toEye = float3(position.xyz) - camPos;
    float3 toEye = camPos - float3(position.xyz);
    float distToEye = length(toEye);
    toEye /= distToEye;
    
    //모든 라이트 계산을 한다...
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 1.0f);
    
    
    float4 shadow = float4(0.0f, 0.0f, 0.0f,0.0f);
    shadow = CalcShadowFactor(samShadow, D_Shadow, ShadowPos);
    
    float4 A, D, S;
    //디렉션
    ComputeDirectionalLight(Mat, DirLight, normals.xyz, toEye, A, D, S);
    ambient     += A;
    diffuse     += shadow.x *D;
    specular    += shadow.x *S;
    
    
    for (int i = 0; i < PointLightSize; i++)
    {
        //포인트
        ComputePointLight(Mat, point_Light[i], position.xyz, normals.xyz, toEye, A, D, S);
        ambient     += A;
        diffuse     += shadow[0] *D;
        specular    += shadow[0] *S;
    }
    
    
    float4 litColor = float4(1, 1, 1, 1);
    litColor =  colors* (ambient + diffuse) + specular;
    litColor.a += Mat.Diffuse.a * colors.a;
    
    //감마 컬렉션 적용
    litColor = pow(litColor, 1 / 2.2f);
    return litColor;
}