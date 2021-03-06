#pragma once
#ifndef __GAME__HPP
#define __GAME__HPP

#include <string>
#include <list>
#include "common.hpp"
#include "json.hpp"

#include "AssetManager.hpp"
#include "Player.hpp"
#include "Chaser.hpp"
#include "Obstacle.hpp"
#include "Collectable.hpp"
#include "Decoration.hpp"

#include "RenderManager.hpp"
#include "Camera.hpp"
#include "POVCamera.hpp"
#include "TrackballCamera.hpp"

namespace splineengine {

// ids of gamemodes
const int CLASSIC = 0;
const int ENDLESS = 1;

// ids of gamestates
const int RUNNING = 0;
const int LEVELWIN = 1;
const int LEVELLOSE = 2;
const int ENDLESSOVER = 3;
const int EXITING = 4;

// name of the 3D model of the player
const std::string playerModelName = "plane";
// how long is displayed the end screen before going back to the menu ?
const float endScreenTime = 5.f;
// maximum distance to the player (folowing the spline) at wich collision checks will happend
const float endlessMaxCollideDistance = 20.f;
const float levelMaxCollideDistance = 60.f;
// maximum distance to the player (folowing the spline) at wich objects will be rendered
const float endlessMaxRenderDistance = 30.f;
const float levelMaxRenderDistance = 100.f;

// max number of anim per object described in json
const uint nbAnimInJson = 4;

// number of random parts that can be generated in an infinite level
const uint nbOfRandomCHunks = 4;

/// \brief class wich represents the game scene , the player and their mechanics
class Game {
	// METHODS
	public:
		// CONSTRUCTORS - DESTRUCTORS
		/// \brief default constructor for infinite game
	    Game();

		/// \brief constructor with level name for loading level
	    Game(const std::string& levelName);

		/// \brief destructor
		~Game();

        // CONST GETTERS
		/// \brief get the player as a const ref
	    const Player& player() const { return _player; }

	    const int gameState() const { return _gameState; }

	    // NON-CONST GETTERS (can be used as setters)
	    /// \brief get the player as a ref
	    Player& player() { return _player; }


		//METHODS

		/// \brief update called at each frame (for the physics)
		void update();

		/// \brief renders the scene at each frame
		void render();

		/// \brief rotate the camera in the horizontal direction by dx pixels
		void moveCameraX(const float dx);
		/// \brief rotate the camera in the vertical direction by dx pixels
		void moveCameraY(const float dy);
		/// \brief zoom the camera view  by a factor of dz
		void zoomCamera(const float dz);
		/// \brief switch between the different available cameras
		void changeCamera();


	private:
		/// \brief creates a radom level chunk between start & finish
		void generateLevel(const float start, const float finish, const int partToGenerate);
		/// \brief load the given level
		void loadLevel(const std::string& levelName);

		/// \brief returns a single gameobject from it's json description
		GameObject gameObjFromJson(nlohmann::json j);

		/// \brief order a list of gameobject according to their FWD position coordinate
		void orderObjListFwd(std::list<std::unique_ptr<GameObject>>& objList);

		/// \brief Handle collision between 2 GameObjects
		// TODO : replace template with gameobject polymorphism or check the type or idk ... 
		template <typename T, typename U>
		void handleCollision(T& firstObject, U& secondObject);


		/// \brief renders a list of gameObjects and remove those far behind the player  
		void drawGameObjList(std::list<std::unique_ptr<GameObject>>& objList);

		/// \brief update the obstacles physics, check for collisions with player 
		///\brief and handle the collision behaviour  
		void updateObstacleList(std::list<std::unique_ptr<GameObject>>&  objList);

		/// \brief update the obstacles physics, check for collisions with player 
		///\brief and handle the collision behaviour  
		void updateCollectableList(std::list<std::unique_ptr<GameObject>>&  objList);

		// MEMBERS
		// gamemodes : CLASSIC, ENDLESS
		int _gameMode;
		// state of the game : RUNNING, LEVELWIN, LEVELLOSE, ENDLESSOVER, EXITING
		int _gameState = RUNNING;
		
		// timer before going back to the menu when a end screen is reached
		float _endScreenTimer = endScreenTime;

		/// \brief manage the camera transformations
		RenderManager _renderManager;
		/// \brief vector of pointers on available cameras
		std::vector<std::unique_ptr<Camera>> _cameras;
		/// \brief id of the current camera
		int _chosenCamera;

		/// \brief represents the spline
	    Spline _spline;
	    // maximum distance to the player (folowing the spline) at wich collision checks will happend
		const float _maxCollideDistance;
		// maximum distance to the player (folowing the spline) at wich objects will be rendered
		const float _maxRenderDistance;
		
		/// \brief represents the player
		Player _player;
		/// \brief represents the skybox
		GameObject _skybox;

		/// \brief the ennemy chasing the player !
		Chaser _alien;

		/// \brief Represents the finish line
		GameObject _finishLine;

		// game objects lists (ordered along the spline acording to their FWD coordinate)
	    std::list<std::unique_ptr<GameObject>> _decorations;
	    std::list<std::unique_ptr<GameObject>> _obstacles;
	    std::list<std::unique_ptr<GameObject>> _collectables;
};

}
#endif
