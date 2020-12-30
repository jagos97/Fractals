#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Geometry.h"
#include "GLDebug.h"
#include "Log.h"
#include "ShaderProgram.h"
#include "Shader.h"
#include "Window.h"


//Gives the middle point between 2 vectors (add them up before using)
glm::vec3 HalfVector(glm::vec3 vec) {
	glm::vec3 newVec(vec.x / 2, vec.y / 2, vec.z);
	return newVec;
}

glm::vec3 red(1.0f, 0.f, 0.f);
glm::vec3 green(.0f, 1.f, 0.f);
glm::vec3 blue(.0f, 0.f, 1.f);
glm::vec3 yellow(1.0f, 1.f, 0.f);
glm::vec3 magenta(1.0f, 0.f, 1.f);
glm::vec3 cyan(.0f, 1.f, 1.f);
glm::vec3 white(1.0f, 1.f, 1.f);

glm::vec3 colours[7] = { red,green,blue,yellow,magenta,cyan,white };

float pi = 3.1415926535;


/// <summary>
/// Class used for the Sierpinski Triangle
/// </summary>
class Triangle {
public:
	Triangle(glm::vec3 top, glm::vec3 left, glm::vec3 right) {
		first = top;
		second = left;
		third = right;
	}

	
	glm::vec3 first;												//top
	glm::vec3 second;												//bottom left
	glm::vec3 third;												//bottom right

	/*Inserts 3 triangles into the cpuGeom given based on a father triangle
	Also decides colours based on colour of parent*/
	void Draw3Triangles(CPU_Geometry& cpu, float r, float g, float blue) {

		//Getting vectors for the 6 points needed
		glm::vec3 a, ab, ac, b, bc, c;
		a = first;													//top of top triangle
		ab = HalfVector(first + second);							//bottom left of top triangle
		ac = HalfVector(first + third);								//bottom right of top triangle
		b = second;													//bottom left of left triangle
		bc = HalfVector(second + third);							//bottom right of left triangle
		c = third;													//bottom right of right triangle


		//red dominant
		if (r > g && r > blue) {
			cpu.cols.push_back(glm::vec3(r * .9f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .9f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .9f, 0, 0));

			cpu.cols.push_back(glm::vec3(r * .6f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .6f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .6f, 0, 0));

			cpu.cols.push_back(glm::vec3(r * .4f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .4f, 0, 0));
			cpu.cols.push_back(glm::vec3(r * .4f, 0, 0));
		}
		//green dominant
		else if (g > blue) {
			cpu.cols.push_back(glm::vec3(0, g * .9f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .9f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .9f, 0));

			cpu.cols.push_back(glm::vec3(0, g * .6f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .6f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .6f, 0));

			cpu.cols.push_back(glm::vec3(0, g * .4f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .4f, 0));
			cpu.cols.push_back(glm::vec3(0, g * .4f, 0));
		}

		//blue dominant
		else if (blue > g) {
			cpu.cols.push_back(glm::vec3(0, 0, blue * .9f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .9f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .9f));

			cpu.cols.push_back(glm::vec3(0, 0, blue * .6f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .6f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .6f));

			cpu.cols.push_back(glm::vec3(0, 0, blue * .4f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .4f));
			cpu.cols.push_back(glm::vec3(0, 0, blue * .4f));
		}

		else {
			cpu.cols.push_back(glm::vec3(1.f, 0.f, 0.f));
			cpu.cols.push_back(glm::vec3(1.f, 0.f, 0.f));
			cpu.cols.push_back(glm::vec3(1.f, 0.f, 0.f));

			cpu.cols.push_back(glm::vec3(0.f, 1.f, 0.f));
			cpu.cols.push_back(glm::vec3(0.f, 1.f, 0.f));
			cpu.cols.push_back(glm::vec3(0.f, 1.f, 0.f));

			cpu.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
			cpu.cols.push_back(glm::vec3(0.f, 0.f, 1.f));
			cpu.cols.push_back(glm::vec3(0.f, 0.f, 1.f));

		}

		cpu.verts.push_back(a);
		cpu.verts.push_back(ab);
		cpu.verts.push_back(ac);

		cpu.verts.push_back(ab);
		cpu.verts.push_back(b);
		cpu.verts.push_back(bc);


		cpu.verts.push_back(ac);
		cpu.verts.push_back(bc);
		cpu.verts.push_back(c);


	}
};


/// <summary>
/// Class used for the Squares and Diamond
/// </summary>
class Quad {
public:
	//Based on diamond formation
	/*If square then
	top = top left
	left = bottom left
	right = bottom right
	bottom = top right*/
	Quad(glm::vec3 top, glm::vec3 left, glm::vec3 right, glm::vec3 bottom, bool red) {
		first = top;
		second = left;
		third = right;
		fourth = bottom;
		isred = red;					//also could be used as isSquare
	}
	bool isred;
	glm::vec3 first;												//top
	glm::vec3 second;												//bottom left
	glm::vec3 third;												//bottom right
	glm::vec3 fourth;												//bottom right


	//Given a quad it will give the points for the opposite quad that lies inside it (square will give diamond, diamond will give square)
	Quad DrawQuadBasedOnQuad() {
		glm::vec3 a, b, c, d;

		a = HalfVector(first + second);
		b = HalfVector(second + third);
		c = HalfVector(third + fourth);
		d = HalfVector(fourth + first);
		if (isred) {
			Quad newDiamond(a, b, c, d, !isred);
			return newDiamond;
		}
		else {
			Quad newSquare(d, a, b, c, !isred);
			return newSquare;
		}
	}


	/// <summary>
	/// Inserts the quad into the cpu
	/// </summary>
	/// <param name="cpu">Cpu to add the vertices in </param>
	/// <param name="shade">shade of the quad to be drawn </param>
	void PutInCPU(CPU_Geometry& cpu, float shade) {
		cpu.verts.push_back(first);
		cpu.verts.push_back(second);
		cpu.verts.push_back(second);
		cpu.verts.push_back(third);
		cpu.verts.push_back(third);
		cpu.verts.push_back(fourth);
		cpu.verts.push_back(fourth);
		cpu.verts.push_back(first);

		if (isred) {
			for (int i = 0; i < 8; i++) {
				cpu.cols.push_back(glm::vec3(1.f * shade, 0.f, 0.f));
			}
		}
		else {
			for (int i = 0; i < 8; i++) {
				cpu.cols.push_back(glm::vec3(0.f, 1.f * shade, 0.f));
			}
		}
	}
};

/// <summary>
/// clears the list of cpus so another scene can be stored in them
/// </summary>
/// <param name="cpus"> List containig cpus </param>
void ClearCPU(CPU_Geometry cpus[7]) {
	for (int i = 0; i < 7; i++) {
		cpus[i].cols.clear();
		cpus[i].verts.clear();
	}
}




// EXAMPLE CALLBACKS
class MyCallbacks : public CallbackInterface {

public:

	int level = 0;
	int scene = 0;
	MyCallbacks(ShaderProgram& shader) : shader(shader) {}

	virtual void keyCallback(int key, int scancode, int action, int mods) {
		if (action == GLFW_PRESS) {
			if (key == GLFW_KEY_R) {
				shader.recompile();
			}
			else if (key == GLFW_KEY_0) {
				level = 0;
			}
			else if (key == GLFW_KEY_1) {
				level = 1;
			}
			else if (key == GLFW_KEY_2) {
				level = 2;
			}
			else if (key == GLFW_KEY_3) {
				level = 3;
			}
			else if (key == GLFW_KEY_4) {
				level = 4;
			}
			else if (key == GLFW_KEY_5) {
				level = 5;
			}
			else if (key == GLFW_KEY_6) {
				level = 6;
			}
			else if (key == GLFW_KEY_RIGHT) {
				scene ++;
				if (scene > 4) scene = 4;
			}
			else if (key == GLFW_KEY_LEFT) {
				scene --;
				if (scene < 0) scene = 0;
			}
		}
	}
	int GetLevel() {
		return level;
	}
	int GetScene() {
		return scene;
	}

private:
	ShaderProgram& shader;

};


//finds the a third of the way between 2 points
glm::vec3 FindThirdOfTheWay(glm::vec3 a, glm::vec3 b) {
	glm::vec3 c = a * (2.0f / 3.0f) + b * (1.0f / 3.0f);
	return c;
}
//vector of the original line
glm::vec3 FindDirectionVector(glm::vec3 a, glm::vec3 b) {
	float x = b.x - a.x;
	float y = b.y - a.y;
	return glm::vec3(x, y, 0.f);
}
//Finds perpendicular vector of original line
glm::vec3 FindPerpVector(glm::vec3 a) {
	if(a.y != 0)return glm::vec3(1.0f, -a.x/a.y, .0f);
	else return glm::vec3(0.f, 1.f, .0f);
}
//FINDS height
float FindHeight(glm::vec3 a, glm::vec3 b) {
	float x = b.x - a.x;
	float y = b.y - a.y;
	float length = sqrt(pow(x, 2) + pow(y, 2));
	return  length * (sqrt(3)/2);
}
/// <summary>
/// Based on the params it finds the point of the triangle NO LONGER USED
/// </summary>
/// <param name="height">height of triangle (from midpoint original line to the tip of triangle) </param>
/// <param name="perp">perpindicular vector of the line</param>
/// <param name="mid"> mid point of original line</param>
/// <param name="a">begining of line</param>
/// <param name="b"> end of line</param>
/// <returns></returns>
glm::vec3 FindFinalVector(float height, glm::vec3 perp, glm::vec3 mid, glm::vec3 a, glm::vec3 b) {
	if (perp.x == 0)return glm::vec3(0.f, -height, 0.0f) + mid;
	float oneside = pow(perp.y,2) + 1.f;
	float x = sqrt(pow(height, 2) / oneside);
	float y = perp.y * x;
	if (a.y > b.y) return glm::vec3(x, y, 0.0f) + mid;
	return glm::vec3(-x, -y, 0.0f) + mid;
	
	
}

glm::vec3 FindLastVector(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	glm::vec3 mid, direction, perp, answer;
	float x = c.x - a.x;
	float y = c.y - a.y;
	float length = sqrt(pow(x, 2) + pow(y, 2));
	direction = FindDirectionVector(a,b);
	float initialAngle;
	initialAngle = atan(direction.y / direction.x);
	if (direction.x < 0 && direction.y > 0) initialAngle -= pi ;
	else if (direction.x < 0 && direction.y < 0) initialAngle -= pi  ;
	else if (direction.y == 0) initialAngle = pi;
	answer = glm::vec3(c.x + length * cos(initialAngle + pi/3.f), c.y + length * sin(initialAngle + pi/3.f), 0);

	/* No longer used
	mid = HalfVector(a + b);
	//std::cout << "half vector found between " << a << " and " << b << "was " << mid << std::endl;
	direction = FindDirectionVector(b, a);
	//std::cout << "direction found between " << a << " and " << b << "was " << direction << std::endl;
	perp = FindPerpVector(direction);
	//std::cout << "perp vector found with direction " << direction <<  "was " << perp << std::endl;
	float height = FindHeight(a, c);
	//std::cout << "height found between " << a << " and " << c << "was " << height << std::endl;
	//answer = FindFinalVector(height, perp,mid, a,b);
	//std::cout << "answer was " << answer << std::endl;
	*/
	return answer;
}


/// <summary>
/// Takes the cpu from the previous level and for every triangle in it, it generates 3 inside it
/// </summary>
/// <param name="verts">previous level's triangles</param>
/// <param name="next"> This level's triangle</param>
void GenerateTriangles(CPU_Geometry verts ,CPU_Geometry &next) {
		for (int i = 0; i + 2 < verts.verts.size(); i = i + 3) {
			Triangle tri = Triangle(verts.verts.at(i), verts.verts.at(i + 1), verts.verts.at(i + 2));
			tri.Draw3Triangles(next, verts.cols.at(i).x, verts.cols.at(i).y, verts.cols.at(i).z);
		}
}

//generates a quad based on another quad
Quad GenerateQuads(Quad quad) {
	Quad newQuad(quad.DrawQuadBasedOnQuad());
	return newQuad;
}

/// <summary>
/// takes 1 line and generates the triangle in the middle
/// Yes i am aware its actually 4 lines my mistake
/// </summary>
/// <param name="old">cpu of the level before(used for connecting the end of the line to the next line)</param>
/// <param name="next">cpu of this level</param>
/// <param name="colour">colour if this level</param>
void Generate3LinesFrom1(CPU_Geometry old , CPU_Geometry &next, glm::vec3 colour) {
	glm::vec3 a, b, c, d, e;
	next.verts.push_back(old.verts.at(0));
	next.cols.push_back(old.cols.at(0));
	for (int i = 0; i + 1 < old.verts.size(); i = i+2) {
		a = old.verts.at(i);
		b = old.verts.at(i + 1);
		c = FindThirdOfTheWay(a, b);
		d = FindThirdOfTheWay(b, a);
		e = FindLastVector(a, b, c);
		next.verts.push_back(c);
		next.verts.push_back(c);
		next.verts.push_back(e);
		next.verts.push_back(e);
		next.verts.push_back(d);
		next.verts.push_back(d);
		next.verts.push_back(b);
		next.verts.push_back(b);
		next.cols.push_back(old.cols.at(i));
		next.cols.push_back(colour);
		next.cols.push_back(colour);
		next.cols.push_back(colour);
		next.cols.push_back(colour);
		next.cols.push_back(old.cols.at(i));
		next.cols.push_back(old.cols.at(i));
		if(i+2 < old.verts.size()) next.cols.push_back(old.cols.at(i + 2));
		else next.cols.push_back(old.cols.at(i + 1));
	}
	//last line of the old cpu that couldnt be done in loop since i got out of bounds (maybe unnecessary now that i used lines instead of line loop but it works so it stays there)
	a = old.verts.at(old.verts.size()- 1);
	b = old.verts.at(0);
	c = FindThirdOfTheWay(a, b);
	d = FindThirdOfTheWay(b, a);
	e = FindLastVector(a, b, c);
	next.verts.push_back(c);
	next.verts.push_back(c);
	next.verts.push_back(e);
	next.verts.push_back(e);
	next.verts.push_back(d);
	next.verts.push_back(d);
	next.verts.push_back(b);
	next.cols.push_back(old.cols.at(old.verts.size()-3));
	next.cols.push_back(colour);
	next.cols.push_back(colour);
	next.cols.push_back(colour);
	next.cols.push_back(colour);
	next.cols.push_back(old.cols.at(old.verts.size() - 2));
	next.cols.push_back(old.cols.at(old.verts.size() - 1));

}


/// <summary>
/// Generates scene 0 the sierpinsky
/// </summary>
/// <param name="cpus">cpus that hold the different levels</param>
void GenerateScene0(CPU_Geometry cpus[7]) {
	ClearCPU(cpus);
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////			BASE CASE		///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////

	cpus[0].verts.push_back(glm::vec3(0.f, 1.0f, 0.f));
	cpus[0].verts.push_back(glm::vec3(-1.f, -1.0f, 0.f));
	cpus[0].verts.push_back(glm::vec3(1.f, -1.0f, 0.f));
	cpus[0].cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpus[0].cols.push_back(glm::vec3(1.f, 1.f, 1.f));
	cpus[0].cols.push_back(glm::vec3(1.f, 1.f, 1.f));

	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////	       END	BASE CASE	///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////

	for (int i = 1; i < 7; i++) {
		GenerateTriangles(cpus[i - 1],cpus[i]);
	}
}

/// <summary>
/// Generates scene 1, Squares and Diamonds
/// </summary>
/// <param name="cpus">cpus that hold the levels</param>
void GenerateScene1(CPU_Geometry cpus[7]) {

	ClearCPU(cpus);
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////			BASE CASE		///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	Quad square(glm::vec3(-0.8f, 0.8f, 0), glm::vec3(-0.8f, -0.8f, 0), glm::vec3(0.8f, -0.8f, 0), glm::vec3(0.8f, 0.8f, 0), true);
	cpus[0].verts.push_back(square.first);
	cpus[0].verts.push_back(square.second);
	cpus[0].verts.push_back(square.second);
	cpus[0].verts.push_back(square.third);
	cpus[0].verts.push_back(square.third);
	cpus[0].verts.push_back(square.fourth);
	cpus[0].verts.push_back(square.fourth);
	cpus[0].verts.push_back(square.first);
	for(int i = 0; i < 8; i++) cpus[0].cols.push_back(glm::vec3(1.f, 0.0f, 0.f));

	Quad diamond = square.DrawQuadBasedOnQuad();
	cpus[0].verts.push_back(diamond.first);
	cpus[0].verts.push_back(diamond.second);
	cpus[0].verts.push_back(diamond.second);
	cpus[0].verts.push_back(diamond.third);
	cpus[0].verts.push_back(diamond.third);
	cpus[0].verts.push_back(diamond.fourth);
	cpus[0].verts.push_back(diamond.fourth);
	cpus[0].verts.push_back(diamond.first);
	for (int i = 0; i < 8; i++) cpus[0].cols.push_back(glm::vec3(0.f, 1.0f, 0.f));

	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////	       END	BASE CASE	///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	float shade = 1.f;
	for (int i = 1; i < 7; i++) {
		//verts and cols of previous level
		for (int j = 0; j < cpus[i - 1].verts.size(); j++) {
			cpus[i].verts.push_back(cpus[i - 1].verts.at(j));
			cpus[i].cols.push_back(cpus[i - 1].cols.at(j));
		}
		shade = shade / 2.2f;
		square = diamond.DrawQuadBasedOnQuad();
		square.PutInCPU(cpus[i], shade);
		diamond = square.DrawQuadBasedOnQuad();
		diamond.PutInCPU(cpus[i], shade);
	}
	for (int i = 0; i < 7; i++) {
		std::cout << "cpu size after Generate " << cpus[i].verts.size() << std::endl;
	}
}

/// <summary>
/// Generates scene 2, Koch Snowflake
/// </summary>
/// <param name="cpus"> cpus for the different levels</param>
void GenerateScene2(CPU_Geometry cpus[7]) {
	ClearCPU(cpus);

	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////			BASE CASE		///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	cpus[0].verts.push_back(glm::vec3(0.f, .85f, 0.f));
	cpus[0].verts.push_back(glm::vec3(.7f, -.55f, 0.f));
	cpus[0].verts.push_back(glm::vec3(.7f, -.55f, 0.f));
	cpus[0].verts.push_back(glm::vec3(-.7f, -.55f, 0.f));
	cpus[0].verts.push_back(glm::vec3(-.7f, -.55f, 0.f));
	cpus[0].verts.push_back(glm::vec3(0.f, .85f, 0.f));


	for (int i = 0; i < 6; i++) cpus[0].cols.push_back(red);
	///////////////////////////////////////////////
	///////////////////////////////////////////////
	//////////	       END	BASE CASE	///////////
	///////////////////////////////////////////////
	///////////////////////////////////////////////

	for (int i = 1; i < 7; i++) {
		Generate3LinesFrom1(cpus[i - 1],cpus[i],colours[i]);
	}

}


/// <summary>
/// The following function was based on the following function given by the wikipedia page for Dragon Curve for iterated function system using pairs of real numbers
/// https://en.wikipedia.org/wiki/Dragon_curve#Heighway_dragon
/// If you need to reach out to me to show that I understand the code, I will be available :)
/// </summary>
void DragonCurve(CPU_Geometry &cpu, float x, float y, float length, float angle, int depth)
{
	if (depth > 0)
	{
		DragonCurve(cpu, x, y, length / sqrt(2.0f), angle - pi / 4.0, depth - 1);
		DragonCurve(cpu, x + length * cos(angle), y + length * sin(angle), length / sqrt(2.0f), angle -  3.0f * pi / 4.0, depth - 1);
	}
	else {
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		cpu.verts.push_back(glm::vec3(x + length * cos(angle), y + length * sin(angle), 0));
	}
}

/// <summary>
/// generates scene 3, Dragon Curve
/// </summary>
/// <param name="cpus">cpus that hold the different levels</param>
void GenerateScene3(CPU_Geometry cpus[7]) {
	ClearCPU(cpus);
	DragonCurve(cpus[0], -0.6f, 0.3f, 1.f, 0,1);
	DragonCurve(cpus[1], -0.6f, 0.3f, 1.f, 0, 2);
	DragonCurve(cpus[2], -0.6f, 0.3f, 1.f, 0, 3);
	DragonCurve(cpus[3], -0.6f, 0.3f, 1.f, 0, 4);
	DragonCurve(cpus[4], -0.6f, 0.3f, 1.f, 0, 5);
	DragonCurve(cpus[5], -0.6f, 0.3f, 1.f, 0, 6);
	DragonCurve(cpus[6], -0.6f, 0.3f, 1.f, 0, 7);

	for (int i = 0; i < cpus[0].verts.size(); i++) cpus[0].cols.push_back(red);
	for (int i = 0; i < cpus[1].verts.size(); i++) cpus[1].cols.push_back(blue);
	for (int i = 0; i < cpus[2].verts.size(); i++) cpus[2].cols.push_back(green);
	for (int i = 0; i < cpus[3].verts.size(); i++) cpus[3].cols.push_back(yellow);
	for (int i = 0; i < cpus[4].verts.size(); i++) cpus[4].cols.push_back(magenta);
	for (int i = 0; i < cpus[5].verts.size(); i++) cpus[5].cols.push_back(cyan);
	for (int i = 0; i < cpus[6].verts.size(); i++) cpus[6].cols.push_back(white);
}

/*
*
*
*
* This was a colossal failure
void HilbertCurve(CPU_Geometry& cpu, float x, float y, float length, float angle, int depth) {

	if (depth > 0) {
		HilbertCurve(cpu, x, y, length / 2.0f, angle, depth - 1);
		NegHilbertCurve(cpu, x, y - length, length / 2.0f, angle - pi / 2.0f, depth - 1);
		NegHilbertCurve(cpu, x + length, y - length, length / 2.0f, angle - pi / 2.0f, depth - 1);
		HilbertCurve(cpu, x + 3 * length / 2.0f, y- length / 2.0f, length / 2.0f, angle - pi, depth - 1);

	}
	else {
			cpu.verts.push_back(glm::vec3(x, y, 0.0f));
			x = x + length * cos(angle);
			y = y + length * sin(angle);
			cpu.verts.push_back(glm::vec3(x, y, 0.0f));
			angle -= pi / 2.0f;
			x = x + length * cos(angle);
			y = y + length * sin(angle);
			cpu.verts.push_back(glm::vec3(x, y, 0.0f));
			angle -= pi / 2.0f;
			x = x + length * cos(angle);
			y = y + length * sin(angle);
			cpu.verts.push_back(glm::vec3(x, y, 0.0f));
	}
}

void NegHilbertCurve(CPU_Geometry& cpu, float x, float y, float length, float angle, int depth) {

	if (depth > 0) {
		HilbertCurve(cpu, x, y, length / 2.0f, angle + pi / 2.0f, depth - 1);
		NegHilbertCurve(cpu, x, y - length, length / 2.0f, angle , depth - 1);
		NegHilbertCurve(cpu, x + length, y - length, length / 2.0f, angle, depth - 1);
		HilbertCurve(cpu, x + 3 * length / 2.0f, y - length / 2.0f, length / 2.0f, angle - pi / 2.0f, depth - 1);

	}
	else {

		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		angle += pi / 2.0f;
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		angle += pi / 2.0f;
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
	}

}
*/
/*
void HilbertCurve(CPU_Geometry& cpu, float x, float y, float length, float angle, int depth, int direction) {

	if (depth > 0) {
		HilbertCurve(cpu, x, y, length / 2.0f, angle + pi / 2, depth - 1, direction);
		HilbertCurve(cpu, x, y - length, length / 2.0f, angle, depth - 1, -direction);
		HilbertCurve(cpu, x + length, y - length, length / 2.0f, angle, depth - 1, -direction);
		HilbertCurve(cpu, x + 3 * length / 2.0f, y - length / 2.0f, length / 2.0f, angle - pi/2, depth - 1, direction);

	}
	else {
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		angle -= direction * (pi / 2.0f);
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		angle -= direction * (pi / 2.0f);
		x = x + length * cos(angle);
		y = y + length * sin(angle);
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
		cpu.verts.push_back(glm::vec3(x, y, 0.0f));
	}
}
*/

/// <summary>
/// Used the following link's lindenmayer system rewriting rules for the implementation of the the Hilbert curve
/// https://en.wikipedia.org/wiki/Hilbert_curve#Representation_as_Lindenmayer_system
/// </summary>
/// <param name="cpu">cpu to fill </param>
/// <param name="toDraw"> string to write </param>
/// <param name="startpoint"></param>
/// <param name="initialAngle"></param>
/// <param name="length"></param>
void HilbertCurve(CPU_Geometry& cpu, std::string toDraw, glm::vec3 startpoint, float initialAngle, float length) {
	cpu.verts.push_back(startpoint);
	glm::vec3 point = startpoint;
	float angle = initialAngle;
	for (char& c : toDraw) {
		if (c == 'F') {
			point.x = length * cos(angle) + point.x;
			point.y = length * sin(angle) + point.y;
			cpu.verts.push_back(point);
		}
		else if (c == 'M') {
			angle -= pi / 2.0f;
		}
		else if(c == 'P') {
			angle += pi / 2.0f;
		}
	}

}

/// <summary>
/// Rewrites string based on the rewriting rules for the L-system of the Hilbert curve
/// </summary>
/// <param name="drawString"> string to alter </param>
/// <returns>new string from the given string</returns>
std::string ModifyString(std::string drawString) {
	std::string newString = "";
	for (char& c : drawString) {
		if (c == 'A') newString += "MBFPAFAPFBM";
		else if (c == 'B') newString += "PAFMBFBMFAP";
		else newString += c;
	}

	return newString;
}

/// <summary>
/// /Generates scene 4 Hilbert curve (bonus)
/// </summary>
/// <param name="cpus">The different cpus for the levels </param>
void GenerateScene4(CPU_Geometry cpus[7]) {
	std::string drawString = "A";
	std::string drawString0 = ModifyString(drawString);
	std::string drawString1 = ModifyString(drawString0);
	std::string drawString2 = ModifyString(drawString1);
	std::string drawString3 = ModifyString(drawString2);
	std::string drawString4 = ModifyString(drawString3);
	std::string drawString5 = ModifyString(drawString4);
	std::string drawString6 = ModifyString(drawString5);

	ClearCPU(cpus);
	HilbertCurve(cpus[0], drawString0, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 1.5f);
	HilbertCurve(cpus[1], drawString1, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.5f);
	HilbertCurve(cpus[2], drawString2, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.2f);
	HilbertCurve(cpus[3], drawString3, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.1f);
	HilbertCurve(cpus[4], drawString4, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.05f);
	HilbertCurve(cpus[5], drawString5, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.025f);
	HilbertCurve(cpus[6], drawString6, glm::vec3(-0.75f, 0.75f, 0.0f), 0, 0.0125f);
	for (int j = 0; j < 7; j++) {
		for (int i = 0; i < cpus[j].verts.size(); i++) {
			cpus[j].cols.push_back(colours[j]);
		}
	}
}

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////       MAIN
int main() {

	Log::debug("Starting main");

	// WINDOW
	glfwInit();
	Window window(800, 800, "CPSC 453"); // can set callbacks at construction if desired

	GLDebug::enable();

	// SHADERS
	ShaderProgram shader("shaders/test.vert", "shaders/test.frag");

	// CALLBACKS
	auto callbacks = std::make_shared<MyCallbacks>(shader);
	window.setCallbacks(callbacks); // can also update callbacks to new ones

	// GEOMETRY
	CPU_Geometry cpuGeom;
	GPU_Geometry gpuGeom;

	CPU_Geometry cpuGeomLevel1;
	CPU_Geometry cpuGeomLevel2;
	CPU_Geometry cpuGeomLevel3;
	CPU_Geometry cpuGeomLevel4;
	CPU_Geometry cpuGeomLevel5;
	CPU_Geometry cpuGeomLevel6;

	CPU_Geometry cpus[7] = { cpuGeom, cpuGeomLevel1,cpuGeomLevel2,cpuGeomLevel3,cpuGeomLevel4,cpuGeomLevel5,cpuGeomLevel6 };
	GLenum typeOfDraw;
	typeOfDraw = GL_TRIANGLES;

	if (callbacks->GetScene() == 0) {
		GenerateScene0(cpus);
		typeOfDraw = GL_TRIANGLES;
	}

	else if (callbacks->GetScene() == 1) {
		GenerateScene1(cpus);
		typeOfDraw = GL_LINES;
	}
	else if (callbacks->GetScene() == 2) {
		GenerateScene2(cpus);
		typeOfDraw = GL_LINES;
	}
	else if (callbacks->GetScene() == 3) {
		GenerateScene3(cpus);
		typeOfDraw = GL_LINES;
	}
	else {
		GenerateScene4(cpus);
		typeOfDraw = GL_LINE_STRIP;
	}
	

	int level = callbacks->GetLevel();
	int scene = callbacks->GetScene();
	gpuGeom.setVerts(cpus[0].verts);
	gpuGeom.setCols(cpus[0].cols);


	// RENDER LOOP
	while (!window.shouldClose()) {
		glfwPollEvents();

		int newLevel = callbacks->GetLevel();
		int newScene = callbacks->GetScene();
		if (level != newLevel) {
			level = newLevel;
			gpuGeom.setVerts(cpus[level].verts);
			gpuGeom.setCols(cpus[level].cols);
		}
		if (scene != newScene) {
			scene = newScene;
			
			if (scene == 0) {
				GenerateScene0(cpus);
				typeOfDraw = GL_TRIANGLES;
			}
			else if (scene == 1) {
				GenerateScene1(cpus);
				typeOfDraw = GL_LINES;
			}
			else if (scene == 2) {
				GenerateScene2(cpus);
				typeOfDraw = GL_LINE_LOOP;
			}
			else if (scene == 3) {
				GenerateScene3(cpus);
				typeOfDraw = GL_LINES;
			}
			else {
			GenerateScene4(cpus);
			typeOfDraw = GL_LINE_STRIP;
			}
			gpuGeom.setVerts(cpus[level].verts);
			gpuGeom.setCols(cpus[level].cols);
		}

		shader.use();
		gpuGeom.bind();

		glEnable(GL_FRAMEBUFFER_SRGB);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(typeOfDraw, 0, cpus[level].verts.size());
		glDisable(GL_FRAMEBUFFER_SRGB); // disable sRGB for things like imgui

		window.swapBuffers();
	}

	glfwTerminate();
	return 0;
}
