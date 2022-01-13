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


uniform int shouldExplode;
uniform int random;

void main()
{

    // TODO(student): Compute gl_Position
    vec3 aux = vp;
    if (shouldExplode == 1) {
        aux += vn;
        aux *= random;
    }

    gl_Position = Projection * View * Model * vec4(aux, 1.0);
}
