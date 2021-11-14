#pragma once
#include "TextureFormat.h"

namespace GameEngine {

	class Renderbuffer {
	public:
		Renderbuffer();
		Renderbuffer(const Renderbuffer& other) = delete;
		Renderbuffer(Renderbuffer&& other) = delete;
		~Renderbuffer();

		void bind();
		void createRenderbufferStorage(TextureFormat format, unsigned int width, unsigned int height);
		unsigned int getID() const;

        Renderbuffer& operator=(const Renderbuffer& other) = delete;
        Renderbuffer& operator=(Renderbuffer&& other) = delete;

	private:
		unsigned int m_renderbufferID;
	};

}