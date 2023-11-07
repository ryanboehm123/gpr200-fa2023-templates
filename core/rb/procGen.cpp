//procGen.cpp
#include "procGen.h"

namespace rbLib {
	ew::MeshData createSphere(float radius, int numSegments) {
		float thetaStep = 2 * ew::PI / numSegments;
		float phiStep = ew::PI / numSegments;
		ew::MeshData data;
		ew::Vertex v;

		//Vertexes
		for (int row = 0; row <= numSegments; row++) {
			//First and last row converge at poles
			float phi = row * phiStep;
			for (int col = 0; col <= numSegments; col++) { //Duplicate column for each row
				float theta = col * thetaStep;
				v.pos.x = radius * cos(theta) * sin(phi);
				v.pos.y = radius * cos(phi);
				v.pos.z = radius * sin(theta) * sin(phi);
				v.normal = ew::Normalize(v.pos - ew::Vec3{0, 1, 0});
				v.uv.x = (float)col / numSegments;
				v.uv.y = (float)row / numSegments;
				data.vertices.push_back(v);
			}
		}

		//Indices
		//Top Face
		int poleStart = 0; //Index of first pole vertex
		int sideStart = numSegments + 1;
		for (size_t i = 0; i < numSegments; i++) {
			data.indices.push_back(sideStart + i);
			data.indices.push_back(poleStart + i); // Pole
			data.indices.push_back(sideStart + i + 1);
		}

		//Side
		int columns = numSegments + 1;
		//Skip top and bottom poles
		for (int row = 1; row < numSegments - 1; row++) {
			for (int col = 0; col < numSegments; col++) {
				int start = row * columns + col;
				//Triangle 1
				data.indices.push_back(start);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns);
				//Triangle 2
				data.indices.push_back(start + columns);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns + 1);
			}
		}

		//Bottom Face
		poleStart = data.vertices.size() - numSegments; //Index of first pole vertex
		sideStart = poleStart - numSegments - 2;
		for (size_t i = 0; i < numSegments; i++) {
			data.indices.push_back(sideStart + i + 1);
			data.indices.push_back(poleStart + i); // Pole
			data.indices.push_back(sideStart + i);
		}

		return data;
	}

	ew::MeshData createCylinder(float height, float radius, int numSegments) {
		ew::MeshData data;
		ew::Vertex v;

		float topY = height / 2; //y=0 is centered
		float bottomY = -topY;
		float thetaStep = 2 * ew::PI / numSegments;

		//Top center
		v.pos = {0,topY,0};
		v.normal = { 0, 1, 0 };
		v.uv = { 0.5, 0.5 };
		data.vertices.push_back(v);

		//Top ring (top face)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal = { 0, 1, 0 };
			v.uv.x = cos(theta) / 2 + 0.5;
			v.uv.y = sin(theta) / 2 + 0.5;
			data.vertices.push_back(v);
		}

		//Top ring (side face)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = topY;
			v.normal.x = cos(theta);
			v.normal.y = 0;
			v.normal.z = sin(theta);
			v.uv.x = cos(theta / 2) / 2 + 0.5;
			v.uv.y = 1;
			data.vertices.push_back(v);
		}

		//Bottom ring (side face)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.normal.x = cos(theta);
			v.normal.y = 0;
			v.normal.z = sin(theta);
			v.uv.x = cos(theta / 2) / 2 + 0.5;
			v.uv.y = 0;
			data.vertices.push_back(v);
		}

		//Bottom ring (bottom face)
		for (int i = 0; i <= numSegments; i++) {
			float theta = i * thetaStep;
			v.pos.x = cos(theta) * radius;
			v.pos.z = sin(theta) * radius;
			v.pos.y = bottomY;
			v.normal = { 0, -1, 0 };
			v.uv.x = cos(theta) / 2 + 0.5;
			v.uv.y = sin(theta) / 2 + 0.5;
			data.vertices.push_back(v);
		}

		//Bottom center
		v.pos = { 0,bottomY,0 };
		v.normal = { 0, -1, 0 };
		v.uv = { 0.5, 0.5 };
		data.vertices.push_back(v);

		//Indices
		int start = 1;
		float center = 0;

		//Top face
		for(int i = 0; i < numSegments; i++) {
			data.indices.push_back(start + i);
			data.indices.push_back(center);
			data.indices.push_back(start + i + 1);
		}

		//Side
		int sideStart = numSegments + 1;
		int columns = numSegments + 2;

		for(int i = 0; i < columns; i++) {
			start = sideStart + i;
			//Triangle 1
			data.indices.push_back(start);
			data.indices.push_back(start + 1);
			data.indices.push_back(start + columns);
			//Triangle 2
			data.indices.push_back(start + 1);
			data.indices.push_back(start + columns + 1);
			data.indices.push_back(start + columns);
		}

		//Bottom face
		start = data.vertices.size() / 2 + data.vertices.size() / 4;
		center = data.vertices.size() - 1;

		for (int i = 0; i <= numSegments; i++) {
			data.indices.push_back(start + i + 1);
			data.indices.push_back(center);
			data.indices.push_back(start + i);
		}

		return data;
	}
	
	ew::MeshData createPlane(float width, float height, int subdivisions) {
		ew::MeshData data;
		ew::Vertex v;
		v.normal = { 0,1,0 };

		for (float row = 0; row <= subdivisions; row++) {
			for (float col = 0; col <= subdivisions; col++) {
				v.pos.x = width * (col/subdivisions);
				v.pos.z = -height * (row/subdivisions);
				v.uv.x = col / width;
				v.uv.y = row / height;
				data.vertices.push_back(v);
			}
		}

		int columns = subdivisions + 1;
		for (int row = 0; row < subdivisions; row++) {
			for (int col = 0; col < subdivisions; col++) {
				int start = row * columns + col;
				//Triangle 1
				data.indices.push_back(start);
				data.indices.push_back(start + 1);
				data.indices.push_back(start + columns + 1);
				//Triangle 2
				data.indices.push_back(start + columns);
				data.indices.push_back(start);
				data.indices.push_back(start + columns + 1);
			}
		}

		return data;
	}
}