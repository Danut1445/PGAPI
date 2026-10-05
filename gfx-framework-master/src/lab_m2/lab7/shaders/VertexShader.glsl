#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
// TODO (student): Add the new attributes neccessary for skinning
layout(location = 3) in ivec4 v_bone_ids;      // Bone indices affecting this vertex
layout(location = 4) in vec4 v_bone_weights;   // Corresponding bone weights

const int MAX_BONES = 200;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
// TODO (student): Declare a new uniform variable array, which will
// receive the bone transformations
uniform mat4 BoneTransforms[MAX_BONES];

// Output
out vec2 texture_coord;
// TODO (student): Send the normal to the fragment shader
out vec3 frag_normal;

void main()
{
    // TODO (student): Compute the final bone transformation
    mat4 skin_matrix = 
        v_bone_weights[0] * BoneTransforms[v_bone_ids[0]] +
        v_bone_weights[1] * BoneTransforms[v_bone_ids[1]] +
        v_bone_weights[2] * BoneTransforms[v_bone_ids[2]] +
        v_bone_weights[3] * BoneTransforms[v_bone_ids[3]];

    texture_coord = v_texture_coord;
    // TODO (student): Compute the normal
    frag_normal = mat3(skin_matrix) * v_normal;

    // TODO (student): Apply the bone transformation on the vertex position
    vec4 skinned_position = skin_matrix * vec4(v_position, 1.0);
    gl_Position = Projection * View * Model * skinned_position;
}