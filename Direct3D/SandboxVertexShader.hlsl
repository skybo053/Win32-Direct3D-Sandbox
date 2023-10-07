struct VSOut
{
  float4 col : Color;
  float4 pos : SV_Position;
};

cbuffer ConstantBuffer
{
  matrix scaleMatrix;
};


VSOut main(float3 pPos : Position, float4 pColor : Color)
{
  VSOut vOut;
  
  vOut.pos = mul(scaleMatrix, float4(pPos, 1.0F));
  vOut.col = pColor;

  return vOut;
}