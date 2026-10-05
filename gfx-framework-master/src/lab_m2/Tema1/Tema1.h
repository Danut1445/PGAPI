#pragma once

#include "components/simple_scene.h"
#include "lab_m2/Tema1/Masa.h"
#include "lab_m2/Tema1/TV.h"
#include "lab_m2/Tema1/Lampa.h"
#include "lab_m2/Tema1/Vaza.h"
#include "core/gpu/particle_effect.h"

namespace m2
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMeshInstanced(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, int instances, const glm::vec3 &color = glm::vec3(1));
        void DrawRoom();
        void DrawRoomShadow();
        void DrawRoomNormal();
        void DrawRoomCubemap();
        void ResetParticlesFireworks(int xSize, int ySize, int zSize);

        void DrawFramebufferTextures();
        void RenderTextureScreen(Shader* shader, unsigned int textureID);

        int UploadCubeMapTexture(const std::string& pos_x, const std::string& pos_y, const std::string& pos_z, const std::string& neg_x, const std::string& neg_y, const std::string& neg_z);

        void RenderSceneRoom();
        void RenderPassOne();
        void RenderPassTwo();
        void RenderCubemap();
        void RenderFireworks(float deltaTimeSeconds);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:
        // Info about the generated surfaces
        glm::vec3 control_p0, control_p1, control_p2, control_p3;
        unsigned int no_of_generated_points, no_of_instances;
        float max_translate, max_rotate;
        masa::Masa* masa1, *masa_normal, *masa2;
        tv::TV* tv1, *tv_normal;
        lampa::Lampa* lampa1, *lampa_normala;
        vaza::Vaza* vaza1, *vaza_normala, *vaza2;
        glm::vec3 generator_position;
        float offset;
        float time_since_last_firework;
        glm::vec3 firework_color;
        gfxc::Camera *camera_firewroks;
        unsigned int firework_framebuffer, shadow_mapping_buffer, cubemap_buffer;
        unsigned int firework_texture, shadow_mapping_texture_color, shadow_mapping_texture_depth, cubemap_color_texture, cubemap_depth_texture;
        unsigned int cubeMapTextureID;

        float angle;
        glm::vec3 light_position;
        glm::vec3 light_direction;
        glm::mat4 light_space_view;
        glm::mat4 light_space_projection;
        float light_space_near_plane;
        float light_space_far_plane;
        float light_intensity;
        float total_time_passed;
    };
}   // namespace m2
