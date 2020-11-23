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

	std::vector<ComponentPool> componentPools;

	template<typename T>
	T* Assign(EntityID id)
	{
		int componentId = GetId<T>();

		if (componentPools.size() <= componentId) // Not enough component pool
		{
			componentPools.resize(componentId + 1, nullptr);
		}
		if (componentPools[componentId] == nullptr) // New component, make a new pool
		{
			componentPools[componentId] = new ComponentPool(sizeof(T));
		}

		// Looks up the component in the pool, and initializes it with placement new
		T* pComponent = new (componentPools[componentId]->get(id)) T();

		// Set the bit of for this component to true and return the created component
		entities[id].mask.set(componentId);
		return pComponent;
	}

	template<typename T>
	T* Get(EntityID id) 
	{
		int componentId = GetId<T>();
		if (!entities[id].mask.test(componentId))
		{
			return nullptr;
		}

		T* pComponent = static_cast<T*>(componentPools[componentId]->ComponentPool(id));
		return pComponent;
	}
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