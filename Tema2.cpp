#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <random>
#include <algorithm>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}

Mesh* Tema2::CreateCube(const std::string& name, float sideLength)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Half the side length for centering the cube at the origin
    float halfSide = sideLength / 2.0f;

    // Define vertices for a unit cube centered at the origin
    vertices = {
        VertexFormat(glm::vec3(-halfSide, -halfSide, -halfSide), glm::vec3(0, 0, 0)), // 0
        VertexFormat(glm::vec3(halfSide, -halfSide, -halfSide), glm::vec3(0, 0, 0)), // 1
        VertexFormat(glm::vec3(halfSide,  halfSide, -halfSide), glm::vec3(0, 0, 0)), // 2
        VertexFormat(glm::vec3(-halfSide,  halfSide, -halfSide), glm::vec3(0, 0, 0)), // 3
        VertexFormat(glm::vec3(-halfSide, -halfSide,  halfSide), glm::vec3(0, 0, 0)), // 4
        VertexFormat(glm::vec3(halfSide, -halfSide,  halfSide), glm::vec3(0, 0, 0)), // 5
        VertexFormat(glm::vec3(halfSide,  halfSide,  halfSide), glm::vec3(0, 0, 0)), // 6
        VertexFormat(glm::vec3(-halfSide,  halfSide,  halfSide), glm::vec3(0, 0, 0)), // 7
    };

    // Define indices for the cube (12 triangles, 6 faces)
    indices = {
        0, 1, 2,  2, 3, 0, // Front face
        1, 5, 6,  6, 2, 1, // Right face
        5, 4, 7,  7, 6, 5, // Back face
        4, 0, 3,  3, 7, 4, // Left face
        3, 2, 6,  6, 7, 3, // Top face
        4, 5, 1,  1, 0, 4  // Bottom face
    };

    // Create the cube mesh
    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* Tema2::CreateParallelepiped(const std::string& name, const glm::vec3& dimensions)
{
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Dimensions of the parallelepiped
    float halfX = dimensions.x / 2.0f;
    float halfY = dimensions.y / 2.0f;
    float halfZ = dimensions.z / 2.0f;

    // Vertices
    vertices = {
        VertexFormat(glm::vec3(-halfX, -halfY, -halfZ), glm::vec3(0, 0, 1)), // 0
        VertexFormat(glm::vec3(halfX, -halfY, -halfZ), glm::vec3(0, 1, 0)), // 1
        VertexFormat(glm::vec3(halfX,  halfY, -halfZ), glm::vec3(1, 0, 0)), // 2
        VertexFormat(glm::vec3(-halfX,  halfY, -halfZ), glm::vec3(0, 1, 1)), // 3
        VertexFormat(glm::vec3(-halfX, -halfY,  halfZ), glm::vec3(1, 0, 1)), // 4
        VertexFormat(glm::vec3(halfX, -halfY,  halfZ), glm::vec3(1, 1, 0)), // 5
        VertexFormat(glm::vec3(halfX,  halfY,  halfZ), glm::vec3(1, 1, 1)), // 6
        VertexFormat(glm::vec3(-halfX,  halfY,  halfZ), glm::vec3(0, 0, 0)), // 7
    };

    // Indices for the 12 triangles (6 faces)
    indices = {
        0, 1, 2,  2, 3, 0,  // Front face
        1, 5, 6,  6, 2, 1,  // Right face
        5, 4, 7,  7, 6, 5,  // Back face
        4, 0, 3,  3, 7, 4,  // Left face
        3, 2, 6,  6, 7, 3,  // Top face
        4, 5, 1,  1, 0, 4   // Bottom face
    };

    // Create and return the mesh
    return CreateMesh(name.c_str(), vertices, indices);
}

Mesh* Tema2::CreateGateFrameMesh(const std::string& name, float width, float height) {
    // Poarta este centrată la origine, vom crea un contur dreptunghiular în planul XZ.
    // width și height sunt dimensiunile porții, o considerăm orientată pe X și Y (sau X și Z, în funcție de cum e gateMesh-ul)
    // Presupunem că poarta e pe X (lățime) și Y (înălțime), grosime mică pe Z.
    // Conturul va fi un dreptunghi subțire.
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;

    std::vector<VertexFormat> vertices = {
        VertexFormat(glm::vec3(-halfW, -halfH, 0)), // colț stânga-jos
        VertexFormat(glm::vec3(halfW, -halfH, 0)), // colț dreapta-jos
        VertexFormat(glm::vec3(halfW,  halfH, 0)), // colț dreapta-sus
        VertexFormat(glm::vec3(-halfW,  halfH, 0))  // colț stânga-sus
    };

    // Indicii pentru linii: vom desena un dreptunghi
    std::vector<unsigned int> indices = {
        0, 1,
        1, 2,
        2, 3,
        3, 0
    };

    Mesh* frameMesh = new Mesh(name);
    frameMesh->InitFromData(vertices, indices);
    frameMesh->SetDrawMode(GL_LINES); // Important, desenăm linii, nu triunghiuri
    return frameMesh;
}

void Tema2::RenderDrone(const glm::vec3& position, const glm::vec3& scale, float rotorRotation, float rotationY)
{
    glm::mat4 droneMatrix = glm::mat4(1);
    droneMatrix = glm::translate(droneMatrix, position);
    droneMatrix = glm::rotate(droneMatrix, rotationY, glm::vec3(0, 1, 0));
    droneMatrix = glm::scale(droneMatrix, scale);

    // ** Corpul dronă **
    // Prima paralelipipedă (stick-ul orizontal)
    glm::mat4 bodyMatrix = glm::translate(droneMatrix, glm::vec3(0, 0.1f, 0)); // Ridicăm deasupra solului
    glm::mat4 bodyHorizontal = glm::scale(bodyMatrix, glm::vec3(0.15f, 0.5f, 0.75f)); // Dimensiuni orizontale
    glUseProgram(shaders["LabShader"]->GetProgramID());
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 1.0f, 0.874f, 0.0f); // Black
    RenderMesh(meshes["parallelepiped"], shaders["LabShader"], bodyHorizontal);

    // A doua paralelipipedă (stick-ul vertical, rotit la 90°)
    glm::mat4 bodyVertical = glm::rotate(glm::mat4(1), glm::radians(90.0f), glm::vec3(0, 1, 0)); // Rotație pe Y
    bodyVertical = glm::translate(bodyVertical, glm::vec3(0, 0.1f, 0));                          // Ridicăm deasupra solului
    bodyVertical = glm::scale(bodyVertical, glm::vec3(0.15f, 0.5f, 0.75f));                      // Dimensiuni verticale
    bodyVertical = droneMatrix * bodyVertical;                                                  // Aplicăm pe matricea dronei
    glUseProgram(shaders["LabShader"]->GetProgramID());
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 1.0f, 0.874f, 0.0f); // Black
    RenderMesh(meshes["parallelepiped"], shaders["LabShader"], bodyVertical);

    // ** Rotori **
    for (int i = 0; i < 4; i++) {
        // Poziționarea rotorului pe X/Y
        float angle = glm::radians(i * 90.0f); // 90° între rotori
        float xOffset = 0.52f * cos(angle);    // Distanța pe X față de centru
        float zOffset = 0.52f * sin(angle);    // Distanța pe Z față de centru

        // Cubul de bază al rotorului
        glm::mat4 rotorBaseCubeMatrix = glm::translate(droneMatrix, glm::vec3(xOffset, 0.16f, zOffset)); // Ridicat față de corp
        rotorBaseCubeMatrix = glm::scale(rotorBaseCubeMatrix, glm::vec3(0.08f));                        // Dimensiuni cub
        glUseProgram(shaders["LabShader"]->GetProgramID());
        glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.5f, 0.5f, 0.5f);
        RenderMesh(meshes["cube"], shaders["LabShader"], rotorBaseCubeMatrix);

        // Lama rotorului (rotită)
        glm::mat4 rotorBladeMatrix = glm::translate(rotorBaseCubeMatrix, glm::vec3(0, 0.5f, 0)); // Ridicat peste cub
        rotorBladeMatrix = glm::rotate(rotorBladeMatrix, rotorRotation, glm::vec3(0, 1, 0));     // Rotație pe Y
        rotorBladeMatrix = glm::scale(rotorBladeMatrix, glm::vec3(8.0f, 1.0f, 0.15f));          // Dimensiuni lamă
        glUseProgram(shaders["LabShader"]->GetProgramID());
        glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 1.0f, 0.0f, 0.0f); // Black
        RenderMesh(meshes["parallelepiped"], shaders["LabShader"], rotorBladeMatrix);
    }
}

void Tema2::RenderTree(const glm::vec3& position, float scale) {
    // Matrice de bază pentru copac
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(scale));

    // Desenăm trunchiul (cilindrul)
    glm::mat4 trunkMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5f, 0)); // ridicăm puțin cilindrul
    trunkMatrix = glm::scale(trunkMatrix, glm::vec3(0.1f, 0.5f, 0.1f)); // trunchi subțire și înalt
    glUseProgram(shaders["LabShader"]->GetProgramID());
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.6f, 0.3f, 0.0f);
    RenderMesh(meshes["cylinder"], shaders["LabShader"], trunkMatrix);

    // Conul inferior (coroana de jos)
    glm::mat4 lowerConeMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.3f, 0));
    lowerConeMatrix = glm::scale(lowerConeMatrix, glm::vec3(0.5f, 0.5f, 0.5f));
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.0f, 0.6f, 0.0f);
    RenderMesh(meshes["cone"], shaders["LabShader"], lowerConeMatrix);

    // Conul superior (coroana de sus)
    glm::mat4 upperConeMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.8f, 0));
    upperConeMatrix = glm::scale(upperConeMatrix, glm::vec3(0.3f, 0.4f, 0.3f));
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.0f, 0.7f, 0.0f);
    RenderMesh(meshes["cone"], shaders["LabShader"], upperConeMatrix);
}

void Tema2::RenderObstacle(const glm::vec3& position, const glm::vec3& dimensions) {
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, dimensions);

    // Setăm culoarea obstacolului, de exemplu un maro-roșcat
    glUseProgram(shaders["LabShader"]->GetProgramID());
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.6f, 0.2f, 0.2f);

    RenderMesh(meshes["parallelepiped"], shaders["LabShader"], modelMatrix);
}

bool Tema2::IsPositionValid(const glm::vec3& pos, const std::vector<glm::vec3>& positions, float minDistance) {
    for (auto& p : positions) {
        float dist = glm::distance(pos, p);
        if (dist < minDistance) {
            return false;
        }
    }
    return true;
}

bool Tema2::CheckCollisionWithGround(const glm::vec3& dronePos) {
    // Drona nu trebuie să coboare sub sol
    if (dronePos.y - droneRadius < groundLevel) {
        return true;
    }
    return false;
}

bool Tema2::CheckCollisionWithObstacles(const glm::vec3& dronePos) {
    // Parcurgem vectorul objectPositions
    // Obiectele la index par = copac, impar = clădire
    for (int i = 0; i < (int)objectPositions.size(); i++) {
        glm::vec3 obstaclePos = objectPositions[i];
        float dist = glm::distance(dronePos, obstaclePos);

        float obstacleRadius = (i % 2 == 0) ? obstacleTreeRadius : obstacleBuildingRadius;

        if (dist < (droneRadius + obstacleRadius)) {
            return true; // coliziune detectată
        }
    }

    return false;
}

bool Tema2::CheckCollisionWithCheckpoint(const glm::vec3& dronePos) {
    // Dacă aveți checkpoint-uri, puteți face un test similar
    // Exemplu: 
    // glm::vec3 checkpointPos = ...;
    // float checkpointRadius = ...;
    // if (glm::distance(dronePos, checkpointPos) < (droneRadius + checkpointRadius)) {
    //     return true; // ați lovit checkpoint-ul (poate înseamnă că ați trecut prin el)
    // }
    return false;
}

struct Gate {
	glm::vec3 position;
	float height;
    float width;
	bool passed;
};
std::vector<Gate> gates;
int currentGateIndex = 0;
std::vector<int> gateOrder; // contine indexii in ordinea in care porțile trebuie parcurse

void Tema2::RenderGates() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    for (int i = 0; i < (int)gates.size(); i++) {
        glm::mat4 gateModel = glm::mat4(1);
        gateModel = glm::translate(gateModel, gates[i].position);

        glUseProgram(shaders["GateShader"]->program);

        GLint loc_model = glGetUniformLocation(shaders["GateShader"]->program, "Model");
        GLint loc_view = glGetUniformLocation(shaders["GateShader"]->program, "View");
        GLint loc_projection = glGetUniformLocation(shaders["GateShader"]->program, "Projection");

        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

        // Mai întâi desenăm conturul negru
        {
            // Poarta are width=gates[i].width, height=gates[i].height
            // Scalăm conturul la dimensiunea porții
            glm::mat4 frameModel = gateModel;
            frameModel = glm::scale(frameModel, glm::vec3(gates[i].width, gates[i].height, 1.0f));

            glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(frameModel));

            // Conturul negru complet opac
            glUniform3f(glGetUniformLocation(shaders["GateShader"]->program, "objectColor"), 0.0f, 0.0f, 0.0f);
            glUniform1f(glGetUniformLocation(shaders["GateShader"]->program, "alpha"), 1.0f);
			glLineWidth(3);
            RenderMesh(meshes["gateFrame"], shaders["GateShader"], frameModel);
            glLineWidth(1);
        }

        // Apoi desenăm poarta propriu-zisă
        {
            glm::mat4 gateInner = gateModel;
            gateInner = glm::scale(gateInner, glm::vec3(gates[i].width, gates[i].height, 0.2f));
            glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(gateInner));

            if (currentGateIndex < (int)gates.size() && i == gateOrder[currentGateIndex] && !gates[i].passed) {
                glUniform3f(glGetUniformLocation(shaders["GateShader"]->program, "objectColor"), 0.0f, 1.0f, 0.0f);
                glUniform1f(glGetUniformLocation(shaders["GateShader"]->program, "alpha"), 0.5f);
            }
            else {
                glUniform3f(glGetUniformLocation(shaders["GateShader"]->program, "objectColor"), 1.0f, 0.0f, 0.0f);
                glUniform1f(glGetUniformLocation(shaders["GateShader"]->program, "alpha"), 0.3f);
            }

            RenderMesh(meshes["gateMesh"], shaders["GateShader"], gateInner);
        }
    }

    glDisable(GL_BLEND);
}

struct Cloud {
    glm::vec3 position;
    float speed; // viteză de deplasare pe axa X
};

// Declarați un vector global de nori
std::vector<Cloud> clouds;

void Tema2::RenderClouds() {
    glUseProgram(shaders["LabShader"]->GetProgramID());
    // setăm culoarea norilor la alb
    glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 1.0f, 1.0f, 1.0f);

    for (auto& c : clouds) {
        // Sfera principală (centru)
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, c.position);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(2.0f)); // sfera principală mai mare
        RenderMesh(meshes["sphere"], shaders["LabShader"], modelMatrix);

        // Sfera stânga
        glm::mat4 leftSphere = glm::mat4(1);
        leftSphere = glm::translate(leftSphere, c.position + glm::vec3(-1.0f, 0.0f, 0.0f));
        leftSphere = glm::scale(leftSphere, glm::vec3(1.5f));
        RenderMesh(meshes["sphere"], shaders["LabShader"], leftSphere);

        // Sfera dreapta
        glm::mat4 rightSphere = glm::mat4(1);
        rightSphere = glm::translate(rightSphere, c.position + glm::vec3(1.0f, 0.0f, 0.0f));
        rightSphere = glm::scale(rightSphere, glm::vec3(1.5f));
        RenderMesh(meshes["sphere"], shaders["LabShader"], rightSphere);
    }
}
void Tema2::RenderMiniMap() {
    minimapCamera->SetPositionAndRotation(glm::vec3(dronePosition.x, dronePosition.y + 100.0f, dronePosition.z), minimapRotation);
    GetSceneCamera()->SetPositionAndRotation(glm::vec3(dronePosition.x, dronePosition.y + 4.0f, dronePosition.z), glm::quatLookAt(glm::vec3(0, -1, 0), glm::vec3(0, 0, 1)));
    glm::mat4 miniMapView = minimapCamera->GetViewMatrix();
    glm::mat4 miniMapProj = minimapCamera->GetProjectionMatrix();

    glm::ivec2 resolution = window->GetResolution();
    int miniMapWidth = resolution.x / 4;
    int miniMapHeight = resolution.y / 4;
    //Setaza camaera astfel incat sa se vada de sus spre teren si sa fie centrata pe drona
    thirdPersonCamera->SetPositionAndRotation(glm::vec3(dronePosition.x, dronePosition.y + 4.0f, dronePosition.z), glm::vec3(0, -droneRotationY, 0));

    // Setăm viewport în colțul dreapta-sus
    glViewport(resolution.x - miniMapWidth, resolution.y - miniMapHeight, miniMapWidth, miniMapHeight);
    glClear(GL_DEPTH_BUFFER_BIT);

    // Desenăm terenul ca verde simplu
    {
        glm::mat4 modelMatrix = glm::mat4(1);
        glUseProgram(shaders["LabShader"]->program);
        GLint loc_model = glGetUniformLocation(shaders["LabShader"]->program, "Model");
        GLint loc_view = glGetUniformLocation(shaders["LabShader"]->program, "View");
        GLint loc_projection = glGetUniformLocation(shaders["LabShader"]->program, "Projection");

        glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(miniMapView));
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(miniMapProj));

        glUniform3f(glGetUniformLocation(shaders["LabShader"]->program, "objectColor"), 0.0f, 1.0f, 0.0f);
        RenderMesh(meshes["terrain"], shaders["LabShader"], modelMatrix);
    }

    // Drona ca pătrat galben
    {
        glm::mat4 droneMarker = glm::mat4(1);
        droneMarker = glm::translate(droneMarker, dronePosition);
        droneMarker = glm::scale(droneMarker, glm::vec3(0.5f, 0.01f, 0.5f));

        glUseProgram(shaders["LabShader"]->program);
        GLint loc_model = glGetUniformLocation(shaders["LabShader"]->program, "Model");
        GLint loc_view = glGetUniformLocation(shaders["LabShader"]->program, "View");
        GLint loc_projection = glGetUniformLocation(shaders["LabShader"]->program, "Projection");

        glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(droneMarker));
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(miniMapView));
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(miniMapProj));

        glUniform3f(glGetUniformLocation(shaders["LabShader"]->program, "objectColor"), 1.0f, 1.0f, 0.0f);
        RenderMesh(meshes["cube"], shaders["LabShader"], droneMarker);
    }

    // Porți ca discuri albastre (folosim sphere aplatizată)
    for (int i = 0; i < (int)gates.size(); i++) {
        glm::mat4 gateMarker = glm::mat4(1);
        gateMarker = glm::translate(gateMarker, gates[i].position);
        gateMarker = glm::scale(gateMarker, glm::vec3(1.0f, 0.01f, 1.0f));

        glUseProgram(shaders["LabShader"]->program);
        GLint loc_model = glGetUniformLocation(shaders["LabShader"]->program, "Model");
        GLint loc_view = glGetUniformLocation(shaders["LabShader"]->program, "View");
        GLint loc_projection = glGetUniformLocation(shaders["LabShader"]->program, "Projection");

        glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(gateMarker));
        glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(miniMapView));
        glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(miniMapProj));

        glUniform3f(glGetUniformLocation(shaders["LabShader"]->program, "objectColor"), 0.0f, 0.0f, 1.0f);
        RenderMesh(meshes["sphere"], shaders["LabShader"], gateMarker);
    }

    // Clădiri maro
    for (int i = 0; i < (int)objectPositions.size(); i++) {
        if (i % 2 == 1) {
            glm::mat4 buildingMarker = glm::mat4(1);
            buildingMarker = glm::translate(buildingMarker, objectPositions[i]);
            buildingMarker = glm::scale(buildingMarker, glm::vec3(1.0f, 0.01f, 1.0f));

            glUseProgram(shaders["LabShader"]->program);
            GLint loc_model = glGetUniformLocation(shaders["LabShader"]->program, "Model");
            GLint loc_view = glGetUniformLocation(shaders["LabShader"]->program, "View");
            GLint loc_projection = glGetUniformLocation(shaders["LabShader"]->program, "Projection");

            glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(buildingMarker));
            glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(miniMapView));
            glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(miniMapProj));

            glUniform3f(glGetUniformLocation(shaders["LabShader"]->program, "objectColor"), 0.6f, 0.3f, 0.0f);
            RenderMesh(meshes["cube"], shaders["LabShader"], buildingMarker);
        }
    }
    // Copaci verzi
    for (int i = 0; i < (int)objectPositions.size(); i++) {
        if (i % 2 == 0) {
            glm::mat4 treeMarker = glm::mat4(1);
            treeMarker = glm::translate(treeMarker, objectPositions[i]);
            treeMarker = glm::scale(treeMarker, glm::vec3(1.0f, 0.01f, 1.0f));
            glUseProgram(shaders["LabShader"]->program);
            GLint loc_model = glGetUniformLocation(shaders["LabShader"]->program, "Model");
            GLint loc_view = glGetUniformLocation(shaders["LabShader"]->program, "View");
            GLint loc_projection = glGetUniformLocation(shaders["LabShader"]->program, "Projection");
            glUniformMatrix4fv(loc_model, 1, GL_FALSE, glm::value_ptr(treeMarker));
            glUniformMatrix4fv(loc_view, 1, GL_FALSE, glm::value_ptr(miniMapView));
            glUniformMatrix4fv(loc_projection, 1, GL_FALSE, glm::value_ptr(miniMapProj));
            glUniform3f(glGetUniformLocation(shaders["LabShader"]->program, "objectColor"), 0.0f, 0.6f, 0.0f);
            RenderMesh(meshes["cube"], shaders["LabShader"], treeMarker);
        }
    }

    // Restaurăm viewport-ul original
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Init()
{
    // Dimensiuni grid (m x n)
    int m = 100; // numărul de celule pe lățime
    int n = 100; // numărul de celule pe lungime
    float cellSize = 1.0f; // dimensiunea unei celule

    // Calculăm offsetul pentru a centra terenul
    float offsetX = -m * cellSize / 2.0f;
    float offsetZ = -n * cellSize / 2.0f;

    // Listă de vertecși și indici
    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    // Generarea vertecșilor
    for (int i = 0; i <= m; ++i) {
        for (int j = 0; j <= n; ++j) {
            float x = i * cellSize + offsetX;
            float z = j * cellSize + offsetZ;
            vertices.emplace_back(glm::vec3(x, 0, z), glm::vec3(0, 1, 0), glm::vec3(1, 1, 1));
        }
    }

    // Generarea indicilor pentru triunghiuri
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            unsigned int topLeft = i * (n + 1) + j;
            unsigned int topRight = topLeft + 1;
            unsigned int bottomLeft = (i + 1) * (n + 1) + j;
            unsigned int bottomRight = bottomLeft + 1;

            // Triunghiul 1
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Triunghiul 2
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Crearea mesh-ului
    CreateMesh("terrain", vertices, indices);

    // Adăugăm un shader pentru teren
    Shader* terrainShader = new Shader("TerrainShader");
    terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TerrainShader.glsl"), GL_VERTEX_SHADER);
    terrainShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "TerrainFragmentShader.glsl"), GL_FRAGMENT_SHADER);
    terrainShader->CreateAndLink();
    shaders[terrainShader->GetName()] = terrainShader;

    Shader* labShader = new Shader("LabShader");
    labShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    labShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    labShader->CreateAndLink();
    shaders["LabShader"] = labShader;

    // Create a parallelepiped (body of the drone)
    Mesh* parallelepiped = CreateParallelepiped("parallelepiped", glm::vec3(0.5f, 0.1f, 1.5f));
    meshes["parallelepiped"] = parallelepiped;

    Mesh* cube = CreateCube("cube", 1.0f); // Cube with side length 1.0
    meshes["cube"] = cube;

    Mesh* gateMesh = CreateParallelepiped("gateMesh", glm::vec3(1.0f, 1.0f, 0.1f));
    meshes["gateMesh"] = gateMesh;

    Mesh* gateFrameMesh = CreateGateFrameMesh("gateFrame", 1.5f, 1.5f);
    meshes["gateFrame"] = gateFrameMesh;

    dronePosition = glm::vec3(0, 3, 0); // la înălțime 1, în centru
    droneRotationY = 0.0f;              // fără rotație inițială
    droneSpeed = 5.0f;                  // viteza de deplasare
	droneRotationSpeed = 2.0f;		  // viteza de rotație
    {
        Mesh* mesh = new Mesh("cone");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
    {
        Mesh* mesh = new Mesh("cylinder");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cylinder.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }
	//pozitionare copaci
    int totalObjects = 70; // 10 copaci + 10 clădiri, de exemplu
    float minDistance = 5.0f; // minimă distanță între obiecte, pentru a evita suprapunerea
    srand((unsigned int)time(NULL));

    for (int i = 0; i < totalObjects; i++) {
        glm::vec3 newPos;
        bool foundValidPosition = false;
        // Încerci de câteva ori să găsești o poziție validă (fără suprapunere)
        for (int attempts = 0; attempts < 100; attempts++) {
            float x = (rand() % 100) - 50.0f;
            float z = (rand() % 100) - 50.0f;
            newPos = glm::vec3(x, 0, z);

            if (IsPositionValid(newPos, objectPositions, minDistance)) {
                foundValidPosition = true;
                break;
            }
        }

        if (foundValidPosition) {
            objectPositions.push_back(newPos);
        }
        else {
            // Dacă nu a găsit în 100 de încercări, pur și simplu nu adăugăm obiectul (foarte puțin probabil)
        }
    }

    {
        Shader* gateShader = new Shader("GateShader");
        gateShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "GateVertexShader.glsl"), GL_VERTEX_SHADER);
        gateShader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "GateFragmentShader.glsl"), GL_FRAGMENT_SHADER);
        gateShader->CreateAndLink();
        shaders[gateShader->GetName()] = gateShader;
    }

    int totalGates = 5;
	srand((unsigned int)time(NULL));

    std::vector<glm::vec3> usedPositions = objectPositions;
    for (int i = 0; i < totalGates; i++) {
        glm::vec3 newPos;
        bool foundValidPosition = false;

        // Încerci să găsești o poziție validă pentru poartă
        for (int attempts = 0; attempts < 100; attempts++) {
            float x = (rand() % 100) - 50.0f;
            float z = (rand() % 100) - 50.0f;
            //y sa fie random intre 2 si 6
			float y = (rand() % 4) + 2.0f;

            newPos = glm::vec3(x, y, z);

            // Verificăm dacă poziția este validă, fără suprapunere
            // Dacă IsPositionValid verifică doar distanțe față de objectPositions,
            // atunci trebuie să i le dăm și pe cele ale porților deja plasate.
            // De aceea am copiat totul în usedPositions. 
            if (IsPositionValid(newPos, usedPositions, minDistance)) {
                foundValidPosition = true;
                break;
            }
        }

        if (foundValidPosition) {
            Gate g;
            g.position = newPos;
            g.width = 1.5f;   // lățimea porții
            g.height = 2.0f;  // înălțimea porții
            g.passed = false;
            gates.push_back(g);

            // Adăugăm poziția porții la usedPositions pentru a evita suprapunerea cu următoarele porți
            usedPositions.push_back(newPos);
        }
        else {
            // Nu am găsit o poziție validă, putem fie să nu adăugăm poarta,
            // fie să micșorăm numărul total de porți.
            // De dragul exemplului, nu adăugăm poarta.
        }
    }
    // Acum avem un vector gates cu porți generate.
    // Stabilim ordinea parcurgerii porților.

    // gateOrder va conține indexii 0, 1, 2, ... totalGates - 1
    for (int i = 0; i < (int)gates.size(); i++) {
        gateOrder.push_back(i);
    }

    // Amestecăm ordinea dacă dorim o ordine aleatoare
    // Alternativ, lăsăm ordinea prestabilită.
    unsigned seed = (unsigned)std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(gateOrder.begin(), gateOrder.end(), mt19937(seed));

    {
        Mesh* sphereMesh = new Mesh("sphere");
        sphereMesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[sphereMesh->GetMeshID()] = sphereMesh;
    }

    int nrClouds = 20;
    for (int i = 0; i < nrClouds; i++) {
        Cloud c;
        c.position = glm::vec3((rand() % 100 - 50), 20.0f, (rand() % 100 - 50)); // între -50 și 50 pe X și Z, sus pe Y=20
        c.speed = 0.5f + (rand() % 10) / 10.0f; // viteză între 0.5 și 1.5
        clouds.push_back(c);
    }

    glm::ivec2 resolution = window->GetResolution();
    textRenderer = new gfxc::TextRenderer(window->props.selfDir, resolution.x, resolution.y);

    // Apoi încărcați un font
    textRenderer->Load(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::FONTS, "Hack-Bold.ttf"), 24);

    {
        Mesh* arrow = new Mesh("arrow");
        arrow->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "Arrow5.obj");
        meshes[arrow->GetMeshID()] = arrow;
    }
    //camera minimapa
    minimapCamera = new cam::Camera();
	//initiliazare camerarotation
    minimapRotation = glm::vec3(0, 0, 0);

	//camera third person
	thirdPersonCamera = new cam::Camera();
}


Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Culoare albastru cer
    glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    // ---------------------- Scena principală ----------------------
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["terrain"], shaders["TerrainShader"], modelMatrix);

    float rotorRotation = Engine::GetElapsedTime() * 5.0f;

    // Calculăm forward-ul dronei pentru camera principală
    glm::vec3 forward = glm::vec3(sin(droneRotationY), 0, -cos(droneRotationY));
    glm::vec3 up = glm::vec3(0, 1, 0);

    float distanceBehind = 3.0f;
    float heightAbove = 0.5f;

    glm::vec3 cameraPosition = dronePosition - forward * distanceBehind + glm::vec3(0, heightAbove, 0);
    glm::vec3 cameraTarget = dronePosition + forward * 2.0f;
	glm::vec3 cameraRotation = glm::vec3(0, -droneRotationY, 0);

    thirdPersonCamera->Set(cameraPosition, dronePosition, cameraTarget);
	GetSceneCamera()->SetPositionAndRotation(cameraPosition, cameraRotation);

    // Randăm drona în scena principală
    RenderDrone(dronePosition, glm::vec3(1.0f), rotorRotation, droneRotationY);

    // Randăm obstacole (copaci și clădiri) în scena principală
    for (int i = 0; i < (int)objectPositions.size(); i++) {
        if (i % 2 == 0) {
            // copac
            RenderTree(objectPositions[i], 1.0f);
        }
        else {
            // clădire
            RenderObstacle(objectPositions[i], glm::vec3(10.0f, 40.0f, 1.0f));
        }
    }

    // Randăm porțile în scena principală
    RenderGates();

    // Verificăm trecerea prin poarta curentă și pornim/oprim cronometrul dacă este cazul
    if (currentGateIndex < (int)gates.size()) {
        int cGate = gateOrder[currentGateIndex];
        Gate& g = gates[cGate];
        if (!g.passed) {
            float dx = fabs(dronePosition.x - g.position.x);
            float dy = fabs(dronePosition.y - g.position.y);
            float dz = fabs(dronePosition.z - g.position.z);

            if (dx < g.width / 2.0f && dy < g.height / 2.0f && dz < 1.0f) {
                g.passed = true;
                currentGateIndex++;

                // Daca tocmai am trecut de prima poarta, pornim cronometrul
                if (currentGateIndex == 1 && !timerStarted) {
                    timerStarted = true;
                    timerStopped = false;
                    startTime = Engine::GetElapsedTime();
                }

                // Daca am trecut de toate portile, oprim cronometrul
                if (currentGateIndex >= (int)gates.size() && timerStarted && !timerStopped) {
                    finalTime = Engine::GetElapsedTime() - startTime;
                    timerStopped = true;
                    std::cout << "Ati terminat cursa in " << finalTime << " secunde!\n";
                }
            }
        }
    }
    if (currentGateIndex < (int)gates.size()) {
        // Următoarea poartă
        int nextGateIndex = gateOrder[currentGateIndex];
        glm::vec3 gatePos = gates[nextGateIndex].position;

        // Vector direcție din dronă spre poartă (pe orizontală)
        glm::vec3 direction = gatePos - dronePosition;

        // Ne asigurăm că vectorul e în plan orizontal (y=0)
        direction.y = 0;

        // Calculăm unghiul săgeții pe baza vectorului direcție
        // atan2 ia (y, x), dar noi avem direcție pe (x,z)
        float angle = atan2(-direction.z, direction.x);

        // Dacă direction este (0,0,0) (poate la final?), prevenim erorile
        if (glm::length(direction) < 0.001f) {
            // Dacă suntem deja la ultimă poartă sau foarte aproape, săgeata poate fi orientată oricum
            angle = 0.0f;
        }

        // Poziționarea săgeții sub dronă:
        // Puteți plasa săgeata la o înălțime un pic sub dronă, de ex. dronePosition.y - 0.5f,
        // și la aceeași poziție pe XZ ca drona.
        glm::vec3 arrowPos = glm::vec3(dronePosition.x, dronePosition.y - 0.5f, dronePosition.z);

        // Cream matricea de model pentru săgeată
        glm::mat4 arrowModel = glm::mat4(1);
        arrowModel = glm::translate(arrowModel, arrowPos);
        arrowModel = glm::rotate(arrowModel, angle, glm::vec3(0, 1, 0));
        arrowModel = glm::scale(arrowModel, glm::vec3(0.025f)); // scalați după preferințe

        // Renduim săgeata:
        glUseProgram(shaders["LabShader"]->GetProgramID());
        glUniform3f(glGetUniformLocation(shaders["LabShader"]->GetProgramID(), "objectColor"), 0.7f, 0.0f, 1.0f);
        RenderMesh(meshes["arrow"], shaders["LabShader"], arrowModel);
    }
	//Randeaza norii
	RenderClouds();

    RenderMiniMap();
	GetSceneCamera()->SetPositionAndRotation(cameraPosition, cameraRotation);   
    //Restauram camera principala
    //thirdPersonCamera->SetPositionAndRotation(cameraPosition, cameraRotation);

    // Afisare timp curent daca timer-ul a pornit
    if (timerStarted && !timerStopped) {
        float currentTime = Engine::GetElapsedTime() - startTime;
        textRenderer->RenderText("Time: " + std::to_string(currentTime), 10, 10, 1.0f, glm::vec3(1, 1, 1));
    }

    if (timerStopped) {
        textRenderer->RenderText("Final Time: " + std::to_string(finalTime), 10, 50, 1.0f, glm::vec3(1, 1, 0));
    }
}


void Tema2::FrameEnd()
{
    
}


void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // TODO(student): Get shader location for uniform mat4 "Model"
	auto modelLocation = glGetUniformLocation(shader->program, "Model");

    // TODO(student): Set shader uniform "Model" to modelMatrix
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // TODO(student): Get shader location for uniform mat4 "View"
	auto viewLocation = glGetUniformLocation(shader->program, "View");

    // TODO(student): Set shader uniform "View" to viewMatrix
	glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();

    // TODO(student): Get shader location for uniform mat4 "Projection"
	auto projectionLocation = glGetUniformLocation(shader->program, "Projection");

    // TODO(student): Set shader uniform "Projection" to projectionMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();

    float elapsedTime = Engine::GetElapsedTime();
	glUniform1f(glGetUniformLocation(shader->program, "Time"), elapsedTime);

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);

	
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    glm::vec3 oldPosition = dronePosition; // salvăm poziția veche a dronei

    // Vectorii direcție
    glm::vec3 forward = glm::vec3(sin(droneRotationY), 0, -cos(droneRotationY));
    glm::vec3 right = glm::vec3(cos(droneRotationY), 0, sin(droneRotationY));
    glm::vec3 up = glm::vec3(0, 1, 0);

    // Actualizăm poziția dronei pe baza inputului
    if (window->KeyHold(GLFW_KEY_W)) {
        dronePosition += forward * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        dronePosition -= forward * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        dronePosition -= right * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_D)) {
        dronePosition += right * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_R)) {
        dronePosition += up * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_F)) {
        dronePosition -= up * droneSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_Q)) {
        droneRotationY -= droneRotationSpeed * deltaTime;
    }
    if (window->KeyHold(GLFW_KEY_E)) {
        droneRotationY += droneRotationSpeed * deltaTime;
    }

    // După ce am modificat poziția, verificăm coliziunile cu sol, obstacole și margini
    bool collisionGround = CheckCollisionWithGround(dronePosition);
    bool collisionObstacles = CheckCollisionWithObstacles(dronePosition);
    bool collisionCheckpoint = CheckCollisionWithCheckpoint(dronePosition);

    // Acum verificăm limitele hărții și înălțimea
    float mapMinX = -50.0f;
    float mapMaxX = 50.0f;
    float mapMinZ = -50.0f;
    float mapMaxZ = 50.0f;

    float minHeight = groundLevel + droneRadius;
    float maxHeight = 50.0f;

    // Verificare margini hartă
    if (dronePosition.x < mapMinX || dronePosition.x > mapMaxX ||
        dronePosition.z < mapMinZ || dronePosition.z > mapMaxZ) {
        dronePosition = oldPosition; // revenim la poziția veche
    }

    // Verificare limite de înălțime
    if (dronePosition.y < minHeight || dronePosition.y > maxHeight) {
        dronePosition = oldPosition; // revenim la poziția veche
    }

    // Dacă apar coliziuni cu sol sau obstacole, de asemenea revenim la poziția veche
    if (collisionGround || collisionObstacles) {
        dronePosition = oldPosition;

        // Ajustăm pentru sol: dacă e sub sol, forțăm y = minHeight
        if (collisionGround && dronePosition.y < minHeight) {
            dronePosition.y = minHeight;
        }
    }

    // Dacă coliziunea cu checkpoint înseamnă colectare, tratați evenimentul
    if (collisionCheckpoint) {
        // ... de ex. scor++
    }
}






void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
		if (key == GLFW_KEY_LEFT_SHIFT) {
			droneSpeed = 10.0f;
		}
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_LEFT_SHIFT) {
        droneSpeed = 5.0f;
    }
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
