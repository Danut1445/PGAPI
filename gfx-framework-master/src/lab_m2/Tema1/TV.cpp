#include "TV.h"

using namespace std;
using namespace tv;

TV::TV(Shader* shader_body, Shader* shader_ecran, Shader* shadow, Mesh* mesh, glm::vec3 location) {
	this->shader_body = shader_body;
	this->shader_ecran = shader_ecran;
	this->Model = glm::mat4(1);
    this->shader_shadow = shadow;
	this->nonimportant_mesh = mesh;
    this->Model = glm::translate(this->Model, location);
    this->Model = glm::scale(this->Model, glm::vec3(0.3, 0.3, 0.3));
}

void TV::RenderBody(glm::mat4 Projection, glm::mat4 View) {
    Shader* shader = shader_body;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-3, 0, 2);
    control_p1 = glm::vec3(-3, 4.6, 2);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("grey.jpg")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(Model));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void TV::RenderEcran(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int cubemap, glm::vec3 eye_position) {
    Shader* shader = shader_ecran;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-2.6, 0.5, 0);
    control_p1 = glm::vec3(-3, 3, 0);
    control_p2 = glm::vec3(-2.7, 4, 0);
    control_p3 = glm::vec3(-2.5, 4.1, 0);
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
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_id_screen);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(glm::translate(Model, glm::vec3(0, 0, 2))));
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(View));
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(Projection));
    GLint eye_position_loc = glGetUniformLocation(shader->program, "camera_position");
    glUniform3f(eye_position_loc, eye_position.x, eye_position.y, eye_position.z);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);
    int loc_texture = shader->GetUniformLocation("texture_cubemap");
    glUniform1i(loc_texture, 1);

    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 10);
}

void TV::RenderTV(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int cubemap, glm::vec3 eye_position) {
	RenderBody(Projection, View);
	RenderEcran(Projection, View, texture_id_screen, cubemap, eye_position);
}

void TV::RenderBodyShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity) {

    Shader* shader = shader_shadow;
    shader->Use();

    // Send uniforms to shaders
    glm::vec3 control_p0, control_p1, control_p2, control_p3;
    control_p0 = glm::vec3(-3, 0, 2);
    control_p1 = glm::vec3(-3, 4.6, 2);
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), 1);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), 1);

    Mesh* mesh = nonimportant_mesh;

    // Draw the object instanced
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);
    TextureManager::GetTexture("grey.jpg")->BindToTextureUnit(GL_TEXTURE0);
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
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, 1);
}

void TV::RenderTVShadow(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int shadow_depth,
    glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
    glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity, unsigned int cubemap) {

    RenderBodyShadow(Projection, View, shadow_depth, eye_position, light_position, light_direction, light_color, light_space_view, light_space_projection, light_intensity);
    RenderEcran(Projection, View, texture_id_screen, cubemap, eye_position);
}

void TV::RotateTV(float angle) {
    Model = glm::rotate(Model, glm::radians(angle), glm::vec3(0, 1, 0));
}
