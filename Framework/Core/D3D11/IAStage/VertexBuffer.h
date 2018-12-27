#pragma once

//id3d11buffer(vertexbuffer) ���� Ŭ����
class VertexBuffer
{
public:
	VertexBuffer(class Context *context);
	virtual ~VertexBuffer();

	template <typename T> //texture, color�� ���� �޶����Ƿ� ���ø�����
	void Create(const vector<T> &vertices, const D3D11_USAGE &usage = D3D11_USAGE_DEFAULT); //���� ���۸� ��� ���·� ����� ���ΰ�, ���� �⺻������ ����ϰڴ�
	void Clear();

	void BindPipeline();

private:
	class Graphics *graphics; //buffer�� ���� device�� ����ϱ� ����

	ID3D11Buffer *vbuffer;
	uint stride; //���� �� ���� ũ��
	uint offset; //������
};

template<typename T>
inline void VertexBuffer::Create(const vector<T>& vertices, const D3D11_USAGE & usage)
{
	assert(!vertices.empty()); //��������� true��ȯ

	Clear();

	stride = sizeof(T); 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	/*
		default : cpu������ �� �����͸� ���� �� ���� �� ����. �а� ���� cpu.
		dynamic : �ǽð����� ���Ž�Ű�� ���� ���. gpu�� �б⸸ ��. ���� �� cpu��. ���� ����.
		staging : gpu���� ���� �����͸� cpu�� ���� ����. cpu�� �б⸸ ����. �ſ� ����.
		immutable : �б� gpu. cpu�� �������� �� �� ����. ���� �� �ʱ�ȭ �ʿ�.
	*/
	if (usage == D3D11_USAGE_DYNAMIC) 
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //���� ���۸� ��� ���۷� ���� ���ΰ�, vertex buffer�� ���ڴ�
	desc.ByteWidth = sizeof(T) * vertices.size();//������ ũ�⸦ ��� ������ �� ���ΰ�

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = vertices.data();

	HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &subData, &vbuffer);
	assert(SUCCEEDED(hr));
}
