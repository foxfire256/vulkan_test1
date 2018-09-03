#ifndef GFX_HPP
#define GFX_HPP

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>

class gfx
{
public:
	gfx();
	virtual ~gfx();

	void init();
	void render();
	void deinit();

	int done();

protected:
	GLFWwindow *window;
	VkInstance instance;
	// optional
	VkApplicationInfo app_info = {};
	// not optional
	VkInstanceCreateInfo create_info = {};
	VkDebugUtilsMessengerEXT messenger_callback;

	std::vector<const char*> get_required_extensions();
	bool check_validation_layer_support();

	static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback
	(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	)
	{
		std::cerr << "Validation layer: " << pCallbackData->pMessage << std::endl;
		return VK_FALSE;
	}
};

#endif
