/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Phong shader - Straight forward per-pixel 
shading. Also changes with the time of day
and environment.  
*/
#version 400
layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
out vec3 color;
out vec3 Position;
out vec3 Normal;
uniform mat4 modelview_matrix;
uniform mat3 normal_matrix;
uniform mat4 projection_matrix;
uniform mat4 mvp;
uniform float red;
uniform float green;
uniform float blue;
void main()
{
color = vec3(red, green, blue);
Normal = normalize( normal_matrix * VertexNormal);
Position = vec3( modelview_matrix * vec4(VertexPosition,1.0) );
gl_Position = mvp * vec4(VertexPosition,1.0);
}