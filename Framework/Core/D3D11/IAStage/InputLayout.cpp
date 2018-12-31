#include "stdafx.h"
#include "InputLayout.h"	

InputLayout::InputLayout(Context *context) : layout(nullptr)
{
	graphics = context->GetSubsystem<Graphics>();
}

InputLayout::~InputLayout()
{
	Clear();
}
 
void InputLayout::Create(D3D11_INPUT_ELEMENT_DESC * descs, const uint & count, ID3D10Blob * blob)
{
	//layout을 만드는데 vertex shader가 들어가는 이유 : 셰이더의 시멘틱과 엘리먼트의 시멘틱이 일치하는지 확인하기 위함
	HRESULT hr = graphics->GetDevice()->CreateInputLayout(descs, count, blob->GetBufferPointer(), blob->GetBufferSize(), &layout);
	assert(SUCCEEDED(hr));
}

void InputLayout::Create(ID3D10Blob * blob)
{
	if (!blob) return;
	
	//Create Shader Reflector
	ID3D11ShaderReflection *reflector = nullptr; //들어오는 특정 데이터를 반사시켜 읽을 수 있게 함
	HRESULT hr = D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&reflector));
	assert(SUCCEEDED(hr));

	//Shader Information
	D3D11_SHADER_DESC shaderDesc;
	reflector->GetDesc(&shaderDesc); //reflector의 정보(blob)로 채움

	vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDescs; //hlsl의 정보를 받아오는데 몇개가 있을지 모르므로 가변배열인 vector로 정의
	for (uint i = 0; i < shaderDesc.InputParameters; i++) { //shader가 받는 parameter의 개수를 알려줌
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc; //각각의 의미있는 parameter를 signiture라고 함
		reflector->GetInputParameterDesc(i, &paramDesc);

		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		//자료형을 따로 구해줘야함
		//첫 parameter는 position인데 자료형은 float4이고 우리가 넣어주는 것은 vector3임
		//이를 자동으로 맞춰주는데 시멘틱이 다르면 인식을 못하므로 position에 한해서 시멘틱을 맞춰줘야 함.
		string temp = elementDesc.SemanticName;
		if (temp == "POSITION") elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		else if (paramDesc.ComponentType == 1) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15) {
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}
		inputLayoutDescs.push_back(elementDesc);
	}

	Create(inputLayoutDescs.data(), inputLayoutDescs.size(), blob);
}

void InputLayout::Clear()
{
	SAFE_RELEASE(layout);
}

void InputLayout::BindPipeline()
{
	graphics->GetDeviceContext()->IASetInputLayout(layout); //정보의 구간, 용도 등을 알려줌
}
