#pragma once
#ifndef __SETTINGS__HPP
#define __SETTINGS__HPP

#include "common.hpp"
#include <glimac/FilePath.hpp>
#include <SDL/SDL.h>

namespace splineengine {

/// \brief class wich represents the Settings
class Settings {
	// METHODS
	public:
		// CONSTRUCTORS - DESTRUCTORS
		/// \brief constructor
	    Settings() {};

		/// \brief destructor
		~Settings() {};

		// CONST GETTERS
		/// \brief get the time difference in seconds between the current frame and the previous (enforced)
		float deltaTime() const { return _deltaTime; };

		/// \brief get the application path
		const glimac::FilePath& appPath() const { return _appPath; };

		// NON-CONST GETTERS (can be used as setters)
		/// \brief set the application path
		glimac::FilePath& appPath() { return _appPath; };
		
	// MEMBERS
	private:
		// CONSTANTS
		/// \brief path of the app
		glimac::FilePath _appPath;
		static const uint32_t _windowWidth = 800;
		static const uint32_t _windowHeight = 600;
		/// \brief time between 2 frames (ms)
		static const Uint32 _framerate_ms = 1000 / 30;
		/// \brief time between 2 frames (seconds)
		const float _deltaTime = 1.f/30.f;



};
}
#endif