////////////////////////////////////////////////////////////////////////////////
// Filename: shadow.vs
////////////////////////////////////////////////////////////////////////////////


/////////////
// GLOBALS //
/////////////
cbuffer MatrixBuffer
{
	matrix world;
	matrix view;
	matrix projection;
	matrix lightView;
	matrix lightProjection;
};


//////////////////////
// CONSTANT BUFFERS //
//////////////////////
//cbuffer LightBuffer2
//{
//    float3 lightPosition;
//	float padding;
//};


//////////////
// TYPEDEFS //
//////////////
struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
    float4 lightViewPosition : TEXCOORD1;
	//float3 lightPos : TEXCOORD2;
};


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PixelInputType ShadowVertexShader(VertexInputType input)
{
    PixelInputType output;
    
    
	// 적절한 행렬 계산을 위해 위치 벡터를 4 단위로 변경합니다.
    input.position.w = 1.0f;

	// 월드, 뷰 및 투영 행렬에 대한 정점의 위치를 ​​계산합니다.
    output.position = mul(input.position, world);
    output.position = mul(output.position, view);
    output.position = mul(output.position, projection);
    
	// 광원에 의해 보았을 때 vertice의 위치를 ​​계산합니다.
    output.lightViewPosition = mul(input.position, world);
    output.lightViewPosition = mul(output.lightViewPosition, lightView);
    output.lightViewPosition = mul(output.lightViewPosition, lightProjection);


    output.tex = input.tex;
    
    output.normal = mul(input.normal, (float3x3)world);
    output.normal = normalize(output.normal);

	return output;
}