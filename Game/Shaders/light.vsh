cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType
{
	float4 Position : POSITION;
	float2 TexCoords : TEXCOORD0;
	float3 Normal : NORMAL;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float2 TexCoords : TEXCOORD0;
	float3 Normal : NORMAL;
};

PixelInputType LightVertexShader(VertexInputType _Input)
{
	PixelInputType Output;

	_Input.Position.w = 1.0f;
	Output.Position = mul(_Input.Position, WorldMatrix);
	Output.Position = mul(Output.Position, ViewMatrix);
	Output.Position = mul(Output.Position, ProjectionMatrix);

	Output.TexCoords = _Input.TexCoords;

	Output.Normal = mul(_Input.Normal, (float3x3)WorldMatrix);
	Output.Normal = normalize(Output.Normal);

	return Output;
}