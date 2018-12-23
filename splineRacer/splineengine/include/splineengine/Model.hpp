#pragma once
#ifndef __MODEL__HPP
#define __MODEL__HPP

#include <iostream>
#include <GL/glew.h>
#include "Collider.hpp"
#include <iostream>
#include <glimac/Geometry.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/SDLWindowManager.hpp>
#include <glimac/common.hpp>
#include <glimac/Program.hpp>
#include <splineengine/Settings.hpp>
#include <splineengine/common.hpp>

namespace splineengine {

/// \brief Represents the 3D model of an object
class Model {
	// METHODS
	public:
		// CONSTRUCTORS - DESTRUCTORS
		/// \brief default model Constructor
		Model();

		/// \brief Model Constructor by model name
		/// \param modelName : std::string& name of the model
		Model(const std::string &modelName);

		/// \brief Model Copy constructor
		/// \param model : Model Model to copy
		Model(const Model &model);

		/// \brief model Destructor
		~Model();

		/// \brief set VBO of the model
		/// \param geometry geometry of the model
		void setVBO(const glimac::Geometry &geometry);

		/// \brief set IBO of the model
		/// \param geometry geometry of the model
		void setIBO(const glimac::Geometry &geometry);

		/// \brief set VAO based on the VBO and VAO
		void setVAO();

		/// \brief get VAO
		GLuint getVAO() const {
			return _VAO;
		};

		/// \brief get IBO
		GLuint getIBO() const {
			return _IBO;
		};

		/// \brief get Geometry
		glimac::Geometry geometry() const {
			return _geometry;
		};

	private:
		/// \brief Contains the vertices and materials of the model
		glimac::Geometry _geometry;
		/// \brief Represent the VBO of the model
		GLuint _VBO;
		/// \brief Represent the IBO of the model
		GLuint _IBO;
		/// \brief Represent the VAO of the model
		GLuint _VAO;
		/// \brief Collider of the model
		Collider _collider;
};

}

#endif