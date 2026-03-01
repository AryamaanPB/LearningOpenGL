#include "TriangleMesh.h"
#include <vector>
#include <glad/glad.h>

TriangleMesh::TriangleMesh()
{
    std::vector<float> position = {
		-1.0f, -1.0f, 0.0f, //bottom left
         1.0f, -1.0f, 0.0f, //bottom right 
		-1.0f, 1.0f, 0.0f, //top left
		 1.0f, 1.0f, 0.0f, //top right
    };

    std::vector<int> colorIndices = {
        0, 1, 2, 3, 1, 2
    };

	std::vector<int> elementIndices = {
		0, 1, 2, 1, 2, 3
	};

    vertexCount = 6;

    VBOs.resize(2);

	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);


    //position
	glGenBuffers(1, &VBOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float), position.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color
	glGenBuffers(1, &VBOs[1]);
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[1]);
	glBufferData(GL_ARRAY_BUFFER, colorIndices.size() * sizeof(int), colorIndices.data(), GL_STATIC_DRAW);
    glVertexAttribIPointer(1, 1, GL_INT, sizeof(int), (void*)0);
	glEnableVertexAttribArray(1);

	//element
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementIndices.size() * sizeof(int), elementIndices.data(), GL_STATIC_DRAW);
}

void TriangleMesh::draw()
{
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);
}

TriangleMesh::~TriangleMesh()
{
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(VBOs.size(), VBOs.data());
	glDeleteBuffers(1, &EBO);
}
