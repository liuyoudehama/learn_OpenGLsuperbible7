#version 450 core

void main() {
    // Vertex positions
    vec3 vertexPositions[3] = vec3[](vec3(-1.0, -1.0, 0.0),  // Vertex 1
                                     vec3(1.0, -1.0, 0.0),   // Vertex 2
                                     vec3(0.0, 1.0, 0.0));   // Vertex 3

    // Set the output position based on the vertex index
    gl_Position = vec4(vertexPositions[gl_VertexID], 1.0);
}
