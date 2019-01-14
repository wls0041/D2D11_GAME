#include "stdafx.h"
#include "Animator.h"

Animator::Animator(Context * context) : context(context), mode(AnimationMode::Play), curFrameNumber(0), frameTimer(0.0f), curAnimation(nullptr)
{
	timer = context->GetSubsystem<Timer>();
	resourceMgr = context->GetSubsystem<ResourceManager>();
}

Animator::~Animator()
{
}

void Animator::Update()
{
	bool bCheck = true;
	bCheck &= curAnimation != nullptr;
	bCheck &= IsPlay();

	if (bCheck) {
		frameTimer += timer->GetDeltaTimeMs();
		if (frameTimer > curAnimation->GetKeyframeFromIndex(curFrameNumber)->time) {
			curFrameNumber++;

			switch (curAnimation->GetReapeatType())
			{
			case RepeatType::Once:
				if (curFrameNumber >= curAnimation->GetKeyframeCount()) {
					curFrameNumber = curAnimation->GetKeyframeCount() - 1;
					Pause();
				}
				break;
			case RepeatType::Loop:
				curFrameNumber %= curAnimation->GetKeyframeCount();
				break;
			}

			frameTimer = 0.0f;
		}
	}
}

Keyframe * Animator::GetCurrentkeyframe()
{
	if (!curAnimation) return nullptr;
	return curAnimation->GetKeyframeFromIndex(curFrameNumber);
}

void Animator::SetCurrentAnimation(const string & animationName)
{
	assert(animations.count(animationName) > 0);
	
	curFrameNumber = 0;
	frameTimer = 0.0f;
	curAnimation = animations[animationName];
}

void Animator::RegisterAnimation(const string & filePath)
{
	auto animation = resourceMgr->Load<Animation>(filePath);

	assert(animation); //animation이 null일 경우
	assert(animations.count(animation->GetResourceName()) < 1);//이미 있을 경우

	animations[animation->GetResourceName()] = animation;
}

void Animator::RegisterAnimation(Animation * animation)
{
	assert(animation); //animation이 null일 경우
	assert(animations.count(animation->GetResourceName()) < 1);//이미 있을 경우

	animations[animation->GetResourceName()] = animation;
	resourceMgr->RegisterResource(animation);
}

void Animator::SaveToFile(const string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLDeclaration *decl = doc.NewDeclaration();
	doc.LinkEndChild(decl); //노드구조 연결

	Xml::XMLElement *root = doc.NewElement("Animator");
	doc.LinkEndChild(root); //노드

	for (auto &animation : animations) {
		Xml::XMLElement *firstElem = doc.NewElement("Animation");
		root->LinkEndChild(firstElem);
		
		firstElem->SetAttribute("Name", animation.second->GetResourceName().c_str()); //root의 속성
		firstElem->SetAttribute("Type", static_cast<int>(animation.second->GetReapeatType()));
		
		for (auto &keyframe : animation.second->GetKeyframes()) {
			Xml::XMLElement *secondElem = doc.NewElement("keyframe");
			firstElem->LinkEndChild(secondElem);

			secondElem->SetAttribute("FilePath", keyframe.filePath.c_str());
			secondElem->SetAttribute("OffsetX", keyframe.offset.x);
			secondElem->SetAttribute("OffsetY", keyframe.offset.y);
			secondElem->SetAttribute("SizeX", keyframe.size.x);
			secondElem->SetAttribute("SizeY", keyframe.size.y);
			secondElem->SetAttribute("Time", keyframe.time);
		}
	}

	doc.SaveFile(("../_Assets/Animation/" + filePath).c_str());
}

void Animator::LoadFromFile(const string & filePath)
{
	Xml::XMLDocument doc;
	Xml::XMLError error = doc.LoadFile(("../_Assets/Animation/" + filePath).c_str());
	assert(error == Xml::XMLError::XML_SUCCESS);//실패하면 그이유도 같이 나옴

	Xml::XMLElement *root = doc.FirstChildElement();
	Xml::XMLElement *firstElem = root->FirstChildElement();

	for (; firstElem != nullptr; firstElem = firstElem->NextSiblingElement()) {
		string name = firstElem->Attribute("Name");
		RepeatType type = static_cast<RepeatType>(firstElem->IntAttribute("Type"));

		Animation *animation = new Animation(context);
		animation->SetResourceName(name);
		animation->SetRepeatType(type);

		Xml::XMLElement *secondElem = firstElem->FirstChildElement();
		for (; secondElem != nullptr; secondElem = secondElem->NextSiblingElement()) {
			string filePath = secondElem->Attribute("FilePath");
			float offsetX = secondElem->FloatAttribute("OffsetX");
			float offsetY = secondElem->FloatAttribute("OffsetY");
			float sizeX = secondElem->FloatAttribute("SizeX");
			float sizeY = secondElem->FloatAttribute("SizeY");
			float time = secondElem->FloatAttribute("Time");
		
			animation->AddKeyframe(filePath, { offsetX, offsetY }, { sizeX, sizeY }, time);
		}
		RegisterAnimation(animation);
	}
}

void Animator::Play()
{
	mode = AnimationMode::Play;
	frameTimer = 0.0f;
}

void Animator::Stop()
{
	mode = AnimationMode::Stop;
}

void Animator::Pause()
{
	mode = AnimationMode::Pause;
	frameTimer = 0.0f;
	curFrameNumber = 0;
}
