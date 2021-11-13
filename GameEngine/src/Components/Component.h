#pragma once
#include "../Entity.h"

namespace GameEngine {

    class Component {
    public:
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
