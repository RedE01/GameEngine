workspace "Game"
	architecture "x64"
	startproject "Game"

	configurations { "Debug", "Release" }

include "GameEngine/vendor/glfw"
include "GameEngine/vendor/glew"

include "GameEngine"
include "Game"