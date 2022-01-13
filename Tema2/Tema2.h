#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/Tema2Camera.h"
#include "lab_m1/Tema2/transform3D.h"


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

        

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix) override;

        void RenderMeshWithExplosion(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int shouldExplode);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        int** GenerateMap(int length, int width, int iterations);
        bool playerEnemyCollision(double enemyXDif, double enemyYDif);
        bool bulletEnemyCollision(double enemyXDif, double enemyYDif);
        bool playerWallCollision(glm::vec3 newCoords);

    protected:
        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        glm::mat4 copyMatrix;
        bool renderCameraTarget;

        float left, right, up, down, z1, z2, pov;
        int count;
        int** map;
        int o;
        double speed;
        double characterAngle;
        bool shouldShoot;
        double bulletAngle;
        int paces;
        glm::vec3 bulletCoords;
        bool bulletSpawn;
        double bulletSpeed;
        double walkX;
        double walkY;
        int dir;
        int hp;
        clock_t time;
        clock_t lastTime;
        double counter;
        double aux;
        int won;
        int over;
        double angleOX;
        int enemyTime;

    };
}   // namespace m1
