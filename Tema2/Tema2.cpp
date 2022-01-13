#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;

Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


int** Tema2::GenerateMap(int length, int width, int iterations) {
    int a = length / 2;
    int b = width / 2;
    int** aux = new int*[a];
    int** result = new int*[length];
    for (int i = 0; i < length / 2; i++) {
        aux[i] = new int[b];
        for (int j = 0; j < width / 2; j++) {
            aux[i][j] = -1;
        }
    }

    for (int i = 0; i < length; i++) {
        result[i] = new int[width];
        for (int j = 0; j < width; j++) {
            result[i][j] = 1;
        }
    }

    while (iterations-- > 0) {
        int x = rand() % (length / 2);
        int y = rand() % (width / 2);
        int lastX, lastY, nextX, nextY;

        while (aux[x][y] == 1) {
            x = rand() % (length / 2);
            y = rand() % (width / 2);
        }

        bool sem = true;

        nextX = x;
        nextY = y;

        while (sem) {
            int direction = rand() % 4;
            
            lastX = nextX;
            lastY = nextY;
            aux[lastX][lastY] = 1;
            result[lastX * 2][lastY * 2] = 0;

            if (direction == 0 && lastX > 0) { // north
                nextY = lastY;
                nextX = lastX - 1;

                aux[nextX][nextY] = 1;
                result[nextX * 2][nextY * 2] = 0;
                if (nextX * 2 - 1 > 0)
                    result[nextX * 2 - 1][nextY * 2] = 0;

            }
            else if (direction == 1 && lastX < a - 1) { // south
                nextY = lastY;
                nextX = lastX + 1;

                aux[nextX][nextY] = 1;
                result[nextX * 2][nextY * 2] = 0;
                if (nextX * 2 + 1 < length)
                    result[nextX * 2 + 1][nextY * 2] = 0;
            }
            else if (direction == 2 && lastY > 0) { // east
                nextY = lastY - 1;
                nextX = lastX;

                aux[nextX][nextY] = 1;
                result[nextX * 2][nextY * 2] = 0;
                if (nextY * 2 - 1 > 0)
                    result[nextX * 2][nextY * 2 - 1] = 0;
            }
            else if (direction == 3 && lastY < b - 1) { // west
                nextY = lastY + 1;
                nextX = lastX;

                aux[nextX][nextY] = 1;
                result[nextX * 2][nextY * 2] = 0;
                if (nextY * 2 + 1 < width)
                    result[nextX * 2][nextY * 2 + 1] = 0;
            }

            sem = false;
            if (x > 0 && aux[x - 1][y] == -1) {
                sem = true;
            }
            if (y > 0 && aux[x][y - 1] == -1) {
                sem = true;
            }
            if (x < a - 1 && aux[x + 1][y] == -1) {
                sem = true;
            }
            if (y < b - 1 && aux[x][y + 1] == -1) {
                sem = true;
            }
        }
    }
    return result;
}


void Tema2::Init()
{
    renderCameraTarget = true;
    pov = RADIANS(60);
    z1 = 0.01f;
    z2 = 200.0f;
    left = -10;
    right = 10;
    up = 10;
    down = -10;
    speed = 0.1f;
    o = -1;
    count = 0;
    shouldShoot = false;
    bulletSpeed = 0.5f;
    walkX = 0;
    hp = 10;
    walkY = 0;
    dir = 0;
    lastTime = clock();
    counter = 120;
    won = 0;
    angleOX = 0;
    over = 0;

    {
        Mesh* follower = new Mesh("follower");
        follower->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(follower);

        Mesh* hpBottom = new Mesh("hpbottom");
        hpBottom->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(hpBottom);

        Mesh* hpUp = new Mesh("hpUp");
        hpUp->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(hpUp);

        Mesh* hpRight = new Mesh("hpright");
        hpRight->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(hpRight);

        Mesh* hpLeft = new Mesh("hpLeft");
        hpLeft->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(hpLeft);


        Mesh* timer = new Mesh("timer");
        timer->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(timer);

        Mesh* mesh = new Mesh("character");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "character.obj");
        meshes[mesh->GetMeshID()] = mesh;

        Mesh* mesh1 = new Mesh("sphere");
        mesh1->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh1->GetMeshID()] = mesh1;
    }

    projectionMatrix = glm::perspective(pov, window->props.aspectRatio, z1, z2);

    map = GenerateMap(24, 24, 4);
    int enemyCount = 0;
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            int chance = rand() % 10;
            if (map[i][j] == 1)
                count++;
            else if (chance == 3) {
                map[i][j] = 2;
                stringstream ss;
                ss << "enemy";
                ss << enemyCount;
                string str = ss.str();
                Mesh* mesh = new Mesh(str);
                mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "enemy.obj");
                AddMeshToList(mesh);
                enemyCount++;
            }
        }
        count = 24 * 24;

        Mesh* wall = new Mesh("wall");
        wall->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        AddMeshToList(wall);

        Mesh* enemy = new Mesh("enemy");
        enemy->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "enemy.obj");
        AddMeshToList(enemy);
        enemyCount++;
    }

    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    characterAngle = 0;

    camera->MoveForward(-100.f);
    camera->TranslateRight(100.f);

    while (!playerWallCollision(camera->GetTargetPosition())) {
        camera->MoveForward(-10.f);
        camera->TranslateRight(10.f);
    }
        for (int i = 0; i < count; i++) {
            stringstream ss;
            ss << i;
            string str = ss.str();
            Mesh* mesh = new Mesh(str);
            mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
            AddMeshToList(mesh);
        }

        

        {
            Shader* shader = new Shader("ShaderTema2");
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader->CreateAndLink();
            shaders[shader->GetName()] = shader;

            Shader* shader1 = new Shader("MapShaderTema2");
            shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "mapShaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader1->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "mapShaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader1->CreateAndLink();
            shaders[shader1->GetName()] = shader1;

            Shader* shader2 = new Shader("FloorShaderTema2");
            shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "floorShaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader2->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "floorShaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader2->CreateAndLink();
            shaders[shader2->GetName()] = shader2;

            Shader* shader3 = new Shader("EnemyShaderTema2");
            shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "enemyShaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader3->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "enemyShaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader3->CreateAndLink();
            shaders[shader3->GetName()] = shader3;

            Shader* shader4 = new Shader("TimerShaderTema2");
            shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "timerShaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
            shader4->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "timerShaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
            shader4->CreateAndLink();
            shaders[shader4->GetName()] = shader4;
        }
}



void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    time = clock();
    aux = (double)(time - lastTime);
    

    if (aux > (double)CLOCKS_PER_SEC && !won && !over) {
        lastTime = time;
        counter--;
        if (counter >= 0)
            cout << "Time remaining " << counter <<  endl;

        if (counter == 0) {
            for (int i = 0; i < 24; i++) {
                for (int j = 0; j < 24; j++) {
                    map[i][j] = 0;
                }
            }

            cout << "Game over" << endl;
        }
    }



    if (renderCameraTarget)
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, 0.5f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::Scale(0.1f, 0.1f, 0.1f);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        RenderMesh(meshes["character"], shaders["ShaderTema2"], modelMatrix);
        copyMatrix = projectionMatrix;
        projectionMatrix = glm::ortho(left, right, down, up, z1, z2);


        
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::Scale((double) hp / 10, 1, (double) hp / 10);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        RenderMesh(meshes["follower"], shaders["ShaderTema2"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 0.5f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::Scale(1, 0.01f, 1);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        RenderMesh(meshes["follower"], shaders["ShaderTema2"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1.6f, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::Scale(1, 0.01f, 1);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        RenderMesh(meshes["follower"], shaders["ShaderTema2"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        modelMatrix *= transform3D::Translate(-0.5f, -0.2f, 0);
        modelMatrix *= transform3D::Scale(0.1f, 1.2f, 0.1f);
        RenderMesh(meshes["follower"], shaders["ShaderTema2"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 1, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        modelMatrix *= transform3D::Translate(0.5f, -0.2f, 0);
        modelMatrix *= transform3D::Scale(0.1f, 1.2f, 0.1f);
        RenderMesh(meshes["follower"], shaders["ShaderTema2"], modelMatrix);

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(camera->GetTargetPosition().x, camera->GetTargetPosition().y + 3, camera->GetTargetPosition().z);
        modelMatrix *= transform3D::Scale(1, (double)(1.0f - (double)(120 - counter) * (1.0f / 120)), 1);
        modelMatrix *= transform3D::RotateOY(-characterAngle);
        RenderMesh(meshes["timer"], shaders["TimerShaderTema2"], modelMatrix);

        projectionMatrix = copyMatrix;
        

    }

    if (shouldShoot && paces < 1000) {
        if (bulletSpawn) {
            bulletCoords = camera->GetTargetPosition();
            bulletAngle = -characterAngle - M_PI / 2;
            bulletSpawn = false;
        } 
        else {
            bulletCoords.x += bulletSpeed * cos(bulletAngle);
            bulletCoords.z += bulletSpeed * sin(bulletAngle);
        }
        paces++;
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(bulletCoords.x, bulletCoords.y, bulletCoords.z);
        modelMatrix *= transform3D::Scale(0.1f, 0.1f, 0.1f);
        RenderMesh(meshes["sphere"], shaders["ShaderTema2"], modelMatrix);
    }

    int s = 0;
    int enemyCount = 0;
    for (int i = 0; i < 24; i++) {
        for (int j = 0; j < 24; j++) {
            if (map[i][j] == 1) {
                stringstream ss;
                ss << s;
                string str = ss.str();
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = transform3D::Translate(i * 10.0f, 0.5f, j * 10.0f);
                modelMatrix *= transform3D::Scale(10, 10, 10);
                RenderMesh(meshes["wall"], shaders["MapShaderTema2"], modelMatrix);
                s++;
            }
            else {
                stringstream ss;
                ss << s;
                string str = ss.str();
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = transform3D::Translate(i * 10.0f, 0, j * 10.0f);
                modelMatrix *= transform3D::Scale(10, 0.1f, 10);
                RenderMesh(meshes["wall"], shaders["FloorShaderTema2"], modelMatrix);
            }
            if (map[i][j] == 2) {
                stringstream ss;
                ss << "enemy";
                ss << enemyCount;
                string str = ss.str();


                if (playerEnemyCollision((double)(i * 10.0f - walkX), (double)((j + 1) * 10.0f - walkY))) {
                    map[i][j] = 0;
                    hp--;
                    cout << "Remaining hp is " << hp * 10 << endl;

                    if (hp == 0 && !won && !over) {
                        for (int i = 0; i < 24; i++) {
                            for (int j = 0; j < 24; j++) {
                                map[i][j] = 0;
                            }
                        }
                        cout << "Game over" << endl;
                        over = 1;
                    }

                }

                if (bulletEnemyCollision((double)(i * 10.0f - walkX), (double)((j + 1) * 10.0f - walkY))) {
                    map[i][j] = -1;
                    enemyTime = 0;
                    shouldShoot = 0;
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = transform3D::Translate(i * 10.0f - walkX, 1.0f, (j + 1) * 10.0f - walkY);
                    modelMatrix *= transform3D::Scale(10, 10, 10);
                    RenderMeshWithExplosion(meshes["enemy"], shaders["EnemyShaderTema2"], modelMatrix, 1);
                }
                else {

                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = transform3D::Translate(i * 10.0f - walkX, 1.0f, (j + 1) * 10.0f - walkY);
                    modelMatrix *= transform3D::Scale(10, 10, 10);
                    RenderMeshWithExplosion(meshes["enemy"], shaders["EnemyShaderTema2"], modelMatrix, 0);
                }
                enemyCount++;
                if (walkX < 5 && dir == 0) {
                    walkX += 0.005f;
                }
                else if (walkY < 5 && dir == 0) {
                    walkY += 0.005f;
                }
                else if (dir == 0) {
                    dir = 1;
                }
                if (walkX > -5 && dir == 1) {
                    walkX -= 0.005f;
                }
                else if (walkY > -5 && dir == 1) {
                    walkY -= 0.005f;
                }
                else if (dir == 1) {
                    dir = 0;
                }

            }
            else if (map[i][j] == -1) {
                enemyCount++;
                if (enemyCount >= 5) {
                    map[i][j] = 0;
                    enemyCount = 0;
                }


                stringstream ss;
                ss << "enemy";
                ss << enemyCount;
                string str = ss.str();

                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = transform3D::Translate(i * 10.0f - walkX, 1.0f, j * 10.0f - walkY);
                modelMatrix *= transform3D::Scale(10, 10, 10);
                RenderMeshWithExplosion(meshes["enemy"], shaders["EnemyShaderTema2"], modelMatrix, 1);
            }
        }
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}



void Tema2::RenderMeshWithExplosion(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int shouldExplode)
{
    int random = rand() % 5;
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    glUniform1i(glGetUniformLocation(shader->program, "shouldExplode"), shouldExplode);
    glUniform1i(glGetUniformLocation(shader->program, "random"), random);
    mesh->Render();
}



bool m1::Tema2::playerEnemyCollision(double enemyXDif, double enemyYDif) {
    double x = camera->GetTargetPosition().x - enemyXDif;
    double y = camera->GetTargetPosition().z - enemyYDif;

    if (x < 0) {
        x *= -1;
    }

    if (y < 0) {
        y *= -1;
    }

    if (x < 3.5f && y < 3.5f) {
        return true;
    }
    return false;
}

bool m1::Tema2::bulletEnemyCollision(double enemyXDif, double enemyYDif) {
    double x = bulletCoords.x - enemyXDif;
    double y = bulletCoords.z - enemyYDif;

    if (x < 0) {
        x *= -1;
    }

    if (y < 0) {
        y *= -1;
    }

    if (x < 1.5f && y < 1.5f) {
        return true;
    }
    return false;
}

bool m1::Tema2::playerWallCollision(glm::vec3 newCoords) {
    int x = 0;
    int z = 0;
    int i;

    for (i = 0; i < 24; i++) {
        if (i * 10 + 5 >= newCoords.x && i * 10 - 5 <= newCoords.x) {
            x = i;
            break;
        }
    }

    if (i == 24 && !won && !over) {
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 24; j++) {
                map[i][j] = 0;
            }
        }
        cout << "Game won" << endl;
        won = 1;
        return true;
    }

    for (i = 0; i < 24; i++) {
        if (i * 10 + 5 >= newCoords.z && i * 10 - 5 <= newCoords.z) {
            z = i;
            break;
        }
    }

    if (i == 24 && !won && !over) {
        for (int i = 0; i < 24; i++) {
            for (int j = 0; j < 24; j++) {
                map[i][j] = 0;
            }
        }
        cout << "Game won" << endl;
        won = 1;
        return true;
    }
    z--;
    if (map[x][z] == 1) {
        return false;
    }
    return true;
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed

    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        camera->MoveForward(cameraSpeed * deltaTime * 10);
        if (!playerWallCollision(camera->GetTargetPosition())) {
            camera->MoveForward(-cameraSpeed * deltaTime * 10);
        }
            
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        camera->TranslateRight(-cameraSpeed * deltaTime * 10);
        if (!playerWallCollision(camera->GetTargetPosition())) {
            camera->TranslateRight(cameraSpeed * deltaTime * 10);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        camera->MoveForward(-cameraSpeed * deltaTime * 10);
        if (!playerWallCollision(camera->GetTargetPosition())) {
            camera->MoveForward(cameraSpeed * deltaTime * 10);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        camera->TranslateRight(cameraSpeed * deltaTime * 10);
        if (!playerWallCollision(camera->GetTargetPosition())) {
            camera->TranslateRight(-cameraSpeed * deltaTime * 10);
        }

    }
}


void Tema2::OnKeyPress(int key, int mods)
{
    
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    float sensivityOX = 0.001f;
    float sensivityOY = 0.001f;

    double x = sensivityOX * deltaX;
    double y = sensivityOY * deltaY;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT) && !renderCameraTarget) {
        renderCameraTarget = true;
    }

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        renderCameraTarget = false;
 
        camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        characterAngle += sensivityOY * -deltaX;
        angleOX += sensivityOX * -deltaY;
    }

}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT)) {
        shouldShoot = true;
        paces = 0;
        bulletSpawn = true;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
