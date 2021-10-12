#pragma once

namespace GameEngine {

	template<typename T>
	struct Resource {
		Resource(T* resource) : ID(resource->getID()) {}
		const unsigned int ID;
	};

}