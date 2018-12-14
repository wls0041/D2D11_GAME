cbuffer WorldViewProjectionBuffer : register(b0) //0번째(슬롯번호) 버퍼
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VertexInput
{
    float4 Position : POSITION0; //초록-시멘틱(인식표)num(시멘틱 번호)
    float4 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; //SV(system value), pixelshader는 pixel을 찍음. 따라서 공간상 좌표(system value)가 필요함.
    float4 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World); //들어온 Local좌표를 World좌표로 변환
    output.Position = mul(output.Position, View); //변환된 World를 View로 변환
    output.Position = mul(output.Position, Projection); //변환된 View에 Projection으로 변환

    output.Color = input.Color;
	
    return output;
}

float4 PS(PixelInput input) : SV_Target //render target에 pixel찍는 놈, 어디에 찍을지 알려줘야 함(시멘틱(SV_Target))
{//정점에서 들어오는 color를 반환
    return input.Color;
}

