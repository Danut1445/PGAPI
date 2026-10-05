#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"
#include "core/gpu/mesh.h"
#include "components/simple_scene.h"
#include <core/gpu/shader.h>

namespace masa {
	class Masa {
	public:
		Masa(Shader* shader_latura, Shader* shader_top, Shader* shader_picioare,
			Shader* shader_latura_shadow, Shader* shader_top_shadow, Shader* shader_picioare_shadow, Shader* shader_top_reflection,
			Shader* shader_picioare_reflect_p1, Shader* shader_picioare_reflect_p2, Shader* shader_picioare_reflect_p3,
			Shader* shader_latura_reflection_p1, Shader* shader_latura_reflection_p2, Shader* shader_latura_reflection_p3,
			Mesh* mesh, glm::vec3 location);
		void RenderTop(glm::mat4 Projection, glm::mat4 View);
		void RenderLaterala(glm::mat4 Projection, glm::mat4 View);
		void RenderPicioare(glm::mat4 Projection, glm::mat4 View);
		void RenderMasa(glm::mat4 Projection, glm::mat4 View);
		void RenderUnPicior(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior);
		void RenderTopShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
							 glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
							 glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderLateralaShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
								  glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
								  glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderPicioareShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
								  glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
								  glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderMasaShadow(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
							  glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
							  glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderUnPiciorShadow(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior, unsigned int shadow_depth,
								  glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
								  glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderTopReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderUnPiciorReflection(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model_Picior, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderPicioareReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderMasaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);
		void RenderLateralaReflection(glm::mat4 Projection, glm::mat4 View, unsigned int shadow_depth,
			glm::vec3 eye_position, glm::vec3 light_position, glm::vec3 light_direction,
			glm::vec3 light_color, glm::mat4 light_space_view, glm::mat4 light_space_projection, float light_intensity);

	public:
		Shader *shader_latura, *shader_top, *shader_picioare, *shader_latura_shadow, *shader_top_shadow, *shader_picioare_shadow;
		Shader* shader_top_reflection, *shader_picioare_reflect_p1, *shader_picioare_reflect_p2, *shader_picioare_reflect_p3;
		Shader* shader_latura_reflection_p1, * shader_latura_reflection_p2, * shader_latura_reflection_p3;
		Mesh* nonimportant_mesh;
		glm::mat4 Model;
	};
}
