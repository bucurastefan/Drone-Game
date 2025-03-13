#pragma once

#include <vector>

#include "components/simple_scene.h"
#include "components/text_renderer.h"
#include "lab_camera.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

        Mesh* CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        void RenderDrone(const glm::vec3& position, const glm::vec3& scale, float rotorRotation, float rotationY);
        Mesh* CreateParallelepiped(const std::string& name, const glm::vec3& dimensions);
        Mesh* CreateCube(const std::string& name, float sideLength);
        void RenderTree(const glm::vec3& position, float scale);
        void RenderObstacle(const glm::vec3& position, const glm::vec3& dimensions);
        bool IsPositionValid(const glm::vec3& pos, const std::vector<glm::vec3>& positions, float minDistance);
        bool CheckCollisionWithGround(const glm::vec3& dronePos);
        bool CheckCollisionWithObstacles(const glm::vec3& dronePos);
        bool CheckCollisionWithCheckpoint(const glm::vec3& dronePos);
        void RenderGates();
        void RenderClouds();
        Mesh* Tema2::CreateGateFrameMesh(const std::string& name, float width, float height);
        void Tema2::RenderMiniMap();

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::vec3 dronePosition;
        float droneRotationY;
        float droneSpeed;
        float droneRotationSpeed;

        std::vector<glm::vec3> objectPositions;
        float droneRadius = 0.3f;
        float obstacleTreeRadius = 1.0f;
        float obstacleBuildingRadius = 2.0f;
        float groundLevel = 0.5f;

        bool timerStarted = false;
        bool timerStopped = false;
        float startTime = 0.0f;
        float finalTime = 0.0f;

        gfxc::TextRenderer* textRenderer;
        cam::Camera* minimapCamera;
		cam::Camera* thirdPersonCamera;
        glm::vec3 minimapRotation;

        float mapMinX = -50.0f;
        float mapMaxX = 50.0f;
        float mapMinZ = -50.0f;
        float mapMaxZ = 50.0f;

        float tiltForwardAngle = 0.0f;
        float tiltRightAngle = 0.0f;


    };
}   // namespace m1
