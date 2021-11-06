#pragma once
#include "../Entity.h"

namespace GameEngine {

    class Component {
    protected:
        Entity getEntity() const {
            return m_entity;
        }

    private:
        Entity m_entity;

        friend class Entity;
    };

}
