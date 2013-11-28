
#pragma once

#include "camera.h"
#include "renderer.h"

// forward declaration
class GLFWwindow;


class WindowManager{
    
    GLFWwindow* window;
    Renderer    renderer;

    double fps;
    void update_fps_counter ();

    public:
        WindowManager(unsigned w, unsigned h, const char* name);
        WindowManager(const WindowManager& ) = delete;
        WindowManager(const WindowManager&& ) = delete;
        ~WindowManager();

        Renderer& getRenderer();
        Camera getCamera(const vec3& pos, const vec3& inclination, const vec3& lookAt) const;

        // in loop functionality
        void setupFrame();
        void finishFrame();

        // query state
        bool isFinish()const;
        double getFrameRate ()const;
};
