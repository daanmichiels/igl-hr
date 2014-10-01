#ifndef CAMERACONTROLS_H
#define CAMERACONTROLS_H

class CameraControls
{
    private:
        Camera _cam;
        GLFWwindow* _win;

        // These can be used later to recompute the view matrix
        // every time (to prevent round-off from transforming the
        // camera every time).
        // glm::vec4 _cam_pos;
        // glm::quat _cam_orientation;
    public:
        CameraControls(GLFWwindow* window, Camera cam);
        void handle(float dt);
        void handle_mouse(float dt);
        void handle_keyboard(float dt);
}

#endif

