#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"
#include "components/simple_scene.h"
#include <core/gpu/shader.h>

namespace vaza {
	class Vaza {
	public:
		Vaza(Shader *shader_rotire, Shader* shader_shadow, 
			Shader* shader_rotire_reflection_p1, Shader* shader_rotire_reflection_p2, Shader* shader_rotire_reflection_p3, Mesh* mesh, glm::vec3 location);
		void RenderVaza(glm::mat4 Projection, glm::mat4 View);
		void RenderVazaShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderVazaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);

	public:
		Shader* shader_rotire, *shader_shadow, *shader_rotire_reflection_p1, *shader_rotire_reflection_p2, *shader_rotire_reflection_p3;
		Mesh* nonimportant_mesh;
		glm::mat4 Model;
	};
}
