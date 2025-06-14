#include "spaceship.h"
#include "resource_manager.h"
#include "renderer.h"
#include "texture.h"
#include <GLFW/glfw3.h>
#include <glm/ext/quaternion_transform.hpp>

using namespace std;

Renderer *renderer;

Spaceship::Spaceship(unsigned int width, unsigned int height): Width(width), Height(height), State(IDLE),
    Position(0.0f, 0.0f, 1.0f), Xrotation(-25.0f), Yrotation(15.0f),
    CameraOffset(0.5f, 0.0f, 3.0f), CameraHeightOffset(1.0f), CurrentCameraMode(FIXED_VIEW)
    {
        for (int i = 0; i < 1024; ++i) {
                Keys[i] = false;
            }
    }

Spaceship::~Spaceship(){

}

void Spaceship::Init(){
    Shader cubeShader = ResourceManager::LoadShader("cube.vert", "cube.frag", NULL, "cubeShader");
    Shader basicShader = ResourceManager::LoadShader("basic.vert", "basic.frag", NULL, "basicShader");

    ResourceManager::LoadTexture("assets/textures/space.jpg", true, "space");
    ResourceManager::LoadTexture("assets/textures/glass.jpg", true, "glass");
    ResourceManager::LoadTexture("assets/textures/cat.jpeg", true, "cat");
    ResourceManager::LoadTexture("assets/textures/red_texture.jpg", true, "red_texture");
    ResourceManager::LoadTexture("assets/textures/yellow_texture.jpg", true, "yellow_texture");
    ResourceManager::LoadTexture("assets/textures/water.jpg", true, "water");

    float fov = 45.0f;                       // ugao u stepenima
    float aspectRatio = this->Width / this->Height;     // širina / visina prozora
    float near = 0.1f;                       // bliska ravan
    float far = 100.0f;                      // daleka ravan
    glm::mat4 projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); // Move camera back

    cubeShader.Use();

    cubeShader.SetMatrix4("projection", projection);
    cubeShader.SetMatrix4("view", view);

    renderer = new Renderer();
}

void Spaceship::Render(){

    Shader basicShader = ResourceManager::GetShader("basicShader");
    Shader cubeShader = ResourceManager::GetShader("cubeShader");

    //Background rendering

    glDisable(GL_DEPTH_TEST);
    glDepthMask(GL_FALSE);

    basicShader.Use();
    glm::mat4 projectionOrtho = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    basicShader.SetMatrix4("projection", projectionOrtho);
    renderer->DrawRectangle(basicShader, glm::vec2(0.0f), glm::vec2(Width, Height), 0.0f, glm::vec4(1.0f), ResourceManager::GetTexture("space"), true);
    glDisable(GL_BLEND);

    //3D Scene Setup

    glm::mat4 view;

    if (this->CurrentCameraMode == FOLLOW_SHIP_VIEW) {

        glm::vec3 cameraTarget = this->Position;

        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(this->Yrotation), glm::vec3(0.0f, 1.0f, 0.0f));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(this->Xrotation), glm::vec3(1.0f, 0.0f, 0.0f));

        // Transformiši CameraOffset vektor korišćenjem samo rotacije (bez translacije)
        glm::vec3 rotatedCameraOffset = glm::vec3(rotationMatrix * glm::vec4(CameraOffset.x, CameraOffset.y, CameraOffset.z, 0.0f));

        // Dodaj rotirani offset i offset visine na poziciju broda
        glm::vec3 cameraPosition = this->Position + rotatedCameraOffset;
        cameraPosition.y += CameraHeightOffset; // Dodaj visinu iznad broda

        // 4. Kreiraj view matricu koristeći glm::lookAt
        view = glm::lookAt(cameraPosition, // Pozicija kamere
                                        cameraTarget,   // Gleda u centar broda
                                        glm::vec3(0.0f, 1.0f, 0.0f)); // Up vektor (uobičajeno Y-osa)
    }
    else{
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -0.5f, -5.0f));
    }

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)this->Width / (float)this->Height, 0.1f, 100.0f);


    glm::mat4 shipModel = glm::mat4(1.0f);
    shipModel = glm::translate(shipModel, this->Position);
    shipModel = glm::rotate(shipModel, glm::radians(this->Yrotation), glm::vec3(0.0f, 1.0f, 0.0f));
    shipModel = glm::rotate(shipModel, glm::radians(this->Xrotation), glm::vec3(1.0f, 0.0f, 0.0f));

    glDepthMask(GL_TRUE); // <-- RESTORE: Allow depth buffer writes for the 3D scene
    glEnable(GL_DEPTH_TEST);
    cubeShader.Use();
    cubeShader.SetMatrix4("projection", projection);
    cubeShader.SetMatrix4("view", view);

    glm::mat4 shipRootModel = shipModel;
    // shipRootModel = glm::translate(shipRootModel, this->Position); // Apply current position
    // shipRootModel = glm::rotate(shipRootModel, glm::radians(this->Yrotation), glm::vec3(0.0f, 1.0f, 0.0f));  // Apply current yaw
    // shipRootModel = glm::rotate(shipRootModel, glm::radians(this->Xrotation), glm::vec3(1.0f, 0.0f, 0.0f)); // Apply current pitch
    // shipRootModel = glm::rotate(shipRootModel, glm::radians(-25.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Yaw
    // shipRootModel = glm::rotate(shipRootModel, glm::radians(15.0f), glm::vec3(1.0f, 0.0f, 0.0f));  // Pitch

    // Cube - main spaceship body
    glm::mat4 bodyModel = shipRootModel;
    bodyModel = glm::scale(bodyModel, glm::vec3(0.4f, 1.2f, 0.4f));
    renderer->DrawCube(cubeShader, bodyModel, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), ResourceManager::GetTexture("red_texture"), true);

    glm::mat4 windowModel = shipRootModel;
    windowModel = glm::scale(windowModel, glm::vec3(0.2f, 0.2f, 0.2f));
    windowModel = glm::translate(windowModel, glm::vec3(0.0f, 0.9f, -0.6f));
    renderer->DrawCube(cubeShader, windowModel, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), ResourceManager::GetTexture("glass"), true);

    // Pyramids

    // Wings
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glm::mat4 wingsModel = shipRootModel;
    wingsModel = glm::scale(wingsModel, glm::vec3(1.0f, 0.4f, 0.4f));
    wingsModel = glm::translate(wingsModel, glm::vec3(0.0f, -0.5f, 0.0f));

    renderer->DrawPyramid(cubeShader, wingsModel, 0, glm::vec4(0.9f, 0.2f, 0.2f, 0.2f), ResourceManager::GetTexture("yellow_texture"), true);

    //Nose of the spaceship
    glm::mat4 noseModel = shipRootModel;
    noseModel = glm::scale(noseModel, glm::vec3(0.4f, 0.5f, 0.4f));
    noseModel = glm::translate(noseModel, glm::vec3(0.0f, 1.2f, 0.0f));
    renderer->DrawPyramid(cubeShader, noseModel, 0, glm::vec4(0.9f, 0.2f, 0.2f, 0.2f), ResourceManager::GetTexture("yellow_texture"), true);

    glDisable(GL_BLEND);

    cubeShader.Use();
    glm::mat4 stationModel = glm::mat4(1.0f);
    stationModel = glm::translate(stationModel, glm::vec3(0.0f, 0.0f, 0.0f));

    stationModel = glm::scale(stationModel, glm::vec3(1.2f, 1.2f, 1.2f));
    stationModel = glm::rotate(stationModel, 10.0f, glm::vec3(0.0f, 0.3f, 0.01f));
    renderer->DrawCube(cubeShader, stationModel, glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), ResourceManager::GetTexture("water"), true);
}

void Spaceship::ProcessInput()
{
    if (!this->Keys[GLFW_KEY_W] && !this->Keys[GLFW_KEY_S] &&
        !this->Keys[GLFW_KEY_A] && !this->Keys[GLFW_KEY_D]) {
            this->State = IDLE;
    }

	if (this->Keys[GLFW_KEY_W])
	{
		this->State = MOVING_UP;
	}

	if (this->Keys[GLFW_KEY_S])
	{
		this->State = MOVING_DOWN;
	}

	if (this->Keys[GLFW_KEY_A])
	{
		this->State = ROTATING_LEFT;
	}
	if (this->Keys[GLFW_KEY_D])
	{
		this->State = ROTATING_RIGHT;
	}

	if (this->Keys[GLFW_KEY_C]) {
        if (this->CurrentCameraMode == FOLLOW_SHIP_VIEW) {
            this->CurrentCameraMode = FIXED_VIEW;
        } else {
            this->CurrentCameraMode = FOLLOW_SHIP_VIEW;
        }
        this->Keys[GLFW_KEY_C] = false;

	}
}
void Spaceship::Update(){
    float dt = 0.01;
    float moveSpeed = 2.0f;
    float rotateSpeed = 90.0f;

    // --- Kalkulacija vektora "napred" broda ---
    // Prvo, kreiraj matricu rotacije koja odražava trenutnu orijentaciju broda (Yaw i Pitch).
    // Nije potrebno uključiti translaciju ovde, samo rotaciju.
    glm::mat4 rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(this->Yrotation), glm::vec3(0.0f, 1.0f, 0.0f));
    rotationMatrix = glm::rotate(rotationMatrix, glm::radians(this->Xrotation), glm::vec3(1.0f, 0.0f, 0.0f));

    // Pretpostavljamo da je lokalni "napred" vektor broda (0, 0, -1) ili (0, 0, 1).
    // Većina 3D modela ima svoj "napred" vektor usmeren duž negativne Z-ose (ako model gleda "iz ekrana")
    // ili pozitivne Z-ose (ako model gleda "u ekran").
    // Ako se tvoj brod kreće unazad kada stisneš W, promeni ovaj vektor u (0.0f, 0.0f, 1.0f).
    glm::vec3 localForward = glm::vec3(0.0f, 0.0f, -1.0f); // Primer: Negativna Z-osa je "napred"

    // Transformiši lokalni "napred" vektor u svetske koordinate
    glm::vec3 worldForward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(localForward, 0.0f)));

    // --- Kretanje napred/nazad (W/S) ---
    if (this->Keys[GLFW_KEY_W])
    {
        this->Position += worldForward * moveSpeed * dt;
    }
    if (this->Keys[GLFW_KEY_S])
    {
        this->Position -= worldForward * moveSpeed * dt;
    }

    // --- Rotiranje levo/desno (A/D) ---
    // Ova logika ostaje ista, jer menja this->Yaw direktno.
    if (this->Keys[GLFW_KEY_A])
    {
        this->Yrotation += rotateSpeed * dt;
    }
    if (this->Keys[GLFW_KEY_D])
    {
        this->Yrotation -= rotateSpeed * dt;
    }

    // Ograniči yaw na opseg [0, 360) da ne raste unedogled
    if (this->Yrotation > 360.0f) this->Yrotation -= 360.0f;
    if (this->Yrotation < 0.0f) this->Yrotation += 360.0f;
}
