#include "lab_m2/lab7/lab7.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab7::Lab7()
{
}


Lab7::~Lab7()
{
}


void Lab7::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 3.5, 4), glm::quat(glm::vec3(-30 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Create a shader program for rendering to texture
    {
        Shader* shader = new Shader("Skinning");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab7", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "lab7", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Load a mesh from file into GPU memory
    {
        Mesh* mesh = new Mesh("animation");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "skinning"), "boblampclean.md5mesh");
        meshes[mesh->GetMeshID()] = mesh;
    }
}


void Lab7::FrameStart()
{
}


void Lab7::Update(float deltaTimeSeconds)
{
    ClearScreen();
    auto shader = shaders["Skinning"];
    shader->Use();

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-90.0f), glm::vec3(1, 0, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f));
    RenderSimpleMesh(meshes["animation"], shader, modelMatrix);

    float runningTime = (float)((double)Engine::GetElapsedTime());
    BoneTransform(meshes["animation"], runningTime);
}

void Lab7::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(GetSceneCamera()->GetProjectionMatrix()));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    glm::mat4 bones[200];
    for (int i = 0; i < mesh->m_BoneInfo.size(); i++)
    {
        bones[i] = mesh->m_BoneInfo[i].finalTransformation;
    }

    // TODO (student): Send the bone final transformation to the shader
    GLint bonesLoc = glGetUniformLocation(shader->program, "BoneTransforms");
    glUniformMatrix4fv(bonesLoc, mesh->m_BoneInfo.size(), GL_FALSE, glm::value_ptr(bones[0]));

    mesh->Render();
}

void Lab7::BoneTransform(Mesh* mesh, float timeInSeconds)
{
    glm::mat4 Identity = glm::mat4(1.0f);

    float ticksPerSecond = mesh->anim[0]->mTicksPerSecond != 0 ? mesh->anim[0]->mTicksPerSecond : 25.0f;
    float timeInTicks = timeInSeconds * ticksPerSecond;
    float animationTime = fmod(timeInTicks, mesh->anim[0]->mDuration);

    // TODO (student): Uncomment this to test the animation
    ReadNodeHierarchy(mesh, animationTime, mesh->rootNode, Identity, mesh->anim);
}
void Lab7::ReadNodeHierarchy(Mesh* mesh, float animationTime, const aiNode* pNode, const glm::mat4& parentTransform, aiAnimation** anim)
{
    std::string NodeName(pNode->mName.data);
    const aiAnimation* pAnimation = anim[0];
    glm::mat4 nodeTransformation(mesh->ConvertMatrix(pNode->mTransformation));
    const aiNodeAnim* pNodeAnim = FindNodeAnim(pAnimation, NodeName);

    if (pNodeAnim) {
        aiVector3D Scaling;
        ComputeInterpolatedScaling(Scaling, animationTime, pNodeAnim);
        glm::mat4 ScalingM = glm::transpose(glm::mat4(Scaling.x, 0, 0, 0,
            0, Scaling.y, 0, 0,
            0, 0, Scaling.z, 0,
            0, 0, 0, 1));

        aiQuaternion RotationQ;
        ComputeInterpolatedRotation(RotationQ, animationTime, pNodeAnim);
        glm::quat rotation(RotationQ.w, RotationQ.x, RotationQ.y, RotationQ.z);
        glm::mat4 RotationM = glm::toMat4(rotation);

        aiVector3D Translation;
        ComputeInterpolatedPosition(Translation, animationTime, pNodeAnim);
        glm::mat4 TranslationM = glm::transpose(glm::mat4(1, 0, 0, Translation.x,
            0, 1, 0, Translation.y,
            0, 0, 1, Translation.z,
            0, 0, 0, 1));

        nodeTransformation = TranslationM * RotationM * ScalingM;
    }

    // TODO (student): Apply the parent transformation to the current transformation
    glm::mat4 GlobalTransformation = parentTransform * nodeTransformation;

    if (mesh->m_BoneMapping.find(NodeName) != mesh->m_BoneMapping.end()) {
        unsigned int BoneIndex = mesh->m_BoneMapping[NodeName];
        mesh->m_BoneInfo[BoneIndex].finalTransformation = mesh->m_GlobalInverseTransform * GlobalTransformation *
            mesh->m_BoneInfo[BoneIndex].boneOffset;
    }

    for (unsigned int i = 0; i < pNode->mNumChildren; i++) {
        ReadNodeHierarchy(mesh, animationTime, pNode->mChildren[i], GlobalTransformation, anim);
    }
}

const aiNodeAnim* Lab7::FindNodeAnim(const aiAnimation* pAnimation, const std::string NodeName)
{
    for (unsigned int i = 0; i < pAnimation->mNumChannels; i++) {
        const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

        if (std::string(pNodeAnim->mNodeName.data) == NodeName) {
            return pNodeAnim;
        }
    }

    return NULL;
}

unsigned int Lab7::FindRotation(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumRotationKeys > 0);

    // TODO (student): Find the last rotation key before animationTime
    for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++) {
        if (animationTime < (float)pNodeAnim->mRotationKeys[i + 1].mTime) {
            return i;
        }
    }
    return pNodeAnim->mNumRotationKeys - 1;
}

void Lab7::ComputeInterpolatedRotation(aiQuaternion& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumRotationKeys == 1) {
        out = pNodeAnim->mRotationKeys[0].mValue;
        return;
    }

    unsigned int rotationIndex = FindRotation(animationTime, pNodeAnim);
    unsigned int nextRotationIndex = (rotationIndex + 1);
    assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    float deltaTime = (float)(pNodeAnim->mRotationKeys[nextRotationIndex].mTime - pNodeAnim->mRotationKeys[rotationIndex].mTime);

    float factor = (animationTime - (float)pNodeAnim->mRotationKeys[rotationIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Interpolate the rotation
    const aiQuaternion& start = pNodeAnim->mRotationKeys[rotationIndex].mValue;
    const aiQuaternion& end = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
    aiQuaternion::Interpolate(out, start, end, factor);
    out.Normalize();
}

unsigned int Lab7::FindScaling(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumScalingKeys > 0);

    // TODO (student): Find the last scaling key before animationTime
    for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++) {
        if (animationTime < (float)pNodeAnim->mScalingKeys[i + 1].mTime) {
            return i;
        }
    }
    return pNodeAnim->mNumScalingKeys - 1;
}


void Lab7::ComputeInterpolatedScaling(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumScalingKeys == 1) {
        out = pNodeAnim->mScalingKeys[0].mValue;
        return;
    }

    unsigned int scalingIndex = FindScaling(animationTime, pNodeAnim);
    unsigned int nextScalingIndex = (scalingIndex + 1);
    assert(nextScalingIndex < pNodeAnim->mNumScalingKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    float deltaTime = (float)(pNodeAnim->mScalingKeys[nextScalingIndex].mTime - pNodeAnim->mScalingKeys[scalingIndex].mTime);

    float factor = (animationTime - (float)pNodeAnim->mScalingKeys[scalingIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Interpolate the scaling
    const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
    const aiVector3D& end = pNodeAnim->mScalingKeys[nextScalingIndex].mValue;
    out = start + factor * (end - start);
}


unsigned int Lab7::FindPosition(float animationTime, const aiNodeAnim* pNodeAnim)
{
    assert(pNodeAnim->mNumPositionKeys > 0);

    // TODO (student): Find the last position key before animationTime
    for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
        if (animationTime < (float)pNodeAnim->mPositionKeys[i + 1].mTime) {
            return i;
        }
    }
    return pNodeAnim->mNumPositionKeys - 1;
}

void Lab7::ComputeInterpolatedPosition(aiVector3D& out, float animationTime, const aiNodeAnim* pNodeAnim)
{
    if (pNodeAnim->mNumPositionKeys == 1) {
        out = pNodeAnim->mPositionKeys[0].mValue;
        return;
    }

    unsigned int positionIndex = FindPosition(animationTime, pNodeAnim);
    unsigned int nextPositionIndex = (positionIndex + 1);
    assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

    // TODO (student): Compute the duration between the two selected time stamps
    float deltaTime = (float)(pNodeAnim->mPositionKeys[nextPositionIndex].mTime - pNodeAnim->mPositionKeys[positionIndex].mTime);

    float factor = (animationTime - (float)pNodeAnim->mPositionKeys[positionIndex].mTime) / deltaTime;
    assert(factor >= 0.0f && factor <= 1.0f);

    // TODO (student): Interpolate the position
    const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
    const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
    out = start + factor * (end - start);
}

void Lab7::FrameEnd()
{
    DrawCoordinateSystem();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab7::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input with window->KeyHold()

}


void Lab7::OnKeyPress(int key, int mods)
{
    // Add key press event

}


void Lab7::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Lab7::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab7::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab7::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab7::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab7::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
