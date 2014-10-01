#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

class CameraControls
{
    private:
        GLFWwindow* _window;

        // These can be used later to recompute the view matrix
        // every time (to prevent round-off from transforming the
        // camera every time).
        // glm::vec4 _cam_pos;
        // glm::quat _cam_orientation;
    public:
        double _mouse_speed = 0.05;
        CameraControls(GLFWwindow* window);
        Scene handle(float delta_time, Scene _s, int width, int height);
        Scene handle_mouse(float delta_time, Scene _s, int width, int height);
        Scene handle_keyboard(Scene _s);
        void set_mouse_speed(double speed);
};

#endif

