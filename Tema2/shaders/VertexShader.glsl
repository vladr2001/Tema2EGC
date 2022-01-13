#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 vp;
layout(location = 3) in vec3 vn;
layout(location = 2) in vec2 vt;
layout(location = 1) in vec3 vc;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float dT;

// Output
// TODO(student): Output values to fragment shader
out vec3 fp;
out vec3 fn;
out vec2 ft;
out vec3 fc;

void main()
{
    // TODO(student): Send output to fragment shader
    fp = vp;
    fn = vn + vec3(sin(dT), sin(dT), cos(dT));
    ft = vt;
    fc = vc;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(vp + vec3(sin(dT), sin(dT), cos(dT)), 1.0);
}
