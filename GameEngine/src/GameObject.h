#pragma once

namespace GameEngine {

	class GameObject {
	public:
		GameObject(unsigned int id);

		void init();
		void deinit();
		void update();

		const unsigned int getID() const;

	private:
		unsigned int m_id;
	};

}
