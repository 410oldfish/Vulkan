#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <vector>

const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = { "VK_LAYER_KHRONOS_validation" };
#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

class HelloTriangleApplication {
public :
    GLFWwindow* window;

    void run()
    {
        initWindow();
        initVulkan();
        mainLoop();
        //cleanup();
    }

private :
    VkInstance instance;

    bool checkValidationLayerSupport()
    {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : validationLayers)
        {
            bool layerFound = false;
            for (const auto& layerProperties : availableLayers)
            {
                if (strcmp(layerName, layerProperties.layerName) == 0)
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound)
            {
                return false;
            }

        }


        return true;
    }

    void initWindow()
    {
        glfwInit();
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
    }

    void initVulkan()
    {
      
        createInstance();
    }

    void createInstance()
    {
        if (enableValidationLayers && !checkValidationLayerSupport())
        {
            throw std::runtime_error("validation_layers_requested,_but_not_available!");
        }

        VkApplicationInfo appInfo = {};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hellp_Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No_Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        VkInstanceCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;

        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;

        if (enableValidationLayers)
        {
            createInfo.enabledLayerCount = static_cast<uint32_t>(validationLayers.size());
            createInfo.ppEnabledExtensionNames = validationLayers.data();
        }
        else 
        {
            createInfo.enabledLayerCount = 0;
        }

#pragma region LogExtensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(glfwExtensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        std::cout << "available_extensions:" << std::endl;
        for (const auto& ext : extensions)
        {
            std::cout << "\t" << ext.extensionName << std::endl;
        }
#pragma endregion


        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed_to_create_instance!");
        }
    }

    void mainLoop()
    {
        while (!glfwWindowShouldClose(window))
        {
            glfwPollEvents();
        }
    }

    void cleanup()
    {
        vkDestroyInstance(instance, nullptr);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
};

int main()
{
    HelloTriangleApplication app;
    try {
        app.run();
    }
    catch (const std::exception& e)
    {

        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
