#ifndef ENGINE_GAME_OBJECT_H_INCLUDED
#define ENGINE_GAME_OBJECT_H_INCLUDED

#pragma once
#include "define.h"
#include "GameTransform.h"
#include "Components.h"
#include "Maths/Vector3.h"
#include "GamePlay/Tags.h"

template <typename ...Args>
struct Event;

namespace gce {

class Scene;

class GameObject
{
public:
    GameTransform transform;

    // May be temporary 
    // For the physic collision
    Vector3f32 m_movingDirection;
    //

    static GameObject& Create( Scene& scene);
    void Destroy();

    [[nodiscard]] uint32 GetID() const;
    [[nodiscard]] cstr GetName() const;
    [[nodiscard]] Tag1 GetTag1() const { return m_Tag1; }
    [[nodiscard]] Tag2 GetTag2() const { return m_Tag2; }

    void SetName(cstr name);
    void SetTag1(Tag1 tag) { m_Tag1 = tag; }
    void SetTag2(Tag2 tag) { m_Tag2 = tag; }

    const bool& IsTag1(Tag1 tag) const { if (m_Tag1 == tag) return true; return false; }
    const bool& IsTag2(Tag2 tag) const { if (m_Tag2 == tag) return true; return false; }

    [[nodiscard]] bool IsActive() const;
    void SetActive( bool active );

    [[nodiscard]] bool HasParent() const;
    [[nodiscard]] GameObject* GetParent();
    [[nodiscard]] GameObject const* GetParent() const;
    void RemoveParent();
    void SetParent( GameObject& parent );

    [[nodiscard]] bool HasChildren() const;
    [[nodiscard]] uint64 ChildrenCount() const;
    [[nodiscard]] Vector<GameObject*>& GetChildren();
    [[nodiscard]] Vector<GameObject*> const& GetChildren() const;
    void AddChild( GameObject& child );
    void RemoveChild( GameObject& child );

    template <class ComponentClass> [[nodiscard]] bool HasComponent() const;
    template <class ComponentClass> [[nodiscard]] bool HasComponentOfSameFamily() const;
    template <class ComponentClass> [[nodiscard]] ComponentClass* GetComponent();
    template <class ComponentClass> [[nodiscard]] ComponentClass const* GetComponent() const;
    template <class ComponentClass> ComponentClass* AddComponent();
    template <class ComponentClass> void RemoveComponent();

    template <class ScriptClass> [[nodiscard]] bool HasScript() const;
    template <class ScriptClass> [[nodiscard]] ScriptClass* GetScript();
    template <class ScriptClass> [[nodiscard]] ScriptClass const* GetScript() const;
    template <class ScriptClass> ScriptClass* AddScript();
    template <class ScriptClass> void RemoveScript();

    Event<GameObject*>* pCollisionEvents = nullptr;
    Event<GameObject*>* pCollision2DEvents = nullptr;
protected:
    explicit GameObject( cstr name = "GameObject" );
    // GameObject( GameObject const& ) = default;
    ~GameObject() = default;

    bool m_created = false;
    bool m_destroyed = false;

    bool m_active = true;

    inline static uint32 s_nextID = 0;
    uint32 m_id = s_nextID++;
    cstr m_name;
    Tag1 m_Tag1 = Tag1::None;
    Tag2 m_Tag2 = Tag2::None;

    Scene* m_pScene = nullptr;
    
    int64 m_componentsBitmask = 0;
    UnorderedMap<Component::TypeEnum, uint16> m_components {};
    UnorderedMap<uint16, uint16> m_scripts {};

    Event<> m_destroyEvent;

    Vector<GameObject*> m_children;
    GameObject* m_pParent = nullptr;

    friend struct GameTransform;
    friend struct Script;
    friend class Scene;
    friend class LifespanSystem;
    friend class PhysicSystem;
};


}

#include "GameObject.inl"

#endif