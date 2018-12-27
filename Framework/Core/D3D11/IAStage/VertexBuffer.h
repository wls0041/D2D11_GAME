#pragma once

//id3d11buffer(vertexbuffer) 래핑 클래스
class VertexBuffer
{
public:
	VertexBuffer(class Context *context);
	virtual ~VertexBuffer();

	template <typename T> //texture, color에 따라 달라지므로 템플릿으로
	void Create(const vector<T> &vertices, const D3D11_USAGE &usage = D3D11_USAGE_DEFAULT); //만든 버퍼를 어떠한 형태로 사용할 것인가, 가장 기본값으로 사용하겠다
	void Clear();

	void BindPipeline();

private:
	class Graphics *graphics; //buffer를 위한 device를 사용하기 위함

	ID3D11Buffer *vbuffer;
	uint stride; //정점 한 개의 크기
	uint offset; //시작점
};

template<typename T>
inline void VertexBuffer::Create(const vector<T>& vertices, const D3D11_USAGE & usage)
{
	assert(!vertices.empty()); //비어있으면 true반환

	Clear();

	stride = sizeof(T); 

	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_BUFFER_DESC));

	/*
		default : cpu에서는 이 데이터를 접근 및 읽을 수 없다. 읽고 쓰기 cpu.
		dynamic : 실시간으로 갱신시키기 위해 사용. gpu가 읽기만 함. 쓰는 건 cpu가. 조금 느림.
		staging : gpu에서 쓰는 데이터를 cpu에 복사 가능. cpu는 읽기만 가능. 매우 느림.
		immutable : 읽기 gpu. cpu가 접근조차 할 수 없음. 만들 때 초기화 필요.
	*/
	if (usage == D3D11_USAGE_DYNAMIC) 
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	desc.Usage = usage;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER; //만든 버퍼를 어떠한 버퍼로 묶을 것인가, vertex buffer로 묶겠다
	desc.ByteWidth = sizeof(T) * vertices.size();//버퍼의 크기를 어느 정도로 할 것인가

	D3D11_SUBRESOURCE_DATA subData;
	ZeroMemory(&subData, sizeof(D3D11_SUBRESOURCE_DATA));
	subData.pSysMem = vertices.data();

	HRESULT hr = graphics->GetDevice()->CreateBuffer(&desc, &subData, &vbuffer);
	assert(SUCCEEDED(hr));
}
