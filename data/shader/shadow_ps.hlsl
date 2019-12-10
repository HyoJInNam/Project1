////////////////////////////////////////////////////////////////////////////////
// Filename: shadow.ps
////////////////////////////////////////////////////////////////////////////////


//////////////
// TEXTURES //
//////////////
Texture2D shaderTexture : register(t0);
Texture2D depthMapTexture : register(t1);


///////////////////
// SAMPLE STATES //
///////////////////
SamplerState SampleTypeClamp : register(s0);
SamplerState SampleTypeWrap  : register(s1);


//////////////////////
// CONSTANT BUFFERS //
//////////////////////
cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
	//float3 lightPos : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 ShadowPixelShader(PixelInputType input) : SV_TARGET
{

	float3 lightDir = -lightDirection;

	// �ε� �Ҽ��� ���е� ������ �ذ��� ���̾ ���� �����մϴ�.
	float bias = 0.001f;
	float4 color = ambientColor;

	//// ���� �� �ؽ�ó ��ǥ�� ����մϴ�.
	float2 projectTexCoord = float2(0.0f, 0.0f);
	//projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	//projectTexCoord.y =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.x = input.lightViewPosition.x / input.position.w / 2.0f + 0.5f;
	projectTexCoord.y = input.lightViewPosition.y / input.position.w / 2.0f + 0.5f;

	// ���� �� ��ǥ�� 0���� 1 ������ �ִ��� �����մϴ�. �׷��ٸ��� �ȼ��� ���� ������ �ֽ��ϴ�.

	float lightIntensity; 
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// ���� �� �ؽ�ó ��ǥ ��ġ���� ���÷��� ����Ͽ� ���� �ؽ�ó���� ������ �� ���� ���� ���ø��մϴ�.
		float depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// ���� ���̸� ����մϴ�.
		float lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		// ������ �� ���� ���̿� ���� ���̸� ���Ͽ��� �ȼ��� ���� ó������ �������� �����մϴ�.
		// ���� ��ü �տ� ������ �ȼ��� ���߰�, �׷��� ������ ��ü (��Ŭ ���)�� �׸��ڸ� �帮 ��� �������� �ȼ��� �׸��ڷ� �׸��ϴ�.
		if(lightDepthValue < depthValue)
		{
			lightIntensity = saturate(dot(input.normal, lightDir));

		    if(lightIntensity > 0.0f)
			{
				color += (diffuseColor * lightIntensity);
				color = saturate(color);
				//color = float4(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}
	else
	{
		lightIntensity = saturate(dot(input.normal, lightDir));
		if (lightIntensity > 0.0f)
		{
			color += (diffuseColor * lightIntensity);
			color = saturate(color);
		}
	}


	float4 textureColor = shaderTexture.Sample(SampleTypeWrap, input.tex);
	color = color * textureColor;

    return color;
}