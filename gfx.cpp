#include "gfx.hpp"

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <vector>

const std::vector<const char*> validation_layers = {
	"VK_LAYER_LUNARG_standard_validation"
};

#ifdef NDEBUG
const bool enable_validation_layers = false;
#else
const bool enable_validation_layers = true;
#endif

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
                                      const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback)
{
	auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
	if(func != nullptr)
	{
		return func(instance, pCreateInfo, pAllocator, pCallback);
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback,
                                   const VkAllocationCallbacks* pAllocator)
{
	auto func = (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
	if(func != nullptr)
	{
		func(instance, callback, pAllocator);
	}
}

gfx::gfx()
{
}

gfx::~gfx()
{
}

void gfx::init()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	window = glfwCreateWindow(800, 600, "Vulkan Test 1", nullptr, nullptr);
	/*
	uint32_t extension_count = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr);
	std::vector<VkExtensionProperties> extensions(extension_count);
	vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, extensions.data());
	std::cout << extension_count << " extensions supported" << std::endl;
	std::cout << "Available Vulkan extensions:" << std::endl;
	for(const auto& extension : extensions)
	{
		std::cout << "\t" << extension.extensionName << std::endl;
	}
	*/
	app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	app_info.pApplicationName = "Vulkan Test 1";
	app_info.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.pEngineName = "No Engine";
	app_info.engineVersion = VK_MAKE_VERSION(1, 0, 0);
	app_info.apiVersion = VK_API_VERSION_1_0;

	create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	create_info.pApplicationInfo = &app_info;

	auto extensions2 = get_required_extensions();
	create_info.enabledExtensionCount = static_cast<uint32_t>(extensions2.size());
	create_info.ppEnabledExtensionNames = extensions2.data();

	if(enable_validation_layers)
	{
		create_info.enabledLayerCount = static_cast<uint32_t>(validation_layers.size());
		create_info.ppEnabledLayerNames = validation_layers.data();
	}
	else
	{
		create_info.enabledLayerCount = 0;
	}

	VkResult result = vkCreateInstance(&create_info, nullptr, &instance);
	if(result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create instance!");
	}

	if(enable_validation_layers)
	{
		VkDebugUtilsMessengerCreateInfoEXT messenger_create_info = {};
		messenger_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
		messenger_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
		messenger_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
		messenger_create_info.pfnUserCallback = debug_callback;

		if(CreateDebugUtilsMessengerEXT(instance, &messenger_create_info, nullptr, &messenger_callback) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to set up debug callback!");
		}

		if(check_validation_layer_support())
			std::cout << "Validation layers OK!" << std::endl;
	}
}

void gfx::render()
{
	glfwPollEvents();
}

void gfx::deinit()
{
	if(enable_validation_layers)
	{
		DestroyDebugUtilsMessengerEXT(instance, messenger_callback, nullptr);
	}

	vkDestroyInstance(instance, nullptr);

	glfwDestroyWindow(window);

	glfwTerminate();
}

int gfx::done()
{
	return glfwWindowShouldClose(window);
}

std::vector<const char*> gfx::get_required_extensions()
{
	uint32_t glfw_extension_count = 0;
	const char** glfw_extensions;
	glfw_extensions = glfwGetRequiredInstanceExtensions(&glfw_extension_count);

	std::vector<const char*> extensions(glfw_extensions, glfw_extensions + glfw_extension_count);

	if(enable_validation_layers)
	{
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

bool gfx::check_validation_layer_support()
{
	uint32_t layer_count;
	vkEnumerateInstanceLayerProperties(&layer_count, nullptr);

	std::vector<VkLayerProperties> available_layers(layer_count);
	vkEnumerateInstanceLayerProperties(&layer_count, available_layers.data());

	for(const char* layer_name : validation_layers)
	{
		bool layer_found = false;

		for(const auto& layer_properties : available_layers)
		{
			if(strcmp(layer_name, layer_properties.layerName) == 0)
			{
				layer_found = true;
				break;
			}
		}

		if(!layer_found)
		{
			return false;
		}
	}

	return true;
}
