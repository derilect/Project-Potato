cbuffer MatrixBuffer
{
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType
{
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PixelInputType
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType _Input)
{
	PixelInputType Output;
	
	Output.position = mul(_Input.position, WorldMatrix);
	Output.position = mul(_Input.position, ViewMatrix);
	Output.position = mul(_Input.position, ProjectionMatrix);

	Output.color = _Input.color;

	return Output;
}

