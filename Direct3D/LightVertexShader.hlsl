cbuffer ConstantBuffer
{
  matrix model;
  matrix view;
  matrix projection;
};

float4 main(float3 pPosition : Position) : SV_Position
{
  matrix m1 = mul(model, view);
  matrix m2 = mul(m1, projection);
  
  return mul(float4(pPosition, 1.0F), m2);
}