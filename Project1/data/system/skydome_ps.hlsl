////////////////////////////////////////////////////////////////////////////////
// Filename: skydome_ps.hlsl
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer GradientBuffer
{
	float4 topColor;
	float4 centerColor;
	float4 bottomColor;
};


//////////////
// TYPEDEFS //
//////////////
struct PixelInputType
{
    float4 position : SV_POSITION;
	float4 domePosition : TEXCOORD0;
};


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 SkyDomePixelShader(PixelInputType input) : SV_TARGET
{
	// 이 픽셀이 있는 스카이 돔에서 위치를 결정합니다.
	float height = input.domePosition.y;
	float4 outputColor;

	if(height > 0.0)outputColor = lerp(centerColor, topColor, height);
	else {
		outputColor = lerp(centerColor, bottomColor, abs(height * 10));
		if (height < -0.112f) outputColor = lerp(bottomColor, centerColor, height);
	}

	//else outputColor = bottomColor;

    return outputColor;
}