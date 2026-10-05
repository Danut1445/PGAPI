#include "lab_m2/Tema2/Tema2.h"

#include <vector>
#include <iostream>

#include "pfd/portable-file-dialogs.h"

using namespace std;
using namespace m2;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
    outputMode = 0;
    gpuProcessing = false;
    saveScreenToImage = false;
    window->SetSize(600, 600);
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{
    // Load default texture fore imagine processing
    originalImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "image", true, true);
    processedImage = TextureManager::LoadTexture(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::TEXTURES, "cube", "pos_x.png"), nullptr, "newImage", true, true);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    std::string shaderPath = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M2, "Tema2", "shaders");

    // Create a shader program for particle system
    {
        Shader *shader = new Shader("Step1");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderStep1.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Step2");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderStep2.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("Circles");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShaderCircles.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
        Shader* shader = new Shader("ImageProcessing");
        shader->AddShader(PATH_JOIN(shaderPath, "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, "FragmentShader.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    CreateFramebufferStep1(window->GetResolution().x, window->GetResolution().y);
    CreateFramebufferStep2(window->GetResolution().x, window->GetResolution().y);

    radius = 3.0f;
    mousePosition = glm::vec2(0.5f, 0.5f);
    ProcessImage();
    current_mode = 0;
    radius = 0.3f;
}

void Tema2::CreateFramebufferStep1(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture' and 'depth_texture'
    // declared in lab1.h

    // TODO(student): Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer_object_P1);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_P1);

    // TODO(student): Generate, bind and initialize the color texture
    glGenTextures(1, &color_texture_P11);
    glBindTexture(GL_TEXTURE_2D, color_texture_P11);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture_P11, 0);

    glGenTextures(1, &color_texture_P12);
    glBindTexture(GL_TEXTURE_2D, color_texture_P12);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, color_texture_P12, 0);

    // TODO(student): Generate, bind and initialize the depth texture
    glGenTextures(1, &depth_texture_P1);
    glBindTexture(GL_TEXTURE_2D, depth_texture_P1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // TODO(student): Bind the depth texture to the framebuffer as a depth attachment
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_P1, 0);

    // TODO(student): Set the color texture as the draw texture
    std::vector<GLenum> draw_textures;

    draw_textures.push_back(GL_COLOR_ATTACHMENT0);
    draw_textures.push_back(GL_COLOR_ATTACHMENT1);

    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    // TODO(student): Check the status of the framebuffer
    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::CreateFramebufferStep2(int width, int height)
{
    // TODO(student): In this method, use the attributes
    // 'framebuffer_object', 'color_texture' and 'depth_texture'
    // declared in lab1.h

    // TODO(student): Generate and bind the framebuffer
    glGenFramebuffers(1, &framebuffer_object_P2);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_P2);

    // TODO(student): Generate, bind and initialize the color texture
    glGenTextures(1, &color_texture_P2);
    glBindTexture(GL_TEXTURE_2D, color_texture_P2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    // TODO(student): Bind the color texture to the
    // framebuffer as a color attachment at position 0
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_texture_P2, 0);

    // TODO(student): Generate, bind and initialize the depth texture
    glGenTextures(1, &depth_texture_P2);
    glBindTexture(GL_TEXTURE_2D, depth_texture_P2);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, 0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    // TODO(student): Bind the depth texture to the framebuffer as a depth attachment
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_texture_P2, 0);

    // TODO(student): Set the color texture as the draw texture
    std::vector<GLenum> draw_textures;

    draw_textures.push_back(GL_COLOR_ATTACHMENT0);

    glDrawBuffers(draw_textures.size(), &draw_textures[0]);

    // TODO(student): Check the status of the framebuffer
    glCheckFramebufferStatus(GL_FRAMEBUFFER);

    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Tema2::FrameStart()
{
}

void Tema2::ProcessImage() {
    // Executa Pasul 1 al procesului de blur:
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_P1);
    ClearScreen();

    auto shader = shaders["Step1"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 0);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::vec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    int radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    originalImage->BindToTextureUnit(GL_TEXTURE0);
    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));


    //Executa Pasul 2 al procesului de blur:
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer_object_P2);
    ClearScreen();

    shader = shaders["Step1"];
    shader->Use();

    flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 0);

    screenSize_loc = shader->GetUniformLocation("screenSize");
    resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    locTexture = shader->GetUniformLocation("textureImage");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_texture_P11);
    glUniform1i(locTexture, 0);

    locTexture = shader->GetUniformLocation("textureBlur");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, color_texture_P12);
    glUniform1i(locTexture, 1);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::ShowStep1() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::vec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    int radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, 0);

    int locTexture = shader->GetUniformLocation("textureImage");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_texture_P11);
    glUniform1i(locTexture, 0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::ShowStep2() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::vec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    int radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, 0);

    int locTexture = shader->GetUniformLocation("textureImage");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_texture_P2);
    glUniform1i(locTexture, 0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::ShowOriginal() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ClearScreen();

    auto shader = shaders["ImageProcessing"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::vec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    int radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    int outputMode_loc = shader->GetUniformLocation("outputMode");
    glUniform1i(outputMode_loc, 0);

    originalImage->BindToTextureUnit(GL_TEXTURE0);
    int locTexture = shader->GetUniformLocation("textureImage");
    glUniform1i(locTexture, 0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::ShowCircles() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ClearScreen();

    auto shader = shaders["Circles"];
    shader->Use();

    int flip_loc = shader->GetUniformLocation("flipVertical");
    glUniform1i(flip_loc, 1);

    int screenSize_loc = shader->GetUniformLocation("screenSize");
    glm::vec2 resolution = window->GetResolution();
    glUniform2i(screenSize_loc, resolution.x, resolution.y);

    int mousePosition_loc = shader->GetUniformLocation("mousePosition");
    glUniform2i(mousePosition_loc, mousePosition.x, mousePosition.y);

    int radius_loc = shader->GetUniformLocation("radius");
    glUniform1f(radius_loc, radius);

    int locTexture = shader->GetUniformLocation("textureImage");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, color_texture_P2);
    glUniform1i(locTexture, 0);

    RenderMesh(meshes["quad"], shader, glm::mat4(1));
}

void Tema2::Update(float deltaTimeSeconds)
{
    // Blur Gausian Orizontal
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    ClearScreen();

    switch (current_mode) {
    case 1: {
        ShowStep1();
        break;
    }
    case 2: {
        ShowStep2();
        break;
    }
    case 3: {
        ShowStep2();
        break;
    }
    case 4: {
        ShowCircles();
        break;
    }
    default: {
        ShowOriginal();
        break;
    }
    }
}


void Tema2::FrameEnd()
{
    DrawCoordinateSystem();
}


void Tema2::OnFileSelected(const std::string &fileName)
{
    if (fileName.size())
    {
        std::cout << fileName << endl;
        originalImage = TextureManager::LoadTexture(fileName, nullptr, "image", true, true);
        processedImage = TextureManager::LoadTexture(fileName, nullptr, "newImage", true, true);

        float aspectRatio = static_cast<float>(originalImage->GetWidth()) / originalImage->GetHeight();
        window->SetSize(static_cast<int>(600 * aspectRatio), 600);
    }
}


void Tema2::GrayScale()
{
    unsigned int channels = originalImage->GetNrChannels();
    unsigned char* data = originalImage->GetImageData();
    unsigned char* newData = processedImage->GetImageData();

    if (channels < 3)
        return;

    glm::ivec2 imageSize = glm::ivec2(originalImage->GetWidth(), originalImage->GetHeight());

    for (int i = 0; i < imageSize.y; i++)
    {
        for (int j = 0; j < imageSize.x; j++)
        {
            int offset = channels * (i * imageSize.x + j);

            // Reset save image data
            char value = static_cast<char>(data[offset + 0] * 0.2f + data[offset + 1] * 0.71f + data[offset + 2] * 0.07);
            memset(&newData[offset], value, 3);
        }
    }

    processedImage->UploadNewData(newData);
}


void Tema2::SaveImage(const std::string &fileName)
{
    cout << "Saving image! ";
    processedImage->SaveToFile((fileName + ".png").c_str());
    cout << "[Done]" << endl;
}


void Tema2::OpenDialog()
{
    std::vector<std::string> filters =
    {
        "Image Files", "*.png *.jpg *.jpeg *.bmp",
        "All Files", "*"
    };

    auto selection = pfd::open_file("Select a file", ".", filters).result();
    if (!selection.empty())
    {
        std::cout << "User selected file " << selection[0] << "\n";
        OnFileSelected(selection[0]);
    }
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F || key == GLFW_KEY_ENTER || key == GLFW_KEY_SPACE)
    {
        OpenDialog();
    }

    if (key == GLFW_KEY_KP_ADD) {
        radius += 0.01f;
    }

    if (key == GLFW_KEY_KP_SUBTRACT) {
        radius -= 0.01f;
    }
    
    if (key == GLFW_KEY_0) {
        current_mode = 0;
    }

    if (key == GLFW_KEY_1) {
        current_mode = 1;
    }

    if (key == GLFW_KEY_2) {
        current_mode = 2;
    }

    if (key == GLFW_KEY_3) {
        current_mode = 3;
    }

    if (key == GLFW_KEY_4) {
        current_mode = 4;
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
    mousePosition = glm::vec2(mouseX, mouseY);
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
    ProcessImage();
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema2::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
