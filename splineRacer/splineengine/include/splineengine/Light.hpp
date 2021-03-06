#pragma once
#ifndef __LIGHT__HPP
#define __LIGHT__HPP

#include <glimac/common.hpp>
#include <glimac/glm.hpp>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <splineengine/GLSLProgram.hpp>

namespace splineengine {

struct Properties
{
	bool _isPoint;
	glm::vec3 _posOrDir;
	glm::vec3 _Kd;
	glm::vec3 _Ks;
	float _shininess;
	glm::vec3 _lightIntensity;
};

class Light {

	public:
		Light();

		Light(const unsigned int id,
			  const bool isPoint,
			  const glm::vec3 &posOrDir,
			  const glm::vec3 &Kd,
			  const glm::vec3 &Ks,
			  const float &shininess, 
			  const glm::vec3 &lightIntensity);

		Light(const Light& light);

		~Light();

		void sendLightShader(const MultiLightProgram &multiLightProgram, const std::string &uniformArrayName) const;
		
		// CONST GETTERS

		/// \brief get PosOrDir of light
		const glm::vec3& posOrDir() const {
			return _properties._posOrDir;
		}

		/// \brief get id
		const unsigned int& id() const {
			return _id;
		}

		// NON CONST GETTERS (setter)
		
		/// \brief set PosOrDir of light
		glm::vec3& posOrDir() {
			return _properties._posOrDir;
		}

	private	:
		Properties _properties;
		unsigned int _id;
};

}

#endif