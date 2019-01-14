#include "stdafx.h"
#include "Animation.h"

Animation::Animation(Context * context) : IResource(context), repeatType(RepeatType::Loop)
{
}

Animation::~Animation()
{
	keyframes.clear();
	keyframes.shrink_to_fit();
}

void Animation::SaveToFile(const string & filePath)
{
	Xml::XMLDocument doc; 
	Xml::XMLDeclaration *decl = doc.NewDeclaration();
	doc.LinkEndChild(decl); //노드구조 연결

	Xml::XMLElement *root = doc.NewElement("Animation");
	doc.LinkEndChild(root); //노드

	root->SetAttribute("Name", name.c_str()); //root의 속성
	root->SetAttribute("Type", static_cast<int>(repeatType));

	for (auto &keyframe : keyframes) {
		Xml::XMLElement *firstElem = doc.NewElement("keyframe");
		root->LinkEndChild(firstElem);

		firstElem->SetAttribute("FilePath", keyframe.filePath.c_str());
		firstElem->SetAttribute("OffsetX", keyframe.offset.x);
		firstElem->SetAttribute("OffsetY", keyframe.offset.y);
		firstElem->SetAttribute("SizeX", keyframe.size.x);
		firstElem->SetAttribute("SizeY", keyframe.size.y);
		firstElem->SetAttribute("Time", keyframe.time);
	}

	doc.SaveFile(("../_Assets/Animation/" + filePath).c_str());
}

void Animation::LoadFromFile(const string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(filePath.c_str());
	assert(error == Xml::XMLError::XML_SUCCESS);//실패하면 그이유도 같이 나옴

	Xml::XMLElement *root = doc.FirstChildElement();
	name = root->Attribute("Name");//문자열, 매개변수가 같고 반환형이 다른 오버로딩은 불가능하기 때문에 반환만 다른 함수로 존재
	repeatType = static_cast<RepeatType>(root->IntAttribute("Type"));//int

	Xml::XMLElement *firstElem = root->FirstChildElement();
	for (; firstElem != nullptr; firstElem = firstElem->NextSiblingElement()) {
		string filePath = firstElem->Attribute("FilePath");
		float offsetX = firstElem->FloatAttribute("OffsetX");
		float offsetY = firstElem->FloatAttribute("OffsetY");
		float sizeX = firstElem->FloatAttribute("SizeX");
		float sizeY = firstElem->FloatAttribute("SizeY");
		float time = firstElem->FloatAttribute("Time");

		AddKeyframe(filePath, { offsetX, offsetY }, { sizeX, sizeY }, time);
	}
}

Keyframe * Animation::GetKeyframeFromIndex(const uint & index)
{
	if(keyframes.size() <= index) return nullptr;
	return &keyframes[index];
}

void Animation::AddKeyframe(const string & filePath, const D3DXVECTOR2 & offset, const D3DXVECTOR2 & size, const float & time)
{
	keyframes.emplace_back(filePath, offset, size, time);
}
