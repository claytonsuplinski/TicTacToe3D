/*
Name: Clayton Suplinski
ID: 906 580 2630
CS 559 Project 3

Phong shader - Straight forward per-pixel 
shading. Also changes with the time of day
and environment.  
*/
#version 400
in vec3 color;
in vec3 Position;
in vec3 Normal;
const vec4 LightPosition = vec4(0.0, 0.0, 0.0, 1.0);
const vec3 LightIntensity = vec3(0.85, 0.85, 0.85);
const vec3 Kd = vec3(0.1, 0.1, 0.1); // Diffuse reflectivity
const vec3 Ka = vec3(0.85, 0.85, 0.85); // Ambient reflectivity
const vec3 Ks = vec3(0.75, 0.75, 0.75); // Specular reflectivity
const float Shininess = 6.0f; // Specular shininess factor
layout( location = 0 ) out vec4 FragColor;

uniform ivec2 size;
vec3 color2 = color;

vec3 setting = vec3(0.0, 0.0, 0.0);
float factor = 1.0f;

vec3 ads( )
{
vec3 n = normalize( Normal );
vec3 s = normalize( vec3(LightPosition) - Position );
vec3 v = normalize(vec3(-Position));
vec3 r = reflect( -s, n );

//Normal
return setting +
LightIntensity * factor *
( Ka * vec3(color) +
Kd * vec3(color) * max( dot(s, n), 0.0 ) +
Ks * vec3(color) * pow( max( dot(r,v), 0.0 ), Shininess ) );
}
void main() {
FragColor = vec4(ads(), 1.0);
}