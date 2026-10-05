#version 330

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 90) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
uniform vec3 control_p0, control_p1;
uniform int no_of_instances;
// TODO(student): Declare any other uniforms here
uniform int no_of_generated_points;

// Input
in int instance[2];

// Output
out vec4 color_of_fragment;
out vec2 texture_coord;
out vec3 world_position;
out vec3 world_normal;

void main()
{
    color_of_fragment = vec4(1, 0, 1, 1);
    float x, z, high, low;
    x = control_p0.x;
    z = control_p0.z;
    high = control_p1.y;
    low = control_p0.y;


    //Bottom part:
    world_normal = mat3(Model) * vec3(0, -1, 0);
    gl_Position = Projection * View * Model * vec4(x, low, z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_position = vec3(Model * vec4(x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, low, z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_position = vec3(Model * vec4(-x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, low, -z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_position = vec3(Model * vec4(x, low, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, low, -z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_position = vec3(Model * vec4(-x, low, -z, 1));
    EmitVertex();
    EndPrimitive();


    //Top part:
    world_normal = mat3(Model) * vec3(0, 1, 0);
    gl_Position = Projection * View * Model * vec4(x, high, z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_position = vec3(Model * vec4(x, high, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_position = vec3(Model * vec4(-x, high, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, high, -z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_position = vec3(Model * vec4(x, high, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, -z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_position = vec3(Model * vec4(-x, high, -z, 1));
    EmitVertex();
    EndPrimitive();


    //Pozitive OZ part:
    world_normal = mat3(Model) * vec3(0, 0, 1);
    gl_Position = Projection * View * Model * vec4(x, low, z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_position = vec3(Model * vec4(x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, low, z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_position = vec3(Model * vec4(-x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, high, z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_position = vec3(Model * vec4(x, high, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_position = vec3(Model * vec4(-x, high, z, 1));
    EmitVertex();
    EndPrimitive();


    //Negative OZ part:
    world_normal = mat3(Model) * vec3(0, 0, -1);
    gl_Position = Projection * View * Model * vec4(x, low, -z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_position = vec3(Model * vec4(x, low, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, low, -z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_position = vec3(Model * vec4(-x, low, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, high, -z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_position = vec3(Model * vec4(x, high, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, -z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_position = vec3(Model * vec4(-x, high, -z, 1));
    EmitVertex();
    EndPrimitive();


    //Negative OX part:
    world_normal = mat3(Model) * vec3(-1, 0, 0);
    gl_Position = Projection * View * Model * vec4(x, low, z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_position = vec3(Model * vec4(x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, low, -z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_position = vec3(Model * vec4(x, low, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, high, z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_position = vec3(Model * vec4(x, high, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(x, high, -z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_position = vec3(Model * vec4(x, high, -z, 1));
    EmitVertex();
    EndPrimitive();


    //Pozitive OX part:
    world_normal = mat3(Model) * vec3(1, 0, 0);
    gl_Position = Projection * View * Model * vec4(-x, low, z, 1);
    texture_coord = vec2(0.0, 0.0);
    world_normal = vec3(Model * vec4(-x, low, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, low, -z, 1);
    texture_coord = vec2(0.0, 1.0);
    world_normal = vec3(Model * vec4(-x, low, -z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, z, 1);
    texture_coord = vec2(1.0, 0.0);
    world_normal = vec3(Model * vec4(-x, high, z, 1));
    EmitVertex();
    gl_Position = Projection * View * Model * vec4(-x, high, -z, 1);
    texture_coord = vec2(1.0, 1.0);
    world_normal = vec3(Model * vec4(-x, high, -z, 1));
    EmitVertex();
    EndPrimitive();
}
