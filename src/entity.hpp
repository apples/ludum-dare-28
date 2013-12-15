#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <exception>
#include <map>
#include <memory>
#include <string>

struct UIDCounter
{
    static int value;
};

template <typename T>
int UIDGet()
{
    static const int value = ++UIDCounter::value;
    return value;
}

class EntityE_DuplicateComponent
    : public std::exception
{
  public:
    virtual const char* what() const noexcept override;
};

class EntityComponentBase
{
  public:
    virtual ~EntityComponentBase() = 0;
    virtual int getUID() const = 0;
};

template <typename T>
class EntityComponent
    : public EntityComponentBase
{
  public:
    virtual ~EntityComponent() override = 0;

    virtual int getUID() const override
    {
        return UIDGet<T>();
    }
};

template <typename T>
EntityComponent<T>::~EntityComponent() = default;

class Entity
{
    std::map<int, std::unique_ptr<EntityComponentBase>> components;

  public:
    template <typename T>
    T* addComponent()
    {
        int id = UIDGet<T>();
        auto iter = components.find(id);
        if (iter != components.end())
        {
            throw EntityE_DuplicateComponent();
        }
        EntityComponentBase* ptr = new T;
        components.insert(iter, std::make_pair(id, std::unique_ptr<EntityComponentBase>(ptr)));
        return static_cast<T*>(ptr);
    }

    template <typename T>
    T* getComponent()
    {
        int id = UIDGet<T>();
        auto iter = components.find(id);
        if (iter == components.end())
        {
            return nullptr;
        }
        return static_cast<T*>(iter->second.get());
    }
};

#endif // ENTITY_HPP
