#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"
#include "components/simple_scene.h"
#include <core/gpu/shader.h>

namespace tv {
	class TV {
	public:
		TV(Shader *shader_body, Shader* shader_ecran, Shader* shadow, Mesh* mesh, glm::vec3 location);
		void RenderBody(glm::mat4 Projection, glm::mat4 View);
		void RenderEcran(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int cubemap, glm::vec3 eye_position);
		void RenderTV(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int cubemap, glm::vec3 eye_position);
		void RenderBodyShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderTVShadow(glm::mat4 Projection, glm::mat4 View, unsigned int texture_id_screen, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity, unsigned int cubemap);
		void RotateTV(float angle);

	public:
		Shader *shader_body, *shader_ecran, *shader_shadow;
		Mesh* nonimportant_mesh;
		glm::mat4 Model;
	};
}
