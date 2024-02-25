cbuffer ConstantBuffer
{
  matrix transform;
};

float4 main(float3 pPosition : Position) : SV_Position
{
  return mul(float4(pPosition, 1.0F), transform);
}