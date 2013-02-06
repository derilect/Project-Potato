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
	
	_Input.Position.w = 1.0f;

	Output.Position = mul(_Input.Position, WorldMatrix);
	Output.Position = mul(Output.Position, ViewMatrix);
	Output.Position = mul(Output.Position, ProjectionMatrix);

	Output.Color = _Input.Color;

	return Output;
}

