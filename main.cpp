#ifdef _WIN32
#include <Windows.h>
#endif

#include <GLFW/glfw3.h>

#include <glad/glad.h>

#include <memory>

#include <cstdlib>

#include "app.hpp"

#ifndef WINDOW_TITLE
#define WINDOW_TITLE "Nodes2C"
#endif

namespace
{
	/**
	 * \brief Used for releasing memory allocated by a window.
	 */
	struct glfw_window_deleter final
	{
		void operator()(GLFWwindow* window) const
		{
			if (window)
				glfwDestroyWindow(window);
		}
	};

	struct app_deleter final
	{
		void operator()(app* app) const
		{
			if (app)
				app_teardown(app);
		}
	};
}

#ifdef _WIN32
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
	if (glfwInit() != GLFW_TRUE)
		return EXIT_FAILURE;

	glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	{
		std::unique_ptr<GLFWwindow, glfw_window_deleter> window(
			glfwCreateWindow(640, 480, WINDOW_TITLE, nullptr, nullptr));
		if (!window)
		{
			glfwTerminate();
			return EXIT_FAILURE;
		}

		glfwMakeContextCurrent(window.get());

		gladLoadGLES2Loader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));

		std::unique_ptr<app, app_deleter> app(app_setup(window.get()));

		while (!glfwWindowShouldClose(window.get()))
		{
			glfwPollEvents();

			app_loop(app.get());

			glfwSwapBuffers(window.get());
		}
	}

	glfwTerminate();

	return EXIT_SUCCESS;
}
