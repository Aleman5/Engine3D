#pragma once

#include <iostream>
#include <vector>
#include "Exports.h"
#include "Renderer.h"
#include "Component.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "Structs.h"

using namespace std;

class ENGINEDLL_API Node
{
	Renderer* renderer;

	string name;
	bool isActive;
	bool shouldDraw;
	vector<Node*> nodeChilds;
	vector<Component*> components;

	Mesh* myMesh;

	void Delete();

protected:
	void Start();

public:
	Node* parent;
	Transform* transform;

	FCCubeData fcData;

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

	void ActivateCameraDebugMode();
	void DesactivateCameraDebugMode();
	void ActivateMeshDebugMode();
	void DesactivateMeshDebugMode();

	string GetName();
	Node* GetChild(unsigned int index);
	Component* GetComponent(string type);
	Component* GetComponentInChildren(string type);
	vector<Component*> GetComponents(string type);

	Node(string name);
	Node(string name, Node* parent);
	~Node();
};