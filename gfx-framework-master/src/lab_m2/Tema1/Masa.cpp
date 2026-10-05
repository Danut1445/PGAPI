#include "Masa.h"
#include <iostream>

using namespace std;
using namespace masa;

Masa::Masa(Shader* shader_latura, Shader* shader_top, Shader* shader_picioare,
    Shader* shader_latura_shadow, Shader* shader_top_shadow, Shader* shader_picioare_shadow, Shader* shader_top_reflection,
    Shader* shader_picioare_reflect_p1, Shader* shader_picioare_reflect_p2, Shader* shader_picioare_reflect_p3,
    Shader* shader_latura_reflection_p1, Shader* shader_latura_reflection_p2, Shader* shader_latura_reflection_p3,
    Mesh* mesh, glm::vec3 location) {

	this->shader_latura = shader_latura;
	this->shader_top = shader_top;
	this->shader_picioare = shader_picioare;
    this->nonimportant_mesh = mesh;
    this->shader_latura_shadow = shader_latura_shadow;
    this->shader_picioare_shadow = shader_picioare_shadow;
    this->shader_top_shadow = shader_top_shadow;
    this->shader_top_reflection = shader_top_reflection;
    this->shader_picioare_reflect_p1 = shader_picioare_reflect_p1;
    this->shader_picioare_reflect_p2 = shader_picioare_reflect_p2;
    this->shader_picioare_reflect_p3 = shader_picioare_reflect_p3;
    this->shader_latura_reflection_p1 = shader_latura_reflection_p1;
    this->shader_latura_reflection_p2 = shader_latura_reflection_p2;
    this->shader_latura_reflection_p3 = shader_latura_reflection_p3;
    Model = glm::translate(glm::mat4(1), location);
    Model = glm::scale(Model, glm::vec3(0.5, 0.5, 0.5));
}

void Masa::RenderTop(glm::mat4 Projection, glm::mat4 View) {
    Shader* shader = shader_top;
    shader->Use();

    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.6, 1, 4);

    // Send uniforms to shaders
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUseProgram(shader->program);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Masa::RenderLaterala(glm::mat4 Projection, glm::mat4 View) {
    Shader* shader = shader_latura;
    shader_latura->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.4, 0.3, 3.8);
    control_p2 = glm::vec3(-7.4, 0.6, 3.8);
    control_p3 = glm::vec3(-7.6, 1, 4);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 10);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 7);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 10);

    control_p0 = glm::vec3(-control_p0.x, control_p0.y, -control_p0.z);
    control_p1 = glm::vec3(-control_p1.x, control_p1.y, -control_p1.z);
    control_p2 = glm::vec3(-control_p2.x, control_p2.y, -control_p2.z);
    control_p3 = glm::vec3(-control_p3.x, control_p3.y, -control_p3.z);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 10);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 7);

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 10);
}

void Masa::RenderUnPicior(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior) {
    Shader* shader = shader_picioare;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.4, 0, 0);
    control_p1 = glm::vec3(-0.8, 3, 0);
    control_p2 = glm::vec3(-0.14, 4, 0);
    control_p3 = glm::vec3(-0.53, 6, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_Picior));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 12);
}

void Masa::RenderPicioare(glm::mat4 Projection, glm::mat4 View) {
    RenderUnPicior(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, 3.2)));
    RenderUnPicior(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, 3.2)));
    RenderUnPicior(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, -3.2)));
    RenderUnPicior(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, -3.2)));
}

void Masa::RenderMasa(glm::mat4 Projection, glm::mat4 View) {
    RenderLaterala(Projection, View);
    RenderTop(Projection, View);
    RenderPicioare(Projection, View);
}

void Masa::RenderTopShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_top_shadow;
    shader->Use();

    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.6, 1, 4);

    // Send uniforms to shaders
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUseProgram(shader->program);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Masa::RenderLateralaShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_latura_shadow;
    shader_latura_shadow->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.4, 0.3, 3.8);
    control_p2 = glm::vec3(-7.4, 0.6, 3.8);
    control_p3 = glm::vec3(-7.6, 1, 4);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 10);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 7);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 10);


    control_p0 = glm::vec3(-control_p0.x, control_p0.y, -control_p0.z);
    control_p1 = glm::vec3(-control_p1.x, control_p1.y, -control_p1.z);
    control_p2 = glm::vec3(-control_p2.x, control_p2.y, -control_p2.z);
    control_p3 = glm::vec3(-control_p3.x, control_p3.y, -control_p3.z);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 10);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 7);

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
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

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 10);
}

void Masa::RenderUnPiciorShadow(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {


    Shader* shader = shader_picioare_shadow;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.4, 0, 0);
    control_p1 = glm::vec3(-0.8, 3, 0);
    control_p2 = glm::vec3(-0.14, 4, 0);
    control_p3 = glm::vec3(-0.53, 6, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 12);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_Picior));
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

void Masa::RenderPicioareShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    RenderUnPiciorShadow(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, 3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorShadow(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, 3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorShadow(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, -3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorShadow(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, -3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}

void Masa::RenderMasaShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {
    RenderLateralaShadow(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderTopShadow(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderPicioareShadow(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}

void Masa::RenderTopReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_top_reflection;
    shader->Use();

    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.6, 1, 4);

    // Send uniforms to shaders
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUseProgram(shader->program);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void Masa::RenderUnPiciorReflection(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {


    Shader* shader = shader_picioare_reflect_p1;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-0.4, 0, 0);
    control_p1 = glm::vec3(-0.8, 3, 0);
    control_p2 = glm::vec3(-0.14, 4, 0);
    control_p3 = glm::vec3(-0.53, 6, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    Mesh* mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_Picior));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 9);


    
    shader = shader_picioare_reflect_p2;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-0.4, 0, 0);
    control_p1 = glm::vec3(-0.8, 3, 0);
    control_p2 = glm::vec3(-0.14, 4, 0);
    control_p3 = glm::vec3(-0.53, 6, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_Picior));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 9);


    shader = shader_picioare_reflect_p3;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-0.4, 0, 0);
    control_p1 = glm::vec3(-0.8, 3, 0);
    control_p2 = glm::vec3(-0.14, 4, 0);
    control_p3 = glm::vec3(-0.53, 6, 0);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform3f(glGetUniformLocation(shader->program, "color"), 1, 0, 0);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 7);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 9);

    mesh = nonimportant_mesh;
    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model_Picior));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 9);
}

void Masa::RenderLateralaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_latura_reflection_p1;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.4, 0.3, 3.8);
    control_p2 = glm::vec3(-7.4, 0.6, 3.8);
    control_p3 = glm::vec3(-7.6, 1, 4);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);


    control_p0 = glm::vec3(-control_p0.x, control_p0.y, -control_p0.z);
    control_p1 = glm::vec3(-control_p1.x, control_p1.y, -control_p1.z);
    control_p2 = glm::vec3(-control_p2.x, control_p2.y, -control_p2.z);
    control_p3 = glm::vec3(-control_p3.x, control_p3.y, -control_p3.z);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
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

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);



    shader = shader_latura_reflection_p2;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.4, 0.3, 3.8);
    control_p2 = glm::vec3(-7.4, 0.6, 3.8);
    control_p3 = glm::vec3(-7.6, 1, 4);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);


    control_p0 = glm::vec3(-control_p0.x, control_p0.y, -control_p0.z);
    control_p1 = glm::vec3(-control_p1.x, control_p1.y, -control_p1.z);
    control_p2 = glm::vec3(-control_p2.x, control_p2.y, -control_p2.z);
    control_p3 = glm::vec3(-control_p3.x, control_p3.y, -control_p3.z);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
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

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);




    shader = shader_latura_reflection_p3;
    shader->Use();

    // Send uniforms to shaders
    control_p0 = glm::vec3(-7.6, 0, 4);
    control_p1 = glm::vec3(-7.4, 0.3, 3.8);
    control_p2 = glm::vec3(-7.4, 0.6, 3.8);
    control_p3 = glm::vec3(-7.6, 1, 4);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);


    control_p0 = glm::vec3(-control_p0.x, control_p0.y, -control_p0.z);
    control_p1 = glm::vec3(-control_p1.x, control_p1.y, -control_p1.z);
    control_p2 = glm::vec3(-control_p2.x, control_p2.y, -control_p2.z);
    control_p3 = glm::vec3(-control_p3.x, control_p3.y, -control_p3.z);

    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 5);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 6);

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("wood.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 6, 0))));
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
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

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 5);
}

void Masa::RenderPicioareReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    RenderUnPiciorReflection(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, 3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorReflection(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, 3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorReflection(Projection, View, glm::translate(Model, glm::vec3(-6.8, 0, -3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderUnPiciorReflection(Projection, View, glm::translate(Model, glm::vec3(6.8, 0, -3.2)), shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}

void Masa::RenderMasaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    RenderLateralaReflection(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderTopReflection(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderPicioareReflection(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
}
