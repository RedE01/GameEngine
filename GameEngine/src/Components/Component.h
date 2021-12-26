#pragma once
#include "../Entity.h"

namespace GameEngine {

    class Component {
    public:
        virtual ~Component() = default;
        virtual std::string getName() const = 0;
        virtual void init() {}
        virtual void update() {}

    protected:
        Entity getEntity() const {
            return m_entity;
        }

    private:
        Entity m_entity;

        friend class Entity;
    };

}
