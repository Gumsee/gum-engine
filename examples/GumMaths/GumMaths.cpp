#include <Engine.h>
#include <Extras/Debug.h>
#include <Extras/Terminal.h>
#include <Maths/vec.h>
#include <Maths/mat.h>
#include <cstddef>
#include "../Examples.h"


World* Examples::createGumMathsExample()
{
    World* pWorld = new World(Gum::Window->getSize());
	pWorld->getObjectManager()->getSkybox()->useGradiant(true);

	Terminal::init();
	
	Gum::Output::print("#");
	Gum::Output::print("# Vector Testing:");
	Gum::Output::print("#");

	tvec<double, 2> vec2_1(2.0, 3.0);
	Gum::Output::print("vec2_1: (2, 3) double");
	vec2_1.print();
	Gum::Output::print("");

	tvec<float, 5> vec5_1(vec2_1, 4.0f, 6.0f, 6.0f);
	Gum::Output::print("vec5_1: (vec2_1, 4, 6, 6) float");
	vec5_1.print();
	Gum::Output::print("");

	Gum::Output::print("#");
	Gum::Output::print("# Matrix Testing:");
	Gum::Output::print("#");

	mat<float, 4, 2> mat4x2_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f);
	Gum::Output::print("mat4x2_1: float");
	mat4x2_1.print();
	Gum::Output::print("");

	mat<float, 2, 4> mat2x4_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f);
	Gum::Output::print("mat2x4_1: float");
	mat2x4_1.print();
	Gum::Output::print("");

	Gum::Output::print("mat2x4_1 * mat4x2_1");
	(mat2x4_1 * mat4x2_1).print();
	Gum::Output::print("");

	Gum::Output::print("transpose(mat2x4_1)");
	mat2x4_1.transpose(mat2x4_1).print();
	Gum::Output::print("");

	mat<float, 4, 4> mat4x4_1(1.0f,0.0f,0.0f,0.0f,0.0f,2.0f,0.0f,0.0f,0.0f,0.0f,3.0f,0.0f,0.0f,0.0f,0.0f,4.0f);
	Gum::Output::print("mat4x4_1: fl0x4_1) * mat4x4_1");
	(mat4x4_1.inverse(mat4x4_1) * mat4x4_1).print();
	Gum::Output::print("");

	mat<float, 3, 3> mat3x3_1(1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f);
	Gum::Output::print("mat3x3_1: float");
	mat3x3_1.print();
	Gum::Output::print("");

	Gum::Output::print("determinate(mat3x3_1)");
	Gum::Output::print(mat3x3_1.determinant());
	Gum::Output::print("");

	mat<float, 3, 5> mat3x5_1;
	mat3x5_1 *= 5.0f;
	Gum::Output::print("mat3x5_1: float");
	mat3x5_1.print();
	Gum::Output::print("");

	Gum::Output::print("mat3x5_1 * vec5_1");
	(mat3x5_1 * vec5_1).print();
	Gum::Output::print("");

	ivec2 ttestvec;
	ttestvec.x = 10;

	std::cout << sizeof(tvec<int, 2>) << " " << sizeof(int) * 2 << std::endl;
	std::cout << sizeof(tvec<int, 3>) << " " << sizeof(int) * 3 << std::endl;
	std::cout << sizeof(tvec<int, 4>) << " " << sizeof(int) * 4 << std::endl;

	vec3 xvec(1, 0, 0);
	mat3 Amat(2,0,1,0,1,0,1,0,2);

	for(int i = 1; i <= 50; i++)
	{
		std::cout << "norm(x" << i-1 << ") =\t" << vec3::normalize(xvec).toString() << std::endl;
		xvec = Amat * vec3::normalize(xvec);
		std::cout << "x" << i << " =\t" << xvec.toString() << std::endl;
	}


	return pWorld;
}
