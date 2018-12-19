#include <glimac/SDLWindowManager.hpp>
#include <glimac/Geometry.hpp>
#include <splineengine/GameObject.hpp>


namespace splineengine {

const glm::mat4 GameObject::matrix(const Spline& spline) {
    if (_isStatic && _hasMatrix) {
        return _transformMat;
    }
    
    glm::mat4 objMatrix = spline.matrix(_sPosition);
    objMatrix = glm::scale(objMatrix, _scale);
    objMatrix = glm::rotate(objMatrix, _rotation[FWD],  -fwdVec);
    objMatrix = glm::rotate(objMatrix, _rotation[LEFT], -leftVec);
    objMatrix = glm::rotate(objMatrix, _rotation[UP],   -upVec);
    
    if (_isStatic) {
        _transformMat = objMatrix;
        _hasMatrix = true;
    }
    return objMatrix;
}

const glm::mat4 GameObject::matrix() {
    // if (_isStatic && _hasMatrix) {
    //     return _transformMat;
    // }
    
    glm::mat4 objMatrix = glm::mat4();
    objMatrix = glm::scale(objMatrix, _scale);
    objMatrix = glm::rotate(objMatrix, _rotation[FWD],  -fwdVec);
    objMatrix = glm::rotate(objMatrix, _rotation[LEFT], -leftVec);
    objMatrix = glm::rotate(objMatrix, _rotation[UP],   -upVec);
    
    // if (_isStatic) {
    //     _transformMat = objMatrix;
    //     _hasMatrix = true;
    // }
    return objMatrix;
}

void GameObject::draw() const {
    glBindVertexArray(_model.getVAO());
    glDrawElements(GL_TRIANGLES, _model.geometry().getIndexCount(), GL_UNSIGNED_INT, 0); // Draw all meshes
    glBindVertexArray(0);
}

}