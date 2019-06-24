#pragma once

#include <iostream>
#include <vector>
#include "Exports.h"
#include "Component.h"
#include "Transform.h"

using namespace std;

class ENGINEDLL_API Node
{
	string name;
	bool isActive;
	vector<Node*> nodeChilds;
	vector<Component*> components;

	void Delete();

protected:
	void Start();

public:
	Node* parent;
	Transform* transform;

	void Update();
	void Draw();

	void SetParent(Node* parent);

	void AddChild(Node* node);
	void RemoveChild(Node* child);
	void RemoveChild(unsigned int index);

	Component* AddComponent(Component* component);
	void RemoveComponent(Component* component);
	void RemoveComponent(string type);

	void ActivateNode();
	void DesactivateNode();
	bool IsActive();

	string GetName();
	Node*  GetChild(unsigned int index);
	Component* GetComponent(string type);

	Node(string name);
	Node(string name, Node* parent);
	~Node();
};