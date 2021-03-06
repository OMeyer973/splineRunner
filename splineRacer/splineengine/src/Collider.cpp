#include <splineengine/Collider.hpp>
#include <iostream>

namespace splineengine {

// Constructors
Collider::Collider(const glm::vec3& position, const float radius)
	:_position(position), _radius(radius)
{}

// Destructor
Collider::~Collider()
{}

bool Collider::intersect(const Collider other, 
	const glm::mat4 selfTransformMat, const float selfScale, 
	const glm::mat4 otherTransformMat, const float otherScale) const {

	// multiplication of homogenous coordinates with 4 components and immediate cast to vec3
	glm::vec3 selfWorldPos(selfTransformMat * glm::vec4(_position,1.f));
	glm::vec3 otherWorldPos(otherTransformMat * glm::vec4(other._position,1.f));

	float sqDist = glm::distance(selfWorldPos, otherWorldPos) * glm::distance(selfWorldPos, otherWorldPos);
	float sqRadiuses =  _radius * _radius * selfScale * selfScale 
						+ other._radius * other._radius * otherScale * otherScale;
	if (sqDist < sqRadiuses) {
		if (debug) std::cout << "collision at " << selfWorldPos << " (world)" << std::endl;
		return true;
	}

	return false;
}


}