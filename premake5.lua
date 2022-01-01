workspace "Game"
	architecture "x64"
	startproject "Game"

	configurations { "Debug", "Release" }

include "GameEngine/vendor/glfw"
include "GameEngine/vendor/glew"
include "GameEngine/vendor/assimp"
include "GameEngine/vendor/imgui"
include "GameEngine/vendor/yaml"

include "GameEngine"
include "Game"
