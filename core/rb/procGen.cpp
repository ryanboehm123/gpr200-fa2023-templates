//procGen.cpp
#include "procGen.h"

namespace rbLib {
	ew::MeshData createSphere(float radius, int numSegments) {
		float thetaStep = 2 * ew::PI / numSegments;
		float phiStep = ew::PI / numSegments;
		std::vertex<ew::Vertex> vertices;
		std::vertex<unsigned int> indices;
		ew::Vec3 v;

		for (int row = 0; row <= numSegments; row++) {
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) {
				float theta = col * thetaStep;
				v.x = radius * sin(phi) * sin(theta);
				v.y = radius * cos(phi);
				v.z = radius * sin(phi) * cos(theta);
				vertices.push_back(v);
			}
		}

		int poleStart = ?;//Index of first pole vertex
		int sideStart = numSegments + 1;
		for (size_t i = 0; i < numSegments; i++) {
			indices.push_back(sideStart + i);
			indices.push_back(poleStart + i); // Pole
			indices.push_back(sideStart + i + 1);
		}

		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int row = 1; row < numSegments - 1; row++) {
			for (int col = 0; col < numSegments; col++) {
				int start = row * columns + col;
				//Triangle 1
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + columns);
				//Triangle 2
				//CONTINUE
			}
		}

	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		/*
		topY = height / 2; //y=0 is centered
		bottomY = -topY;

		//Top center
		vertices.push_back({0,topY,0});

		//Top ring
		Add top ring vertices

		//Bottom ring
		Add bottom ring vertices

		//Bottom center
		vertices.push_back({0,bottomY,0});

		thetaStep = 2 * ew::PI / numSegments
		for(i = 0; i <= numSegments; i++) {
			theta = i * thetaStep;
			v.x = cos(theta) * radius;
			v.z = sin(theta) * radius;
			v.y = topY; // or bottomY
			vertices.push_back(v);
		}

		start = //Index of first ring vector
		center = //Index of center vertex

		for(i = 0; i <= numSegments; i++) {
			indices.push_back(start + i);
			indices.push_back(center);
			indices.push_back(start + i + 1);
		}

		sideStart = //index of first top ring vertex
		columns = numSegments + 1;
		for(i = 0; i < colums; i++) {
			start = sideStart + i;
			//Triangle 1
			indices.push_back(start)
			indices.push_back(start + 1);
			indices.push_back(start + columns);
			//Triangle 2
			...
		}
		*/
	}
	
	ew::MeshData createPlane(float size, int subdivisions) {
		/*
		for (row = 0; row <= subdivisions) {
			for(col = 0; col <= subdivisions) {
				v.x = width * (col/subdivisions);
				v.z = -height * (row/subdivisions);
				verticies.push_back(v);
			}
		}

		columns = subdivisions + 1;
		for(row = 0; row < subdivisions) {
			for(col = 0; col < subdivisions) {
				start = row * columns + col;
				indices.push_back(start);
				indices.push_back(start + 1);
				indices.push_back(start + columns + 1);
				indices.push_back(start + columns);
				indices.push_back(start);
			}
		}

		*/
	}
}