
cbuffer ConstantBuffer
{
  matrix rotation;
};


struct Out
{
  float2 texCoord : TexCoord;
  float4 position : SV_Position;
};


Out main(float3 pPos : Position, float2 pTexCoord : TexCoord)
{
  Out o;
  o.texCoord = pTexCoord;
  o.position = mul(float4(pPos, 1.0F), rotation);


  return o;

  //return mul(float4(pPos, 1.0F), rotation);
}