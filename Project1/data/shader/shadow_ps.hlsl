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

	// 부동 소수점 정밀도 문제를 해결할 바이어스 값을 설정합니다.
	float bias = 0.001f;
	float4 color = ambientColor;

	//// 투영 된 텍스처 좌표를 계산합니다.
	float2 projectTexCoord = float2(0.0f, 0.0f);
	//projectTexCoord.x =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	//projectTexCoord.y =  input.lightViewPosition.x / input.lightViewPosition.w / 2.0f + 0.5f;
	projectTexCoord.x = input.lightViewPosition.x / input.position.w / 2.0f + 0.5f;
	projectTexCoord.y = input.lightViewPosition.y / input.position.w / 2.0f + 0.5f;

	// 투영 된 좌표가 0에서 1 범위에 있는지 결정합니다. 그렇다면이 픽셀은 빛의 관점에 있습니다.

	float lightIntensity; 
	if((saturate(projectTexCoord.x) == projectTexCoord.x) && (saturate(projectTexCoord.y) == projectTexCoord.y))
	{
		// 투영 된 텍스처 좌표 위치에서 샘플러를 사용하여 깊이 텍스처에서 섀도우 맵 깊이 값을 샘플링합니다.
		float depthValue = depthMapTexture.Sample(SampleTypeClamp, projectTexCoord).r;

		// 빛의 깊이를 계산합니다.
		float lightDepthValue = input.lightViewPosition.z / input.lightViewPosition.w;
		lightDepthValue = lightDepthValue - bias;

		// 섀도우 맵 값의 깊이와 빛의 깊이를 비교하여이 픽셀을 음영 처리할지 조명할지 결정합니다.
		// 빛이 객체 앞에 있으면 픽셀을 비추고, 그렇지 않으면 객체 (오클 루더)가 그림자를 드리 우기 때문에이 픽셀을 그림자로 그립니다.
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