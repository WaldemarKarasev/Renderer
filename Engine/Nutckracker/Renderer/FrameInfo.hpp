#pragma once

#include "Camera.hpp"
#include "GameObject.hpp"

namespace NK {
	
    #define  MAX_LIGHTS 10

    struct PointLight
    {
        glm::vec4 position{}; // ignore w
        glm::vec4 color{};    // w is intensity 
    };
    
    struct GlobalUbo
    {
        glm::mat4 projection{1.f};
        glm::mat4 view{1.f};
        glm::mat4 inverseView{1.f};
        glm::vec4 ambientLightColor{1.f, 1.f, 1.f, 0.02f}; // w is intensity
        PointLight pointLight[MAX_LIGHTS];
        int numLights;
    };

    struct FrameInfo
	{
		int    frameIndex;
        float  frameTime;
        Camera& camera;
		GameObject::Map& gameObjects;
	};


    struct SystemInfo
    {
        bool isLightSystem = false;
    };
}