#version 330

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(triangle_strip, max_vertices = 128) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 control_p0, control_p1, control_p2, control_p3;
uniform int no_of_instances;
// TODO(student): Declare any other uniforms here
uniform int no_of_generated_points;

// Input
in int instance[2];

// Output
out vec4 color_of_fragment;


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

    // TODO(student): Rather than emitting vertices for the control
    // points, you must emit vertices that approximate the curve itself.
    // gl_Position = Projection * View * vec4(control_p0, 1);   EmitVertex();
    // gl_Position = Projection * View * vec4(control_p1, 1);   EmitVertex();
    // gl_Position = Projection * View * vec4(control_p2, 1);   EmitVertex();
    // gl_Position = Projection * View * vec4(control_p3, 1);   EmitVertex();

    float step = 1.0f / no_of_generated_points;
    float t = 0;
    int nr_instance = instance[0];

    // Pentru Curba Bezier:
    // for (int i = 0; i < no_of_generated_points; i++) {
    //    gl_Position = Projection * View * vec4(bezier(t), 1);
    //    EmitVertex();
    //    t = t + step;
    // }

    // Pentru Task 2:
    for (int i = 0; i < no_of_generated_points; i++) {
        gl_Position = Projection * View * vec4(bezier(t) + vec3((4.0f / no_of_instances) * nr_instance, 0, 0), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t) + vec3((4.0f / no_of_instances) * (nr_instance + 1), 0, 0), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3((4.0f / no_of_instances) * nr_instance, 0, 0), 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(bezier(t) + vec3((4.0f / no_of_instances) * (nr_instance + 1), 0, 0), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3((4.0f / no_of_instances) * (nr_instance + 1), 0, 0), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3((4.0f / no_of_instances) * nr_instance, 0, 0), 1);
        EmitVertex();

        t = t + step;
    }

    
    // Pentru Taskul 3:
    float radian_rot = radians(360.0f / no_of_instances);
    float sin1, sin2, cos1, cos2;

/*
    for (int i = 0; i < no_of_generated_points; i++) {
        sin1 = sin(radian_rot * nr_instance);
        sin2 = sin(radian_rot * (nr_instance + 1));
        cos1 = cos(radian_rot * nr_instance);
        cos2 = cos(radian_rot * (nr_instance + 1));
        gl_Position = Projection * View * vec4(bezier(t) + vec3(2.0f * sin1, 0, 2.0f * cos1), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t) + vec3(2.0f * sin2, 0, 2.0f * cos2), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3(2.0f * sin1, 0, 2.0f * cos1), 1);
        EmitVertex();

        gl_Position = Projection * View * vec4(bezier(t) + vec3(2.0f * sin2, 0, 2.0f * cos2), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3(2.0f * sin2, 0, 2.0f * cos2), 1);
        EmitVertex();
        gl_Position = Projection * View * vec4(bezier(t + step) + vec3(2.0f * sin1, 0, 2.0f * cos1), 1);
        EmitVertex();
        t = t + step;
    }
*/
    float angle1, angle2;
/*
    for (int i = 0; i < no_of_generated_points; i++) {
        angle1 = radian_rot * nr_instance;
        angle2 = radian_rot * (nr_instance + 1);

        gl_Position = Projection * View * vec4(rotateY(bezier(t), angle1), 1);
        if (angle1 < 2) {
            color_of_fragment = vec4((1 - angle1 / 2), angle1 / 2, 0, 0);
        } else if (angle1 < 4) {
            color_of_fragment = vec4(0, 2 - angle1 / 2, angle1 / 2 - 1, 0);
        } else if (angle1 < 6) {
            color_of_fragment = vec4(angle1 / 2 - 2, 0, 3 - angle1 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();

        gl_Position = Projection * View * vec4(rotateY(bezier(t), angle2), 1);
        if (angle2 < 2) {
            color_of_fragment = vec4((1 - angle2 / 2), angle2 / 2, 0, 0);
        } else if (angle2 < 4) {
            color_of_fragment = vec4(0, 2 - angle2 / 2, angle2 / 2 - 1, 0);
        } else if (angle2 < 6) {
            color_of_fragment = vec4(angle2 / 2 - 2, 0, 3 - angle2 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();

        gl_Position = Projection * View * vec4(rotateY(bezier(t + step), angle1), 1);
        if (angle1 < 2) {
            color_of_fragment = vec4((1 - angle1 / 2), angle1 / 2, 0, 0);
        } else if (angle1 < 4) {
            color_of_fragment = vec4(0, 2 - angle1 / 2, angle1 / 2 - 1, 0);
        } else if (angle1 < 6) {
            color_of_fragment = vec4(angle1 / 2 - 2, 0, 3 - angle1 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();
        EndPrimitive();

        gl_Position = Projection * View * vec4(rotateY(bezier(t), angle2), 1);
        if (angle2 < 2) {
            color_of_fragment = vec4((1 - angle2 / 2), angle2 / 2, 0, 0);
        } else if (angle2 < 4) {
            color_of_fragment = vec4(0, 2 - angle2 / 2, angle2 / 2 - 1, 0);
        } else if (angle2 < 6) {
            color_of_fragment = vec4(angle2 / 2 - 2, 0, 3 - angle2 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();

        gl_Position = Projection * View * vec4(rotateY(bezier(t + step), angle2), 1);
        if (angle2 < 2) {
            color_of_fragment = vec4((1 - angle2 / 2), angle2 / 2, 0, 0);
        } else if (angle2 < 4) {
            color_of_fragment = vec4(0, 2 - angle2 / 2, angle2 / 2 - 1, 0);
        } else if (angle2 < 6) {
            color_of_fragment = vec4(angle2 / 2 - 2, 0, 3 - angle2 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();

        gl_Position = Projection * View * vec4(rotateY(bezier(t + step), angle1), 1);
        if (angle1 < 2) {
            color_of_fragment = vec4((1 - angle1 / 2), angle1 / 2, 0, 0);
        } else if (angle1 < 4) {
            color_of_fragment = vec4(0, 2 - angle1 / 2, angle1 / 2 - 1, 0);
        } else if (angle1 < 6) {
            color_of_fragment = vec4(angle1 / 2 - 2, 0, 3 - angle1 / 2, 0);
        } else {
            color_of_fragment = vec4(0, 0, 0, 1);
        }
        EmitVertex();
        EndPrimitive();

        t = t + step;
    }
*/
}
