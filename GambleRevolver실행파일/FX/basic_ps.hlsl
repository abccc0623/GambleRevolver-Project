#include "MainHeader.hlsli"
#include "Function.hlsli"

struct PixelInputType
{
    float4 posH		    : SV_POSITION;
    float3 NormalW	    : NORMAL;
    float3 TangentW     : TANGENT;
    float2 Tex          : TEXCOORD;
    float4 ShadowPosH   : TEXCOORD1;
};

//�𷺼� ����Ʈ
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

float4 main(PixelInputType input) : SV_TARGET
{
  
    //������ �������Ͱ� �ƴ� �� �����Ƿ� �ٽ� ����ȭ �Ѵ�.
    input.NormalW = normalize(input.NormalW);
    
    //toEye ���ͷ� ���� ���
    float3 toEye = camPos - input.posH.xyz;
    
    ////������ �� ǥ�� �� ������ �Ÿ��� ����
    float distToEye = length(toEye);
    //
    toEye /= distToEye;
    
    float4 textureColor = { 1.0f, 1.0f, 1.0f, 1.0f};
    float4 TexColor = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    float4 Tex01 = { 1.0f, 1.0f, 1.0f, 1.0f };
    float4 Tex02 = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    //�����ؽ��İ� �ִ��� Ȯ�ο���
    if (Texture_Mask & TEXTURE_MASK_MAIN )
    {
        Tex01 = MainTexture.Sample(SampleType, input.Tex);
        Tex01.rgb = pow(Tex01.rgb, 2.2f); //�����ݷ���..
        TexColor = Tex01;
    }
    
    //���� �ؽ��İ� �ִ��� Ȯ�ο���
    if (Texture_Mask & TEXTURE_MASK_SUB)
    {
        float4 Tex02 = SubTexture.Sample(SampleType, input.Tex);
        //Tex02.rgb = pow(Tex02.rgb, 2.2f); //�����ݷ���..
        TexColor = Tex01 * Tex02;
    }
    
    //�븻 �ؽ��İ� �ִ��� 
    float3 bumpedNormalW = 1;
    if (Texture_Mask & TEXTURE_MASK_NOMAL)
    {
        float4 Tex03 = NomalTexture.Sample(SampleType, input.Tex);
        bumpedNormalW = NormalSampleToWorldSpace(Tex03.rgb, input.NormalW, input.TangentW);
    }
   

    float3 shadow = float3(0.0f, 0.0f, 0.0f);
    shadow = CalcShadowFactor(samShadow, D_Shadow, input.ShadowPosH);
    
    textureColor = TexColor;
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
    
    float4 A, D, S;
    ComputeDirectionalLight(Mat, DirLight, bumpedNormalW, toEye, A, D, S);
    ambient +=A;
    diffuse += shadow[0]*D;
    specular += shadow[0]*S;
    
    
    textureColor = TexColor * (ambient + diffuse) + specular;
    textureColor.a += Mat.Diffuse.a * TexColor.a;
    
    textureColor =  pow(textureColor,1/ 2.2f);
    return textureColor;
}

