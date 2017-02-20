#include "MyPrimitive.h"
MyPrimitive::MyPrimitive() { }
MyPrimitive::MyPrimitive(const MyPrimitive& other) { }
MyPrimitive& MyPrimitive::operator=(const MyPrimitive& other) { return *this; }
MyPrimitive::~MyPrimitive(void) { super::Release(); }
void MyPrimitive::CompileObject(vector3 a_v3Color)
{
	m_uVertexCount = static_cast<int> (m_lVertexPos.size());
	for (uint i = 0; i < m_uVertexCount; i++)
	{
		AddVertexColor(a_v3Color);
	}
	
	CompleteTriangleInfo(true);
	CompileOpenGL3X();

}
//C--D
//|\ |
//| \|
//A--B
//This will make the triang A->B->C and then the triang C->B->D
void MyPrimitive::AddQuad(vector3 a_vBottomLeft, vector3 a_vBottomRight, vector3 a_vTopLeft, vector3 a_vTopRight)
{
	AddVertexPosition(a_vBottomLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopLeft);

	AddVertexPosition(a_vTopLeft);
	AddVertexPosition(a_vBottomRight);
	AddVertexPosition(a_vTopRight);
}
void MyPrimitive::GeneratePlane(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;

	vector3 pointA(-fValue, -fValue, 0.0f); //0
	vector3 pointB(fValue, -fValue, 0.0f); //1
	vector3 pointC(fValue, fValue, 0.0f); //2
	vector3 pointD(-fValue, fValue, 0.0f); //3

	vector3 pointE(fValue, -fValue, -0.001f); //1
	vector3 pointF(-fValue, -fValue, -0.001f); //0
	vector3 pointG(fValue, fValue, -0.001f); //2
	vector3 pointH(-fValue, fValue, -0.001f); //3

											  //F
	AddQuad(pointA, pointB, pointD, pointC);
	//Double sided
	AddQuad(pointE, pointF, pointG, pointH);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCube(float a_fSize, vector3 a_v3Color)
{
	if (a_fSize < 0.01f)
		a_fSize = 0.01f;

	Release();
	Init();

	float fValue = 0.5f * a_fSize;
	//3--2
	//|  |
	//0--1
	vector3 point0(-fValue, -fValue, fValue); //0
	vector3 point1(fValue, -fValue, fValue); //1
	vector3 point2(fValue, fValue, fValue); //2
	vector3 point3(-fValue, fValue, fValue); //3

	vector3 point4(-fValue, -fValue, -fValue); //4
	vector3 point5(fValue, -fValue, -fValue); //5
	vector3 point6(fValue, fValue, -fValue); //6
	vector3 point7(-fValue, fValue, -fValue); //7

											  //F
	AddQuad(point0, point1, point3, point2);

	//B
	AddQuad(point5, point4, point6, point7);

	//L
	AddQuad(point4, point0, point7, point3);

	//R
	AddQuad(point1, point5, point2, point6);

	//U
	AddQuad(point3, point2, point7, point6);

	//D
	AddQuad(point4, point5, point0, point1);

	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCone(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;
	
	std::vector<vector3> points;
	float theta = 0; 
	vector3 basecenter(0.0f, -a_fHeight, 0.0f);
	vector3 topcenter(0.0f, a_fHeight, 0.0f);

	//theta travels around and creates the base
	float travel = 2 * PI / static_cast<float>(a_nSubdivisions);


	//make bases of the cone ---------------------------------------------
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		points.push_back(vector3(cos(theta), -a_fHeight, sin(theta)));
		theta += travel;
	}
	for (int i = 0; i < a_nSubdivisions-1; i++)
	{
		//drawing the triangles using addtri
		AddTri(points[i+1], points[i],basecenter); 
		AddTri(points[i], points[i + 1], topcenter);
	}

	//connecting the last parts
	AddTri(points[0], points[a_nSubdivisions - 1], basecenter);
	AddTri(points[0], points[a_nSubdivisions - 1], topcenter);

	//Done creating the shape, so pop because we are done with the shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		points.pop_back(); 
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateCylinder(float a_fRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();
	//Your code starts here
	float fValue = 0.5f;

	std::vector<vector3> points;
	std::vector<vector3> points2;
	float theta = 0;
	vector3 basecenter(0.0f, -a_fHeight, 0.0f);
	vector3 topcenter(0.0f, a_fHeight, 0.0f);

	//theta travels around and creates the base
	float travel = 2 * PI / static_cast<float>(a_nSubdivisions);


	//make bases of the cylindar---------------------------------------------
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		points.push_back(vector3(cos(theta), -a_fHeight, sin(theta)));
		points2.push_back(vector3(cos(theta), a_fHeight, sin(theta)));
		theta += travel;
	}

	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		
		AddTri(points[i+1], points[i], basecenter);
		AddTri(points2[i], points2[i+1], topcenter);
		AddQuad(points[i+1], points[i], points2[i+1], points2[i]);
	}

	//connecting the last parts
	AddTri(points[0], points[a_nSubdivisions - 1], basecenter);
	AddTri(points2[a_nSubdivisions - 1],points2[0], topcenter);
	AddQuad(points[0], points[a_nSubdivisions - 1], points2[0], points2[a_nSubdivisions - 1] );

	//Done creating the shape, so pop because we are done with the shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		points.pop_back();
		points2.pop_back();
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateTube(float a_fOuterRadius, float a_fInnerRadius, float a_fHeight, int a_nSubdivisions, vector3 a_v3Color)
{
	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	//Your code starts here
	float fValue = 0.5f;

	std::vector<vector3> topouter;
	std::vector<vector3> topinner;
	std::vector<vector3> bottomouter;
	std::vector<vector3> bottominner;
	float theta = 0;

	//theta travels around and creates the base
	float travel = 2 * PI / static_cast<float>(a_nSubdivisions);


	//make bases--------------------------------------------
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		//making points 
		topouter.push_back(vector3(cos(theta)*a_fOuterRadius, a_fHeight, sin(theta)*a_fOuterRadius));
		topinner.push_back(vector3(cos(theta)*a_fInnerRadius, a_fHeight, sin(theta)*a_fInnerRadius));
		bottominner.push_back(vector3(cos(theta)*a_fInnerRadius, -a_fHeight, sin(theta)*a_fInnerRadius));
		bottomouter.push_back(vector3(cos(theta)*a_fOuterRadius, -a_fHeight, sin(theta)*a_fOuterRadius));
		theta += travel;
	}

	//drawing the quads
	for (int i = 0; i < a_nSubdivisions - 1; i++)
	{
		AddQuad(bottomouter[i + 1], bottomouter[i], topouter[i + 1], topouter[i]);
		AddQuad(bottominner[i], bottominner[i+1], topinner[i], topinner[i+1]);
		AddQuad(topouter[i+1], topouter[i], topinner[i+1], topinner[i]);
		AddQuad(bottomouter[i], bottomouter[i+1], bottominner[i], bottominner[i+1]);
	}

	//connecting the last parts
	AddQuad(bottomouter[0], bottomouter[a_nSubdivisions-1], topouter[0], topouter[a_nSubdivisions-1]);
	AddQuad(bottominner[a_nSubdivisions-1], bottominner[0], topinner[a_nSubdivisions-1], topinner[0]);
	AddQuad(topouter[0], topouter[a_nSubdivisions-1], topinner[0], topinner[a_nSubdivisions-1]);
	AddQuad(bottomouter[a_nSubdivisions - 1],bottomouter[0], bottominner[a_nSubdivisions - 1],bottominner[0]);

	//Done creating the shape, so pop because we are done with the shape
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		bottominner.pop_back();
		bottomouter.pop_back();
		topinner.pop_back();
		topouter.pop_back();
	}

	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::GenerateSphere(float a_fRadius, int a_nSubdivisions, vector3 a_v3Color)
{

	if (a_nSubdivisions < 3)
		a_nSubdivisions = 3;
	if (a_nSubdivisions > 360)
		a_nSubdivisions = 360;

	Release();
	Init();

	float leftangle;
	float rightangle;
	float thetaright;
	float thetaleft;
	for (int i = 0; i < a_nSubdivisions; i++)
	{
		leftangle = (PI * i) / a_nSubdivisions;
		rightangle = (PI * (i + 1)) / a_nSubdivisions;

		for (int j = 0; j < a_nSubdivisions; j++)
		{
			thetaleft = (2 * PI * j) / a_nSubdivisions;
			thetaright = (2 * PI * (j + 1)) / a_nSubdivisions;


			vector3 point0(a_fRadius * cos(thetaleft)* sin(leftangle), a_fRadius* cos(leftangle), a_fRadius*sin(thetaleft)*sin(leftangle));
			vector3 point1(a_fRadius * cos(thetaleft)* sin(rightangle), a_fRadius* cos(rightangle), a_fRadius*sin(thetaleft)*sin(rightangle));
			vector3 point2(a_fRadius * cos(thetaright)* sin(leftangle), a_fRadius* cos(leftangle), a_fRadius*sin(thetaright)*sin(leftangle));
			vector3 point3(a_fRadius * cos(thetaright)* sin(rightangle), a_fRadius* cos(rightangle), a_fRadius*sin(thetaright)*sin(rightangle));

			AddQuad(point0, point1, point2, point3);
		}
	}
	
	//Your code ends here
	CompileObject(a_v3Color);
}
void MyPrimitive::AddTri(vector3 bottomleft, vector3 bottomright, vector3 topleft)
{
	AddVertexPosition(topleft);
	AddVertexPosition(bottomright);
	AddVertexPosition(bottomleft);
}