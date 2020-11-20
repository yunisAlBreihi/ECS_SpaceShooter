#include <stdio.h>
#include <bitset>
#include <vector>

// Some typedefs to aid in reading
typedef unsigned long long EntityID;
const int MAX_COMPONENTS = 32;
const int MAX_ENTITIES = 32;
typedef std::bitset<MAX_COMPONENTS> ComponentMask;

extern int s_componentCounter = 0;
template<class T>
int GetId()
{
	static int s_componentId = s_componentCounter++;
	return s_componentId;
}

struct Scene
{
	//All the information we need about each entity
	struct EntityDesc
	{
		EntityID id;
		ComponentMask mask;
	};
	std::vector<EntityDesc> entities;

	EntityID NewEntity()
	{
		entities.push_back({ entities.size(), ComponentMask() });
		return entities.back().id;
	}

	template<typename T>
	void Assign(EntityID id) 
	{
		int componentId = GetId<T>();
		entities[id].mask.set(componentId);
	}
};

struct ComponentPool 
{
	ComponentPool(size_t elementsize) 
	{
		// We'll allocate enough memory to hold MAX_ENTITIES, each with element size
		elementsize = elementsize;
		pData = new char[elementsize * MAX_ENTITIES];
	}

	~ComponentPool() 
	{
		delete[] pData;
	}
	
	inline void* get(size_t index) 
	{
		// looking up the component at the desired index
		return pData + index * elementSize;
	}

	char* pData{ nullptr };
	size_t elementSize{ 0 };
};

struct TransformComponent
{
	float position{ 1.0f };
	float rotation{ 2.0f };
};

int main()
{
	Scene scene;

	EntityID newEnt = scene.NewEntity();
	scene.Assign<TransformComponent>(newEnt);

	printf("TransformComponent ID: %i\n", GetId<TransformComponent>());
	return 0;
}