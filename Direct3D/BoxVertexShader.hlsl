
cbuffer ConstantBuffer
{
  matrix model;
  matrix view;
  matrix projection;
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
  
  matrix m1 = mul(model, view);
  matrix m2 = mul(m1, projection);
  
  o.position = mul(float4(pPos, 1.0F), m2);


  return o;

  //return mul(float4(pPos, 1.0F), rotation);
}