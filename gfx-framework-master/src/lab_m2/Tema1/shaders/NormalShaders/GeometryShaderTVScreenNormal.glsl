#version 330

// Input and output topologies
// TODO(student): First, generate a curve (via line strip),
// then a rotation/translation surface (via triangle strip)
layout(lines) in;
layout(line_strip, max_vertices = 90) out;

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;
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
    color_of_fragment = vec4(0, 1, 0, 1);

    float step = 1.0f / no_of_generated_points;
    float t = 0;
    int nr_instance = instance[0];
    vec3 point_low, point_high, ideal_circle_low_1, ideal_circle_high_1, ideal_circle_high_2, ideal_circle_low_2;
    vec3 p1, p2, p3, p4, vector_normal_1, vector_normal_2, norm;
    
    // Pentru Taskul 3:
    float radian_rot = radians(-180.0f / no_of_instances);
    float angle1, angle2, angle3;

    for (int i = 0; i < no_of_generated_points; i++) {
        angle1 = radian_rot * nr_instance;
        angle2 = radian_rot * (nr_instance + 1);
        angle3 = radian_rot * (nr_instance + 2);
        point_high = bezier(t + step);
        point_low = bezier(t);
        ideal_circle_high_1 =  rotateY(point_high, angle1);
        ideal_circle_low_1 =  rotateY(point_low, angle1);
        ideal_circle_high_2 =  rotateY(point_high, angle2);
        ideal_circle_low_2 =  rotateY(point_low, angle2);
        p1 = rotateY(bezier(t + 2*step), angle1);
        p2 = rotateY(bezier(t + 2*step), angle2);
        p3 = rotateY(point_low, angle3);
        p4 = rotateY(point_high, angle3);
        ideal_circle_low_1 = vec3(ideal_circle_low_1.x, ideal_circle_low_1.y, point_low.z + (ideal_circle_low_1.z - point_low.z) * 0.2);
        ideal_circle_low_2 = vec3(ideal_circle_low_2.x, ideal_circle_low_2.y, point_low.z + (ideal_circle_low_2.z - point_low.z) * 0.2);
        ideal_circle_high_1 = vec3(ideal_circle_high_1.x, ideal_circle_high_1.y, point_high.z + (ideal_circle_high_1.z - point_high.z) * 0.2);
        ideal_circle_high_2 = vec3(ideal_circle_high_2.x, ideal_circle_high_2.y, point_high.z + (ideal_circle_high_2.z - point_high.z) * 0.2);
        p1 = vec3(p1.x, p1.y, (bezier(t + 2 * step)).z + (p1.z - (bezier(t + 2 * step)).z) * 0.2);
        p2 = vec3(p2.x, p2.y, (bezier(t + 2  *step)).z + (p2.z - (bezier(t + 2 * step)).z) * 0.2);
        p3 = vec3(p3.x, p3.y, point_low.z + (p3.z - point_low.z) * 0.2);
        p4 = vec3(p4.x, p4.y, point_low.z + (p4.z - point_low.z) * 0.2);

        vector_normal_1 = ideal_circle_low_2 - ideal_circle_low_1;
        vector_normal_2 = ideal_circle_high_1 - ideal_circle_low_1;
        norm = cross(vector_normal_1, vector_normal_2);
        gl_Position = Projection * View * Model * vec4(ideal_circle_low_1, 1);
        EmitVertex();
        gl_Position = Projection * View * Model * vec4(ideal_circle_low_1 + 2 * norm, 1);
        EmitVertex();
        EndPrimitive();

        vector_normal_1 = p3 - ideal_circle_low_2;
        vector_normal_2 = ideal_circle_high_2 - ideal_circle_low_2;
        norm = cross(vector_normal_1, vector_normal_2);
        gl_Position = Projection * View * Model * vec4(ideal_circle_low_2, 1);
        EmitVertex();
        gl_Position = Projection * View * Model * vec4(ideal_circle_low_2 + 2 * norm, 1);
        EmitVertex();
        EndPrimitive();
        
        vector_normal_1 = ideal_circle_high_2 - ideal_circle_high_1;
        vector_normal_2 = p1 - ideal_circle_high_1;
        norm = cross(vector_normal_1, vector_normal_2);
        gl_Position = Projection * View * Model * vec4(ideal_circle_high_1, 1);
        EmitVertex();
        gl_Position = Projection * View * Model * vec4(ideal_circle_high_1 + 2 * norm, 1);
        EmitVertex();
        EndPrimitive();
        
        vector_normal_1 = p4 - ideal_circle_high_2;
        vector_normal_2 = p2 - ideal_circle_high_2;
        norm = cross(vector_normal_1, vector_normal_2);
        gl_Position = Projection * View * Model * vec4(ideal_circle_high_2, 1);
        EmitVertex();
        gl_Position = Projection * View * Model * vec4(ideal_circle_high_2 + 2 * norm, 1);
        EmitVertex();
        EndPrimitive();
        

        t = t + step;
    }

}
