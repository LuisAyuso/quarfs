#pragma once

#include <vector>

#include "camera.h"
#include "renderer.h"
#include "input/listener.h"

// forward declaration
struct GLFWwindow;

namespace quarfs{

class WindowManager{
    
    GLFWwindow* window;
    Renderer    renderer;

    double fps;
    void update_fps_counter ();

    std::vector<InputListener*> inputListeners;

    double mouseX, lastmX;
    double mouseY, lastmY;

    public:
        WindowManager(unsigned w, unsigned h, const char* name);
        WindowManager(const WindowManager& ) = delete;
        WindowManager(const WindowManager&& ) = delete;
        ~WindowManager();

        void registerInputListener(InputListener*);

        Renderer& getRenderer();
        Camera getCamera(const glm::vec3& pos, const glm::vec3& inclination, const glm::vec3& lookAt);

        // in loop functionality
        void setupFrame();
        void finishFrame();

        // query state
        bool isFinish()const;
        double getFrameRate ()const;
};

}// namespace quarfs{
