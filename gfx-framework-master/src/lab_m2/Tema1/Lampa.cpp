#include "Lampa.h"

using namespace std;
using namespace lampa;

Lampa::Lampa(Shader* shader_rotire, Shader* shader_rotire_shadow, Shader* shader_rotire_reflection_p1,
    Shader* shader_rotire_reflection_p2, Shader* shader_rotire_reflection_p3, Mesh* mesh, glm::vec3 location) {
	this->shader_rotire = shader_rotire;
	this->nonimportant_mesh = mesh;
	this->Model = glm::mat4(1);
    this->Model = glm::translate(this->Model, location);
    this->Model = glm::scale(this->Model, glm::vec3(0.5, 0.5, 0.5));
    this->shader_rotire_reflection_p1 = shader_rotire_reflection_p1;
    this->shader_rotire_reflection_p2 = shader_rotire_reflection_p2;
    this->shader_rotire_reflection_p3 = shader_rotire_reflection_p3;
    this->shader_rotire_shadow = shader_rotire_shadow;
}

void Lampa::RenderCorp(glm::mat4 Projection, glm::mat4 View) {
    Shader* shader = shader_rotire;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.53, 0, 0);
    control_p1 = glm::vec3(-1.58, 0.5, 0);
    control_p2 = glm::vec3(-0.38, 1, 0);
    control_p3 = glm::vec3(-0.23, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 0.58, 0.2, 0.2);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("lamp_wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 12);
}

void Lampa::RenderPalarie(glm::mat4 Projection, glm::mat4 View) {
    Shader* shader = shader_rotire;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(0, 0, 0);
    control_p1 = glm::vec3(-1.36, 1, 0);
    control_p2 = glm::vec3(-0.89, 3.5, 0);
    control_p3 = glm::vec3(-1.74, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 1, 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glm::mat4 Model_palarie = Model;
    Model_palarie = glm::rotate(Model_palarie, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    Model_palarie = glm::translate(Model_palarie, glm::vec3(0.4, 4, 0));
    Model_palarie = glm::rotate(Model_palarie, glm::radians(100.0f), glm::vec3(0, 0, 1));
    Model_palarie = glm::scale(Model_palarie, glm::vec3(0.6, 0.6, 0.6));
    glUseProgram(shader->program);
    TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_palarie));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 12);
}

void Lampa::RenderLampa(glm::mat4 Projection, glm::mat4 View) {
	RenderCorp(Projection, View);
	RenderPalarie(Projection, View);
}

void Lampa::RenderCorpShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
                              glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
                              glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    Shader* shader = shader_rotire_shadow;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.53, 0, 0);
    control_p1 = glm::vec3(-1.58, 0.5, 0);
    control_p2 = glm::vec3(-0.38, 1, 0);
    control_p3 = glm::vec3(-0.23, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 0.58, 0.2, 0.2);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("lamp_wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 12);
}

void Lampa::RenderPalarieShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
                                 glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
                                 glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    Shader* shader = shader_rotire_shadow;
    shader->Use();

    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(0, 0, 0);
    control_p1 = glm::vec3(-1.36, 1, 0);
    control_p2 = glm::vec3(-0.89, 3.5, 0);
    control_p3 = glm::vec3(-1.74, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 1, 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    glm::mat4 Model_palarie = Model;
    Model_palarie = glm::rotate(Model_palarie, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    Model_palarie = glm::translate(Model_palarie, glm::vec3(0.4, 4, 0));
    Model_palarie = glm::rotate(Model_palarie, glm::radians(100.0f), glm::vec3(0, 0, 1));
    Model_palarie = glm::scale(Model_palarie, glm::vec3(0.6, 0.6, 0.6));
    TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_palarie));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 12);
}

void Lampa::RenderLampaShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
                               glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
                               glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    RenderCorpShadows(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderPalarieShadows(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}

void Lampa::RenderCorpReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    
    Shader* shader = shader_rotire_reflection_p1;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.53, 0, 0);
    control_p1 = glm::vec3(-1.58, 0.5, 0);
    control_p2 = glm::vec3(-0.38, 1, 0);
    control_p3 = glm::vec3(-0.23, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 0.58, 0.2, 0.2);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("lamp_wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);



    shader = shader_rotire_reflection_p2;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-0.53, 0, 0);
    control_p1 = glm::vec3(-1.58, 0.5, 0);
    control_p2 = glm::vec3(-0.38, 1, 0);
    control_p3 = glm::vec3(-0.23, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 0.58, 0.2, 0.2);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("lamp_wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);


    shader = shader_rotire_reflection_p3;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-0.53, 0, 0);
    control_p1 = glm::vec3(-1.58, 0.5, 0);
    control_p2 = glm::vec3(-0.38, 1, 0);
    control_p3 = glm::vec3(-0.23, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 0.58, 0.2, 0.2);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("lamp_wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);
}

void Lampa::RenderPalarieReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_rotire_reflection_p1;
    shader->Use();

    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(0, 0, 0);
    control_p1 = glm::vec3(-1.36, 1, 0);
    control_p2 = glm::vec3(-0.89, 3.5, 0);
    control_p3 = glm::vec3(-1.74, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 1, 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    glm::mat4 Model_palarie = Model;
    Model_palarie = glm::rotate(Model_palarie, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    Model_palarie = glm::translate(Model_palarie, glm::vec3(0.4, 4, 0));
    Model_palarie = glm::rotate(Model_palarie, glm::radians(100.0f), glm::vec3(0, 0, 1));
    Model_palarie = glm::scale(Model_palarie, glm::vec3(0.6, 0.6, 0.6));
    TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_palarie));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);



    shader = shader_rotire_reflection_p2;
    shader->Use();

    control_p0 = glm::vec3(0, 0, 0);
    control_p1 = glm::vec3(-1.36, 1, 0);
    control_p2 = glm::vec3(-0.89, 3.5, 0);
    control_p3 = glm::vec3(-1.74, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 1, 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    Model_palarie = Model;
    Model_palarie = glm::rotate(Model_palarie, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    Model_palarie = glm::translate(Model_palarie, glm::vec3(0.4, 4, 0));
    Model_palarie = glm::rotate(Model_palarie, glm::radians(100.0f), glm::vec3(0, 0, 1));
    Model_palarie = glm::scale(Model_palarie, glm::vec3(0.6, 0.6, 0.6));
    TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_palarie));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);


    shader = shader_rotire_reflection_p3;
    shader->Use();

    control_p0 = glm::vec3(0, 0, 0);
    control_p1 = glm::vec3(-1.36, 1, 0);
    control_p2 = glm::vec3(-0.89, 3.5, 0);
    control_p3 = glm::vec3(-1.74, 4, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 1, 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    Model_palarie = Model;
    Model_palarie = glm::rotate(Model_palarie, glm::radians(-90.0f), glm::vec3(0, 1, 0));
    Model_palarie = glm::translate(Model_palarie, glm::vec3(0.4, 4, 0));
    Model_palarie = glm::rotate(Model_palarie, glm::radians(100.0f), glm::vec3(0, 0, 1));
    Model_palarie = glm::scale(Model_palarie, glm::vec3(0.6, 0.6, 0.6));
    TextureManager::GetTexture("white.png")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_palarie));
    loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

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
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, shadow_depth);
    glUniform1i(glGetUniformLocation(shader->program, "depth_texture"), 1);

    glUniformMatrix4fv(glGetUniformLocation(shader->GetProgramID(), "viewMatrices"), 6, GL_FALSE, glm::value_ptr(cubeView[0]));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 7);
}

void Lampa::RenderLampaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    RenderCorpReflection(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderPalarieReflection(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}
