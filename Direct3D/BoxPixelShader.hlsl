Texture2D tex2D;
SamplerState samplerState;


float4 main(float2 pTexCoord : TexCoord) : SV_Target
{
  //return tex2D.Sample(samplerState, pTexCoord);
  return float4(1.0F, 0.0F, 0.0F, 1.0);
}