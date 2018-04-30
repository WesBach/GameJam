#ifndef _Keyboard_HG_
#define _Keyboard_HG_
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

class Keyboard {
public:
	const static int KeyCount = 356;
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void key_resolution(int index,float deltaTime);
	static void setKeys();
	static bool isShiftKeyDown(int mods, bool bByItself = true);
};

#endif // !_Keyboard_HG_
