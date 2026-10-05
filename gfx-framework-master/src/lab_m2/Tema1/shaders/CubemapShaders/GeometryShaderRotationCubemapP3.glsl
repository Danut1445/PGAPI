#version 330

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 78) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform vec3 color;
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
    color_of_fragment = vec4(color, 1);

    float step = 1.0f / no_of_generated_points;
    float step_ox = 1.0f / no_of_instances;
    float t = 6 * step;
    int nr_instance = instance[0];
    
    // Pentru Taskul 3:
    float radian_rot = radians(360.0f / no_of_instances);
    float angle1, angle2, angle3;
    vec3 vector_cross_1, vector_cross_2;
    vec3 p1, p2, p3, p4, p5, p6, p7, p8;

    for (int layer = 0; layer < 6; layer++) {
        gl_Layer = layer;
        for (int i = 6; i < no_of_generated_points; i++) {
            t = step * i;
            angle1 = radian_rot * nr_instance;
            angle2 = radian_rot * (nr_instance + 1);
            angle3 = radian_rot * (nr_instance + 2);
        
            p1 = rotateY(bezier(t), angle1);
            p2 = rotateY(bezier(t), angle2);
            p3 = rotateY(bezier(t + step), angle1);
            p4 = rotateY(bezier(t + step), angle2);
            p5 = rotateY(bezier(t), angle3);
            p6 = rotateY(bezier(t + step), angle3);
            p7 = rotateY(bezier(t + 2 * step), angle1);
            p8 = rotateY(bezier(t + 2 * step), angle2);

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(rotateY(bezier(t), angle1), 1);
            texture_coord = vec2(step_ox * nr_instance, t);
            world_position = vec3(Model * vec4(rotateY(bezier(t), angle1), 1));
            vector_cross_1 = p2 - p1;
            vector_cross_2 = p3 - p1;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(rotateY(bezier(t), angle2), 1);
            texture_coord = vec2(step_ox * (nr_instance + 1), t);
            world_position = vec3(Model * vec4(rotateY(bezier(t), angle2), 1));
            vector_cross_1 = p5 - p2;
            vector_cross_2 = p6 - p2;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(rotateY(bezier(t + step), angle1), 1);
            texture_coord = vec2(step_ox * nr_instance, t + step);
            world_position = vec3(Model * vec4(rotateY(bezier(t + step), angle1), 1));
            vector_cross_1 = p4 - p3;
            vector_cross_2 = p7 - p3;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();

            gl_Position = Projection * viewMatrices[layer] * Model * vec4(rotateY(bezier(t + step), angle2), 1);
            texture_coord = vec2(step_ox * (nr_instance + 1), t + step);
            world_position = vec3(Model * vec4(rotateY(bezier(t + step), angle2), 1));
            vector_cross_1 = p6 - p4;
            vector_cross_2 = p8 - p4;
            world_normal = cross(vector_cross_2, vector_cross_1);
            EmitVertex();
            EndPrimitive();

            t = t + step;
        }
    }

}
