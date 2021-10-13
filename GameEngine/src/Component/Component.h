#pragma once

namespace GameEngine {

	class Component {
	public:
		Component(unsigned int id);

		void init();
		void update();

		const unsigned int getID() const;

	private:
		unsigned int m_id;
	};
	
}