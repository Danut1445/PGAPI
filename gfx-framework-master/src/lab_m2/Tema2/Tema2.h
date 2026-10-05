#pragma once

#include <string>

#include "components/simple_scene.h"
#include "core/gpu/frame_buffer.h"


namespace m2
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

        void CreateFramebufferStep1(int width, int height);
        void CreateFramebufferStep2(int width, int height);

        void ProcessImage();
        void ShowStep1();
        void ShowStep2();
        void ShowOriginal();
        void ShowCircles();

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void OpenDialog();
        void OnFileSelected(const std::string &fileName);

        // Processing effects
        void GrayScale();
        void SaveImage(const std::string &fileName);

     private:
        Texture2D *originalImage;
        Texture2D *processedImage;

        unsigned int framebuffer_object_P1, color_texture_P11, color_texture_P12, depth_texture_P1, framebuffer_object_P2, color_texture_P2, depth_texture_P2;

        int outputMode;
        bool gpuProcessing;
        bool saveScreenToImage;
        int current_mode;

        glm::vec2 mousePosition;
        float radius;
    };
}   // namespace m2
