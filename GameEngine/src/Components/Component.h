#pragma once

namespace GameEngine {

    class Entity;

    class Component {
    protected:
        Entity* getEntity() const {
            return m_entity;
        }

    private:
        Entity *m_entity = nullptr;

        friend class Entity;
    };

}
