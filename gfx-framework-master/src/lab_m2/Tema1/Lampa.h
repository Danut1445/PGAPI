#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"
#include "components/simple_scene.h"
#include <core/gpu/shader.h>

namespace lampa {
	class Lampa {
	public:
		Lampa(Shader *shader_rotire, Shader *shader_rotire_shadow, Shader *shader_rotire_reflection_p1,
			Shader* shader_rotire_reflection_p2, Shader* shader_rotire_reflection_p3, Mesh* mesh, glm::vec3 location);
		void RenderCorp(glm::mat4 Projection, glm::mat4 View);
		void RenderPalarie(glm::mat4 Projection, glm::mat4 View);
		void RenderLampa(glm::mat4 Projection, glm::mat4 View);
		void RenderCorpShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
							   glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
							   glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderPalarieShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
								  glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
								  glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderLampaShadows(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
								glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
								glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderCorpReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderPalarieReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderLampaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);

	public:
		Shader* shader_rotire, *shader_rotire_shadow, *shader_rotire_reflection_p1, *shader_rotire_reflection_p2, * shader_rotire_reflection_p3;
		Mesh* nonimportant_mesh;
		glm::mat4 Model;
		glm::vec3 unghi_palarie;
	};
}
