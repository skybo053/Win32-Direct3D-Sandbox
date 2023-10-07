Texture2D tex;
SamplerState samplerState;

float4 main(float2 pTexCoord : TexCoord) : SV_Target
{
  //return float4(1.0F, 0.0F, 0.0F, 1.0F);
  return tex.Sample(samplerState, pTexCoord);
}