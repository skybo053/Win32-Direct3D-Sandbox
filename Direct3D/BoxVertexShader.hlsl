
cbuffer ConstantBuffer
{
  matrix rotation;
};

struct VSOut
{
  float2 texCoord : TexCoord;
  float4 position : SV_Position;
};


VSOut main(float3 pPos : Position, float2 pTexCoord : TexCoord)
{
  VSOut vsOut;
  vsOut.position = mul(float4(pPos, 1.0F), rotation);
  vsOut.texCoord = pTexCoord;

  return vsOut;

  //return float4(pPos, 1.0F);
  //return mul(rotation, float4(pPos, 1.0F));
}