#pragma once
#ifndef __COMMON__HPP
#define __COMMON__HPP

#include<glm/gtc/random.hpp>

// declare global variables here

// print debug messages ?
const bool debug = true;

// special level ID to identif the endless level
const int LEVEL_ENDLESS = 0;
// number of levels (including the endless level)
const uint NUMBER_OF_LEVELS = 2;

// directions for spline-space vectors
const int FWD = 0;
const int LEFT = 1; 
const int UP = 2;

// spline-space vectors
const glm::vec3 sFwdVec  (1.f, 0.f, 0.f);
const glm::vec3 sLeftVec (0.f, 1.f, 0.f);
const glm::vec3 sUpVec   (0.f, 0.f, 1.f);

// world space vectors
const glm::vec3 fwdVec  ( 0.f, 0.f,-1.f);
const glm::vec3 leftVec (-1.f, 0.f, 0.f);
const glm::vec3 upVec   ( 0.f, 1.f, 0.f);




#endif