
cbuffer ConstantBuffer
{
  matrix rotation;
};

struct VSOut
{
  float4 pos : SV_Position;
  float2 texCoord : TexCoord;
};

VSOut main(float3 pPos : Position, float2 pTexCoord : TexCoord)
{
  VSOut vVsOut = { float4(pPos, 1.0F), pTexCoord};
  //vVsOut.pos = float4(pPos, 1.0F);
  //vVsOut.texCoord = pTexCoord;

  return vVsOut;
}