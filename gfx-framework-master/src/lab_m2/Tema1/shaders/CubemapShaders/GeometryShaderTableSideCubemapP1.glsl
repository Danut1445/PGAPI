#version 330

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 76) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
// TODO(student): Declare any other uniforms here
uniform int no_of_generated_points;
uniform mat4 viewMatrices[6];

// Input
in int instance[2];

// Output
out vec4 color_of_fragment;
out vec2 texture_coord;
out vec3 world_position;
out vec3 world_normal;


vec3 rotateY(vec3 point, float u)
{
    float x = point.x * cos(u) - point.z *sin(u);
    float z = point.x * sin(u) + point.z *cos(u);
    return vec3(x, point.y, z);
}


vec3 translateX(vec3 point, float t)
{
    return vec3(point.x + t, point.y, point.z);
}


// This function computes B(t) with 4 control points. For a visual example,
// see [1]. For an interactive Javascript example with the exact points in
// this code, see [2].
//
// [1] https://www.desmos.com/calculator/cahqdxeshd
// [2] https://jsfiddle.net/6yuv9htf/
vec3 bezier(float t)
{
    return  control_p0 * pow((1 - t), 3) +
            control_p1 * 3 * t * pow((1 - t), 2) +
            control_p2 * 3 * pow(t, 2) * (1 - t) +
            control_p3 * pow(t, 3);
}


// TODO(student): If you want to take things a step further, try drawing a
// Hermite spline. Hint: you can repurpose two of the control points. For a
// visual example, see [1]. For an interactive Javascript example with the
// exact points in this code, see [2].
// 
// Unlike the Javascript function, you MUST NOT call the Bezier function.
// There is another way to draw a Hermite spline, all you need is to find
// the formula.
//
// [1] https://www.desmos.com/calculator/5knm5tkr8m
// [2] https://jsfiddle.net/6yuv9htf/

void main()
{
    const int SURFACE_TYPE_ROTATION     = 0;
    const int SURFACE_TYPE_TRANSLATION  = 1;
    color_of_fragment = vec4(1, 0, 0, 1);

    float step = 1.0f / no_of_generated_points;
    float step_ox = 1.0f / no_of_instances;
    vec3 step_high_x, step_low_x, step_high_z, step_low_z;
    vec3 point_low, point_high;
    float t = 0;
    int nr_instance = instance[0];
    vec3 vector_cross_1, vector_cross_2;

    // Pentru Task 2:
    for (int layer = 0; layer < 6; layer++) {
        gl_Layer = layer;
        for (int i = 0; i < 2; i++) {
            t = i * step;
            point_low = bezier(t);
            point_high = bezier(t + step);
            step_low_x = vec3(2 * -point_low.x, 0, 0) / no_of_instances;
            step_high_x = vec3(2 * -point_high.x, 0, 0) / no_of_instances;
            step_low_z = vec3(0, 0, 2 * -point_low.z) / no_of_instances;
            step_high_z = vec3(0, 0, 2 * -point_high.z) / no_of_instances;


            //This is for OX cover of table
            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_low + step_low_x * nr_instance, 1);
            texture_coord = vec2(nr_instance * step_ox, t);
            world_position = vec3(Model * vec4(point_low + step_low_x * nr_instance, 1));
            vector_cross_1 = step_low_x;
            vector_cross_2 = point_high - point_low;
            world_normal = cross(vector_cross_1, vector_cross_2);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_low + step_low_x * (nr_instance + 1), 1);
            texture_coord = vec2((nr_instance + 1) * step_ox, t);
            world_position = vec3(Model * vec4(point_low + step_low_x * (nr_instance + 1), 1));
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_high + step_high_x * nr_instance, 1);
            texture_coord = vec2(nr_instance * step_ox, t + step);
            world_position = vec3(Model * vec4(point_high + step_high_x * nr_instance, 1));
            vector_cross_1 = step_high_x;
            vector_cross_2 = bezier(t + 2 * step) - point_high;
            world_normal = cross(vector_cross_1, vector_cross_2);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_high + step_high_x * (nr_instance + 1), 1);
            texture_coord = vec2((nr_instance + 1) * step_ox, t + step);
            world_position = vec3(Model * vec4(point_high + step_high_x * (nr_instance + 1), 1));
            EmitVertex();
            EndPrimitive();

            // This is for OZ cover of table
            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_low + step_low_z * nr_instance, 1);
            texture_coord = vec2(nr_instance * step_ox, t);
            world_position = vec3(Model * vec4(point_low + step_low_z * nr_instance, 1));
            vector_cross_1 = step_low_z;
            vector_cross_2 = point_high - point_low;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_low + step_low_z * (nr_instance + 1), 1);
            texture_coord = vec2((nr_instance + 1) * step_ox, t);
            world_position = vec3(Model * vec4(point_low + step_low_z * (nr_instance + 1), 1));
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_high + step_high_z * nr_instance, 1);
            texture_coord = vec2(nr_instance * step_ox, t + step);
            world_position = vec3(Model * vec4(point_high + step_high_z * nr_instance, 1));
            vector_cross_1 = step_high_z;
            vector_cross_2 = bezier(t + 2 * step) - point_high;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(point_high + step_high_z * (nr_instance + 1), 1);
            texture_coord = vec2((nr_instance + 1) * step_ox, t + step);
            world_position = vec3(Model * vec4(point_high + step_high_z * (nr_instance + 1), 1));
            EmitVertex();
            EndPrimitive();

            t = t + step;
        }
    }
}
