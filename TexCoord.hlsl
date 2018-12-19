cbuffer ViewProjectionBuffer : register(b0) //0��°(���Թ�ȣ) ����
{
    matrix View;
    matrix Projection;
};

cbuffer WorldBuffer : register(b1) //0��°(���Թ�ȣ) ����
{
    matrix World;
};

struct VertexInput
{
    float4 Position : POSITION0; //�ʷ�-�ø�ƽ(�ν�ǥ)num(�ø�ƽ ��ȣ)
    float2 Uv : TEXCOORD;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; //SV(system value), pixelshader�� pixel�� ����. ���� ������ ��ǥ(system value)�� �ʿ���.
    float2 Uv : TEXCOORD;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World); //���� Local��ǥ�� World��ǥ�� ��ȯ
    output.Position = mul(output.Position, View); //��ȯ�� World�� View�� ��ȯ
    output.Position = mul(output.Position, Projection); //��ȯ�� View�� Projection���� ��ȯ

    output.Uv = input.Uv;
	
    return output;
}

Texture2D DiffuseMap : register(t0); //texture�� �޾ƿ�. (texture 0��°)
Texture2D DiffuseMap2 : register(t1); //texture�� �޾ƿ�. (texture 0��°)

SamplerState DiffuseSamp : register(s0);


float4 PS(PixelInput input) : SV_Target //render target�� pixel��� ��, ��� ������ �˷���� ��(�ø�ƽ(SV_Target))
{//�������� ������ color�� ��ȯ
    float4 color = DiffuseMap.Sample(DiffuseSamp, input.Uv); //������ ������ �׷���(input.uv)

    //clip(color.a - 0.9f); //���İ��� 0.9 ���̸�(���İ��� ������) �ڸ�. ������ �ڸ� �κ��ε� ps�� ���� �ͼ� ����ϴ� ��ŭ ��ȿ������ �κ��� ����
    
    if (input.Uv.x < 1.0f) color = DiffuseMap.Sample(DiffuseSamp, input.Uv);
    if (input.Uv.x > 1.0f) color = DiffuseMap2.Sample(DiffuseSamp, float2(input.Uv.x - 1.0f, input.Uv.y));

    return color;
}

