Texture2D tex;
SamplerState samplerState;

struct Out
{
  float2 texCoord : TexCoord;
  float4 position : SV_Position;
};

float4 main(Out pOut) : SV_Target
{
  return tex.Sample(samplerState, pOut.texCoord);
  //return float4(1.0F, 0.0F, 0.0F, 1.0);
}