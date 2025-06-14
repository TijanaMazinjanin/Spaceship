#include <glm/glm.hpp>

enum SpaceshipState {
    IDLE,
    MOVING_UP,
    MOVING_DOWN,
    ROTATING_LEFT,
    ROTATING_RIGHT

};

enum CameraViewMode {
    FOLLOW_SHIP_VIEW,
    FIXED_VIEW // Or any other view mode you want
};

class Spaceship{
public:
    SpaceshipState State;
    CameraViewMode CurrentCameraMode;
    bool Keys[1024];
    unsigned int Width, Height;

    glm::vec3 Position;
    float Yrotation;
    float Xrotation;

    glm::vec3 CameraOffset; //Relative to spaceship
    float CameraHeightOffset; // How high above the spaceship the camera is

    Spaceship(unsigned int width, unsigned int height);
    ~Spaceship();

    void Init();
    void Render();
    void ProcessInput();
    void Update();
private:
};
