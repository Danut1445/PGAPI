#include "lab_m2/Tema1/Tema1.h"

#include <vector>
#include <iostream>
#include <limits>
#include "stb/stb_image.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */
struct ParticleTema1
{
    glm::vec4 position;
    glm::vec4 speed;
    glm::vec4 initialPos;
    glm::vec4 initialSpeed;
    float delay;
    float initialDelay;
    float lifetime;
    float initialLifetime;

    ParticleTema1() {}

    ParticleTema1(const glm::vec4& pos, const glm::vec4& speed)
    {
        SetInitial(pos, speed);
    }

    void SetInitial(const glm::vec4& pos, const glm::vec4& speed,
        float delay = 0, float lifetime = 0)
    {
        position = pos;
        initialPos = pos;

        this->speed = speed;
        initialSpeed = speed;

        this->delay = (rand() % 100 - 50.0f) / 100.0f;
        initialDelay = (rand() % 100 - 50.0f) / 100.0f;

        this->lifetime = lifetime;
        initialLifetime = (rand() % 100 - 50.0f) / 100.0f;
    }
};


ParticleEffect<ParticleTema1>* particleEffectTema1;



Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    //Initialize main camera:
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-40 * TO_RADIANS, 0, 0)));
    camera->Update();

    //Initialize second camera for fireworks:
    camera_firewroks = new gfxc::Camera();
    camera_firewroks->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-15 * TO_RADIANS, 0, 0)));
    camera_firewroks->SetPerspective(60, window->props.aspectRatio, 0.01f, 200);

    ToggleGroundPlane();

    // Create a shader program for surface generation
    {
        Shader *shader = new Shader("TableSide");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTableSide.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderTexture.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("NormalSide");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderTableSideNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ShadowSide");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTableSide.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionSideP1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderTableSideCubemapP1.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionSideP2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderTableSideCubemapP2.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionSideP3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderTableSideCubemapP3.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TableTop");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTableTop.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderTexture.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("NormalTop");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderTableTopNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ShadowTop");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTableTop.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionTop");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderTableTopCubemap.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Rotation");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderRotation.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderTexture.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("NormalRotation");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderRotationNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ShadowRotation");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderRotation.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionRotationP1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRotationCubemapP1.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionRotationP2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRotationCubemapP2.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ReflectionRotationP3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRotationCubemapP3.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TVScreen");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTVScreen.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderScreen.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ScreenNormal");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderTVScreenNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TVBody");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTVBody.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderTexture.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("TVBodyShadow");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderTVBody.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderShadows.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("BodyNormal");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderTVBodyNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Room");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderRoom.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderRoom.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RoomShadow");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "GeometryShaderRoom.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderRoomShadow.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RoomNormal");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/NormalShaders", "GeometryShaderRoomNormal.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RoomCubemapP1");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRoomCubemapP1.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderRoomShadow.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RoomCubemapP2");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRoomCubemapP2.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderRoomShadow.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("RoomCubemapP3");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/CubemapShaders", "GeometryShaderRoomCubemapP3.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders", "FragmentShaderRoomShadow.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Fireworks");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/FireworksShaders", "Particle_fireworks.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/FireworksShaders", "Particle.GS.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/FireworksShaders", "Particle_simple.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ViewColorTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/BufferShaders", "ViewColorTexture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/BufferShaders", "ViewColorTexture.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ViewDepthTexture");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/BufferShaders", "ViewDepthTexture.VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema1", "shaders/BufferShaders", "ViewDepthTexture.FS.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load Textures:
    {
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "noise.png");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "wood.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "vase.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "grey.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "lamp_wood.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "white.png");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "floor.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "wall.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "ceiling.jpg");
        TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES), "particle2.png");
    }

    // Parameters related to surface generation
    no_of_generated_points = 10;            // number of points on a Bezier curve
    no_of_instances = 5;                    // number of instances (number of curves that contain the surface)
    max_translate = 8.0f;                   // for the translation surface, it's the distance between the first and the last curve
    max_rotate = glm::radians(360.0f);      // for the rotation surface, it's the angle between the first and the last curve

    // Define control points
    control_p0 = glm::vec3(-0.89, 0,  0);
    control_p1 = glm::vec3(-2.69,  1,  0);
    control_p2 = glm::vec3(-0.05,  3,  0);
    control_p3 = glm::vec3(-0.63,  6, 0);

    // Create a bogus mesh with 2 points (a line)
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(control_p0, glm::vec3(0, 1, 1)),
            VertexFormat(control_p3, glm::vec3(0, 1, 0)),
            VertexFormat(control_p3, glm::vec3(0, 0, 1))
        };

        vector<unsigned int> indices =
        {
            0, 1
        };

        meshes["surface"] = new Mesh("generated initial surface points");
        meshes["surface"]->InitFromData(vertices, indices);
        meshes["surface"]->SetDrawMode(GL_LINES);
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    //Initialize objects:
    masa1 = new masa::Masa(shaders["TableSide"], shaders["TableTop"], shaders["Rotation"],
                           shaders["ShadowSide"], shaders["ShadowTop"], shaders["ShadowRotation"], shaders["ReflectionTop"],
                            shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"],
                            shaders["ReflectionSideP1"], shaders["ReflectionSideP2"], shaders["ReflectionSideP3"], meshes["surface"], glm::vec3(0, 0, -4.5));
    masa2 = new masa::Masa(shaders["TableSide"], shaders["TableTop"], shaders["Rotation"],
        shaders["ShadowSide"], shaders["ShadowTop"], shaders["ShadowRotation"], shaders["ReflectionTop"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"],
        shaders["ReflectionSideP1"], shaders["ReflectionSideP2"], shaders["ReflectionSideP3"], meshes["surface"], glm::vec3(0, 0, 6.5));
    masa_normal = new masa::Masa(shaders["NormalSide"], shaders["NormalTop"], shaders["NormalRotation"],
                                 shaders["ShadowSide"], shaders["ShadowTop"], shaders["ShadowRotation"], shaders["ReflectionTop"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"],
        shaders["ReflectionSideP1"], shaders["ReflectionSideP2"], shaders["ReflectionSideP3"], meshes["surface"], glm::vec3(0, 0, -4.5));
    tv1 = new tv::TV(shaders["TVBody"], shaders["TVScreen"], shaders["TVBodyShadow"], meshes["surface"], glm::vec3(0, 3.5, -4.5));
    tv_normal = new tv::TV(shaders["BodyNormal"], shaders["ScreenNormal"], shaders["TVBodyShadow"], meshes["surface"], glm::vec3(0, 3.5, -4.5));
    lampa1 = new lampa::Lampa(shaders["Rotation"], shaders["ShadowRotation"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"], meshes["surface"], glm::vec3(0, 3.5, 6.5));
    lampa_normala = new lampa::Lampa(shaders["NormalRotation"], shaders["ShadowRotation"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"], meshes["surface"], glm::vec3(0, 3.5, 6.5));
    vaza1 = new vaza::Vaza(shaders["Rotation"], shaders["ShadowRotation"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"], meshes["surface"], glm::vec3(2.5, 3.5, -5));
    vaza2 = new vaza::Vaza(shaders["Rotation"], shaders["ShadowRotation"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"], meshes["surface"], glm::vec3(-2.5, 3.5, -5));
    vaza_normala = new vaza::Vaza(shaders["NormalRotation"], shaders["ShadowRotation"],
        shaders["ReflectionRotationP1"], shaders["ReflectionRotationP2"], shaders["ReflectionRotationP3"], meshes["surface"], glm::vec3(0, 0, 0));

    //Initialize fireworks:
    ResetParticlesFireworks(20, 20, 20);

    generator_position = glm::vec3(0, 0, 0);
    offset = 0.05;
    time_since_last_firework = 0.0f;

    //Generate Framebuffer for Fireworks:
    glGenFramebuffers(1, &firework_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, firework_framebuffer);

    glGenTextures(1, &firework_texture);
    glBindTexture(GL_TEXTURE_2D, firework_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, firework_texture, 0);

    unsigned int depth_texture;
    glGenTextures(1, &depth_texture);
    glBindTexture(GL_TEXTURE_2D, depth_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture, 0);

    std::vector<GLenum> draw_textures;
    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    glCheckFramebufferStatus(GL_FRAMEBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Initialize framebuffer for shadows:
    glGenFramebuffers(1, &shadow_mapping_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_mapping_buffer);

    glGenTextures(1, &shadow_mapping_texture_color);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_color);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, shadow_mapping_texture_color, 0);

    glGenTextures(1, &shadow_mapping_texture_depth);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_depth);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, shadow_mapping_texture_depth, 0);

    std::vector<GLenum> draw_textures_shadows;
    draw_textures_shadows.push_back(GL_COLOR_ATTACHMENT0);
    glDrawBuffers(draw_textures_shadows.size(), &draw_textures_shadows[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);


    //Light variables:
    angle = 0;
    light_position = glm::vec3(0, 5.6, 6.3);
    light_direction = glm::normalize(glm::vec3(0, -0.37, -1));
    light_space_near_plane = 0.1f;
    light_space_far_plane = 30.0f;
    total_time_passed = 0.0f;


    //CubeMap stuff:
    std::string texturePath = PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube");
    cubeMapTextureID = UploadCubeMapTexture(
        PATH_JOIN(texturePath, "pos_x.png"),
        PATH_JOIN(texturePath, "pos_y.png"),
        PATH_JOIN(texturePath, "pos_z.png"),
        PATH_JOIN(texturePath, "neg_x.png"),
        PATH_JOIN(texturePath, "neg_y.png"),
        PATH_JOIN(texturePath, "neg_z.png"));


    glGenFramebuffers(1, &cubemap_buffer);
    glBindFramebuffer(GL_FRAMEBUFFER, cubemap_buffer);


    // TODO(student): Generate and bind the color texture
    glGenTextures(1, &cubemap_color_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_color_texture);


    // TODO(student): Initialize the color textures
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, 1024, 1024, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    if (cubemap_color_texture) {
        //cubemap params
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        if (GLEW_EXT_texture_filter_anisotropic) {
            float maxAnisotropy;

            glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
        }
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        // Bind the color textures to the framebuffer as a color attachments
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, cubemap_color_texture, 0);

        glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

        std::vector<GLenum> draw_textures_cubemap;
        draw_textures_cubemap.push_back(GL_COLOR_ATTACHMENT0);
        glDrawBuffers(draw_textures_cubemap.size(), &draw_textures_cubemap[0]);

    }

    // TODO(student): Generate and bind the depth texture
    glGenTextures(1, &cubemap_depth_texture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_depth_texture);


    // TODO(student): Initialize the depth textures
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);


    if (cubemap_depth_texture) {
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, cubemap_depth_texture, 0);
    }

    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

int Tema1::UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z) {
    int width, height, chn;

    unsigned char* data_pos_x = stbi_load(pos_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_y = stbi_load(pos_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_pos_z = stbi_load(pos_z.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_x = stbi_load(neg_x.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_y = stbi_load(neg_y.c_str(), &width, &height, &chn, 0);
    unsigned char* data_neg_z = stbi_load(neg_z.c_str(), &width, &height, &chn, 0);

    unsigned int textureID = 0;
    // TODO(student): Create the texture
    glGenTextures(1, &textureID);

    // TODO(student): Bind the texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (GLEW_EXT_texture_filter_anisotropic) {
        float maxAnisotropy;

        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // TODO(student): Load texture information for each face
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_pos_z);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_x);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_y);
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data_neg_z);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
    }

    // Free memory
    SAFE_FREE(data_pos_x);
    SAFE_FREE(data_pos_y);
    SAFE_FREE(data_pos_z);
    SAFE_FREE(data_neg_x);
    SAFE_FREE(data_neg_y);
    SAFE_FREE(data_neg_z);

    return textureID;
}

void Tema1::ResetParticlesFireworks(int xSize, int ySize, int zSize)
{
    unsigned int nrParticles = 2500;

    particleEffectTema1 = new ParticleEffect<ParticleTema1>();
    particleEffectTema1->Generate(nrParticles, true);

    auto particleSSBO = particleEffectTema1->GetParticleBuffer();
    ParticleTema1* data = const_cast<ParticleTema1*>(particleSSBO->GetBuffer());

    int xhSize = xSize / 2;
    int yhSize = ySize / 2;
    int zhSize = zSize / 2;

    for (unsigned int i = 0; i < nrParticles; i++)
    {
        glm::vec4 pos(1);
        pos.x = 0.0f;
        pos.y = 0.0f;
        pos.z = 0.0f;

        glm::vec4 speed(0);
        speed.x = 0.0f;
        speed.z = 0.0f;
        speed.y = 4.0f;

        data[i].SetInitial(pos, speed);
    }

    firework_color = glm::vec3((rand() % 100) / 100.0f, (rand() % 100) / 100.0f, (rand() % 100) / 100.0f);
    particleSSBO->SetBufferData(data);
}

void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema1::RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, instances);
}

void Tema1::DrawRoom() {
    Shader* shader = shaders["Room"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("floor.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    TextureManager::GetTexture("ceiling.jpg")->BindToTextureUnit(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    TextureManager::GetTexture("wall.jpg")->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Tema1::DrawRoomShadow() {
    Shader* shader = shaders["RoomShadow"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("floor.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    TextureManager::GetTexture("ceiling.jpg")->BindToTextureUnit(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    TextureManager::GetTexture("wall.jpg")->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);
    GLint loc_light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(loc_light_direction, light_direction.x, light_direction.y, light_direction.z);
    GLint eye_position_loc = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position_loc, eye_position.x, eye_position.y, eye_position.z);
    GLint loc_light_space_view = glGetUniformLocation(shader->program, "light_space_view");
    glUniformMatrix4fv(loc_light_space_view, 1, GL_FALSE, glm::value_ptr(light_space_view));
    GLint loc_light_space_projection = glGetUniformLocation(shader->program, "light_space_projection");
    glUniformMatrix4fv(loc_light_space_projection, 1, GL_FALSE, glm::value_ptr(light_space_projection));
    GLint loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, 30);
    GLint loc_light_intensity = glGetUniformLocation(shader->program, "light_intensity");
    glUniform1f(loc_light_intensity, light_intensity);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 3);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Tema1::DrawRoomCubemap() {
    //Draw P1:
    Shader* shader = shaders["RoomCubemapP1"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("floor.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    TextureManager::GetTexture("ceiling.jpg")->BindToTextureUnit(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    TextureManager::GetTexture("wall.jpg")->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    
    glm::vec3 eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    GLint loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);
    GLint loc_light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(loc_light_direction, light_direction.x, light_direction.y, light_direction.z);
    GLint eye_position_loc = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position_loc, eye_position.x, eye_position.y, eye_position.z);
    GLint loc_light_space_view = glGetUniformLocation(shader->program, "light_space_view");
    glUniformMatrix4fv(loc_light_space_view, 1, GL_FALSE, glm::value_ptr(light_space_view));
    GLint loc_light_space_projection = glGetUniformLocation(shader->program, "light_space_projection");
    glUniformMatrix4fv(loc_light_space_projection, 1, GL_FALSE, glm::value_ptr(light_space_projection));
    GLint loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, 30);
    GLint loc_light_intensity = glGetUniformLocation(shader->program, "light_intensity");
    glUniform1f(loc_light_intensity, light_intensity);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 3);

    glm::mat4 cubeView[6] =
    {
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(1, 4, -4.5), glm::vec3(0.0f,-1.0f, 0.0f)), // +X
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(-1, 4, -4.5), glm::vec3(0.0f,-1.0f, 0.0f)), // -X
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(0, 5, -4.5), glm::vec3(0.0f, 0.0f, 1.0f)), // +Y
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(0, 3, -4.5), glm::vec3(0.0f, 0.0f,-1.0f)), // -Y
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(0, 4, -3.5), glm::vec3(0.0f,-1.0f, 0.0f)), // +Z
        glm::lookAt(glm::vec3(0, 4, -4.5), glm::vec3(0, 4, -5.5), glm::vec3(0.0f,-1.0f, 0.0f)), // -Z
    };

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);


    //Draw P2:
    shader = shaders["RoomCubemapP2"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("floor.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    TextureManager::GetTexture("ceiling.jpg")->BindToTextureUnit(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    TextureManager::GetTexture("wall.jpg")->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    
    eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);
    loc_light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(loc_light_direction, light_direction.x, light_direction.y, light_direction.z);
    eye_position_loc = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position_loc, eye_position.x, eye_position.y, eye_position.z);
    loc_light_space_view = glGetUniformLocation(shader->program, "light_space_view");
    glUniformMatrix4fv(loc_light_space_view, 1, GL_FALSE, glm::value_ptr(light_space_view));
    loc_light_space_projection = glGetUniformLocation(shader->program, "light_space_projection");
    glUniformMatrix4fv(loc_light_space_projection, 1, GL_FALSE, glm::value_ptr(light_space_projection));
    loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, 30);
    loc_light_intensity = glGetUniformLocation(shader->program, "light_intensity");
    glUniform1f(loc_light_intensity, light_intensity);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 3);

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);



    //Draw P3:
    shader = shaders["RoomCubemapP3"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("floor.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    TextureManager::GetTexture("ceiling.jpg")->BindToTextureUnit(GL_TEXTURE1);
    glUniform1i(glGetUniformLocation(shader->program, "texture_2"), 1);
    TextureManager::GetTexture("wall.jpg")->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(glGetUniformLocation(shader->program, "texture_3"), 2);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));
    
    
    eye_position = GetSceneCamera()->m_transform->GetWorldPosition();
    loc_light_position = glGetUniformLocation(shader->program, "light_position");
    glUniform3f(loc_light_position, light_position.x, light_position.y, light_position.z);
    loc_light_direction = glGetUniformLocation(shader->program, "light_direction");
    glUniform3f(loc_light_direction, light_direction.x, light_direction.y, light_direction.z);
    eye_position_loc = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position_loc, eye_position.x, eye_position.y, eye_position.z);
    loc_light_space_view = glGetUniformLocation(shader->program, "light_space_view");
    glUniformMatrix4fv(loc_light_space_view, 1, GL_FALSE, glm::value_ptr(light_space_view));
    loc_light_space_projection = glGetUniformLocation(shader->program, "light_space_projection");
    glUniformMatrix4fv(loc_light_space_projection, 1, GL_FALSE, glm::value_ptr(light_space_projection));
    loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, 30);
    loc_light_intensity = glGetUniformLocation(shader->program, "light_intensity");
    glUniform1f(loc_light_intensity, light_intensity);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, shadow_mapping_texture_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 3);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Tema1::DrawRoomNormal() {
    Shader* shader = shaders["RoomNormal"];
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-15, 0, 15);
    control_p1 = glm::vec3(-15, 15, 15);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = meshes["surface"];

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::mat4(1)));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Tema1::RenderFireworks(float deltaTimeSeconds) {
    glClearColor(0, 0, 0, 1);
    // Clears the color buffer (using the previously set color) and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);
    time_since_last_firework += deltaTimeSeconds;
    if (time_since_last_firework > 1.5f) {
        ResetParticlesFireworks(20, 20, 20);
        time_since_last_firework = 0.0f;
    }

    auto shader = shaders["Fireworks"];
    if (shader->GetProgramID())
    {
        shader->Use();

        TextureManager::GetTexture("particle2.png")->BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
        particleEffectTema1->Render(camera_firewroks, shader);

        int loc_color = shader->GetUniformLocation("firework_color");
        glUniform3fv(loc_color, 1, glm::value_ptr(firework_color));

        // TODO(student): Send uniforms generator_position,
        // deltaTime and offset to the shader
        int loc_instances = shader->GetUniformLocation("generator_position");
        glUniform3fv(loc_instances, 1, glm::value_ptr(glm::vec3(0, -2, 0)));

        glUniform1f(glGetUniformLocation(shader->program, "offset"), offset);
        glUniform1f(glGetUniformLocation(shader->program, "deltaTime"), deltaTimeSeconds);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
}

void Tema1::RenderPassOne() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, 1024, 1024);
    light_space_view = GetSceneCamera()->GetViewMatrix();
    light_space_projection = GetSceneCamera()->GetProjectionMatrix();

    DrawRoom();
    lampa1->RenderLampa(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    masa1->RenderMasa(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    masa2->RenderMasa(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    tv1->RenderTV(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), firework_texture, cubeMapTextureID, GetSceneCamera()->m_transform->GetWorldPosition());
    vaza1->RenderVaza(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    vaza2->RenderVaza(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
}

void Tema1::RenderPassTwo() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, window->GetResolution(true).x, window->GetResolution(true).y);

    DrawRoomShadow();
    //DrawRoomNormal();
    masa1->RenderMasaShadow(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    masa2->RenderMasaShadow(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    //masa_normal->RenderMasa(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    tv1->RenderTVShadow(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), firework_texture, shadow_mapping_texture_depth,
            GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
            light_space_view, light_space_projection, light_intensity, cubemap_color_texture);
    //tv_normal->RenderTV(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    lampa1->RenderLampaShadows(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
                               GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
                               light_space_view, light_space_projection, light_intensity);
    //lampa_normala->RenderLampa(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
    vaza1->RenderVazaShadow(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
                            GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
                            light_space_view, light_space_projection, light_intensity);
    vaza2->RenderVazaShadow(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    //vaza_normala->RenderVaza(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix());
}

void Tema1::RenderCubemap() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, 1024, 1024);

    DrawRoomCubemap();
    masa1->RenderMasaReflection(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    masa2->RenderMasaReflection(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    lampa1->RenderLampaReflection(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    vaza1->RenderVazaReflection(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);
    vaza2->RenderVazaReflection(GetSceneCamera()->GetProjectionMatrix(), GetSceneCamera()->GetViewMatrix(), shadow_mapping_texture_depth,
        GetSceneCamera()->m_transform->GetWorldPosition(), light_position, light_direction, glm::vec3(1, 1, 1),
        light_space_view, light_space_projection, light_intensity);

    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap_color_texture);
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void Tema1::RenderSceneRoom() {
    auto camera = GetSceneCamera();
    glm::vec3 camPosition = camera->m_transform->GetWorldPosition();
    glm::quat camRotation = camera->m_transform->GetWorldRotation();
    auto projectionInfo = camera->GetProjectionInfo();

    //Render step one for shadow calculations
    glBindFramebuffer(GL_FRAMEBUFFER, shadow_mapping_buffer);
    camera->SetPosition(light_position);
    camera->SetRotation(glm::quatLookAt(light_direction, glm::vec3(0, 1, 0)));
    camera->SetPerspective(90, 1, 0.1f, 40.0f);
    RenderPassOne();

    //Render room without TV for reflection calculations
    camera->SetPosition(camPosition);
    camera->SetRotation(camRotation);
    glBindFramebuffer(GL_FRAMEBUFFER, cubemap_buffer);
    RenderCubemap();

    camera->SetProjection(projectionInfo);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    RenderPassTwo();
    //DrawFramebufferTextures();
}

void Tema1::Update(float deltaTimeSeconds)
{
    total_time_passed += deltaTimeSeconds;
    light_intensity = sin(1.2f * total_time_passed) * 10.0f + 10.0f;
    glBindFramebuffer(GL_FRAMEBUFFER, firework_framebuffer);
    glViewport(0, 0, 1024, 1024);
    RenderFireworks(deltaTimeSeconds);
    RenderSceneRoom();
    //tv1->RotateTV(deltaTimeSeconds * 40);
}


void Tema1::FrameEnd()
{
#if 0
    DrawCoordinateSystem();
#endif
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */

void Tema1::DrawFramebufferTextures()
{
    int screenPosX = window->GetResolution().x - 950;
    int screenPosY = 100;

    int width = 400;
    int height = 400;

    if (shadow_mapping_texture_color)
    {
        // Render the color texture on the screen
        glViewport(screenPosX, screenPosY, width, height);

        RenderTextureScreen(shaders["ViewColorTexture"], shadow_mapping_texture_color);
    }

    if (shadow_mapping_texture_depth)
    {
        // Render the depth texture on the screen
        glViewport(screenPosX + 450, screenPosY, width, height);

        RenderTextureScreen(shaders["ViewDepthTexture"], shadow_mapping_texture_depth);
    }
}

void Tema1::RenderTextureScreen(Shader* shader, unsigned int textureID)
{
    if (!shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set uniforms for near and far plane of the
    // projection transformation in the light space
    GLint loc_light_space_near_plane = glGetUniformLocation(shader->program, "light_space_near_plane");
    glUniform1f(loc_light_space_near_plane, light_space_near_plane);

    GLint loc_light_space_far_plane = glGetUniformLocation(shader->program, "light_space_far_plane");
    glUniform1f(loc_light_space_far_plane, light_space_far_plane);

    // Set texture uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // Draw the object
    glBindVertexArray(meshes["quad"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["quad"]->GetDrawMode(), static_cast<int>(meshes["quad"]->indices.size()), GL_UNSIGNED_INT, 0);
}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // You can move the control points around by using the dedicated key,
    // in combination with Ctrl, Shift, or both.
    float delta = deltaTime;
    /*auto keyMaps = std::vector<std::pair<glm::vec3 &, uint32_t>>
    {
        { control_p0, GLFW_KEY_1 },
        { control_p1, GLFW_KEY_2 },
        { control_p2, GLFW_KEY_3 },
        { control_p3, GLFW_KEY_4 },
    };

    for (const auto &k : keyMaps)
    {
        if (window->KeyHold(k.second))
        {
            if (mods & GLFW_MOD_SHIFT && mods & GLFW_MOD_CONTROL)
            {
                k.first.y -= delta;
            }
            else if (mods & GLFW_MOD_CONTROL)
            {
                k.first.y += delta;
            }
            else if (mods & GLFW_MOD_SHIFT)
            {
                k.first.x -= delta;
            }
            else
            {
                k.first.x += delta;
            }

            std::cout << glm::vec2(control_p0) << glm::vec2(control_p1) << glm::vec2(control_p2) << glm::vec2(control_p3) << "\n";
        }
    }*/

    if (window->KeyHold(GLFW_KEY_1)) {
        tv1->RotateTV(20.0f * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_2)) {
        tv1->RotateTV(-20.0f * deltaTime);
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // TODO(student): Use keys to change the number of instances and the
    // number of generated points. Avoid the camera keys, and avoid the
    // the keys from `OnInputUpdate`.
    if (key == GLFW_KEY_M) {
        tv1->RotateTV(20.0f);
    }
    if (key == GLFW_KEY_N) {
        tv1->RotateTV(-20.0f);
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema1::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
