
#pragma once

// forward declaration
class GLFWwindow;


class WindowManager{
    
    GLFWwindow* window;
    unsigned shader_program;

    double fps;
    void update_fps_counter ();

    public:
        WindowManager(unsigned w, unsigned h, const char* name);
        WindowManager(const WindowManager& ) = delete;
        WindowManager(const WindowManager&& ) = delete;
        ~WindowManager();

        void setupFrame();
        void finishFrame();

        bool isFinish()const;

        double getFrameRate ();
};
