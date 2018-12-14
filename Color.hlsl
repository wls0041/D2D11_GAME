cbuffer WorldViewProjectionBuffer : register(b0) //0��°(���Թ�ȣ) ����
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VertexInput
{
    float4 Position : POSITION0; //�ʷ�-�ø�ƽ(�ν�ǥ)num(�ø�ƽ ��ȣ)
    float4 Color : COLOR0;
};

struct PixelInput
{
    float4 Position : SV_POSITION0; //SV(system value), pixelshader�� pixel�� ����. ���� ������ ��ǥ(system value)�� �ʿ���.
    float4 Color : COLOR0;
};

PixelInput VS(VertexInput input)
{
    PixelInput output;
    output.Position = mul(input.Position, World); //���� Local��ǥ�� World��ǥ�� ��ȯ
    output.Position = mul(output.Position, View); //��ȯ�� World�� View�� ��ȯ
    output.Position = mul(output.Position, Projection); //��ȯ�� View�� Projection���� ��ȯ

    output.Color = input.Color;
	
    return output;
}

float4 PS(PixelInput input) : SV_Target //render target�� pixel��� ��, ��� ������ �˷���� ��(�ø�ƽ(SV_Target))
{//�������� ������ color�� ��ȯ
    return input.Color;
}

