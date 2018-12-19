cbuffer ViewProjectionBuffer : register(b0) //0번째(슬롯번호) 버퍼
{
    matrix View;
    matrix Projection;
};

cbuffer WorldBuffer : register(b1) //0번째(슬롯번호) 버퍼
{
    matrix World;
};

struct VertexInput
{
    float4 Position : POSITION0; //초록-시멘틱(인식표)num(시멘틱 번호)
    float2 Uv : TEXCOORD;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; //SV(system value), pixelshader는 pixel을 찍음. 따라서 공간상 좌표(system value)가 필요함.
    float2 Uv : TEXCOORD;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World); //들어온 Local좌표를 World좌표로 변환
    output.Position = mul(output.Position, View); //변환된 World를 View로 변환
    output.Position = mul(output.Position, Projection); //변환된 View에 Projection으로 변환

    output.Uv = input.Uv;
	
    return output;
}

Texture2D DiffuseMap : register(t0); //texture를 받아옴. (texture 0번째)
Texture2D DiffuseMap2 : register(t1); //texture를 받아옴. (texture 0번째)

SamplerState DiffuseSamp : register(s0);


float4 PS(PixelInput input) : SV_Target //render target에 pixel찍는 놈, 어디에 찍을지 알려줘야 함(시멘틱(SV_Target))
{//정점에서 들어오는 color를 반환
    float4 color = DiffuseMap.Sample(DiffuseSamp, input.Uv); //어디부터 어디까지 그려라(input.uv)

    //clip(color.a - 0.9f); //알파값이 0.9 밑이면(알파값이 없으면) 자름. 어차피 자를 부분인데 ps에 끌고 와서 계산하는 만큼 비효율적인 부분이 있음
    
    if (input.Uv.x < 1.0f) color = DiffuseMap.Sample(DiffuseSamp, input.Uv);
    if (input.Uv.x > 1.0f) color = DiffuseMap2.Sample(DiffuseSamp, float2(input.Uv.x - 1.0f, input.Uv.y));

    return color;
}

