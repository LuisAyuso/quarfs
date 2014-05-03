#pragma once

#include "render/renderer.h"
#include "world/world.h"
#include "input/listener.h"

#include <GL/glew.h> // include GLEW and new version of GL on Windows
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <set>
#include <vector>


class Camera : public InputListener{

    glm::vec3 pos, up, lookAt;
    Renderer& renderer;

    glm::mat4 camMat;

public:
    Camera (const glm::vec3& pos, const glm::vec3& lookAt, const glm::vec3& up, Renderer& renderer);
    ~Camera();

    // from input listener
    void keyUp();
    void keyDown();
    void keyLeft();
    void keyRight();
    void mouseDiff(float x, float y);
    void update();
};


void shotFrame(const TreeNode& tree);
void shotFrame(const VaoList& list);


