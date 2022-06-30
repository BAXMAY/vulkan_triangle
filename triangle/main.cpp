//
//  main.cpp
//  triangle
//
//  Created by Baramee Thunyapoo on 29/6/22.
//

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <algorithm>

//
//  GLOBALS
//

const uint32_t WIDTH = 800;
const uint32_t HEIGHT = 600;

class HelloTriangleApplication {
public:
    void run() {
        initWindow();
        initVulkan();
        mainLoop();
        cleanUp();
    }

//  Private Variable Members
private:
    GLFWwindow* window;
    VkInstance instance;

//  Private Function Members
private:
    
    /*
     
     Check if all glfwExtension are available
     
     */
    bool isAllExtensionSupported( const char** glfwExtensions, uint32_t requiredExtensionCount ) {
        
        //  Get all available extensions
        uint32_t extensionCount = 0;
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
        std::vector<VkExtensionProperties> extensions(extensionCount);
        vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
        
        std::cout << "available extensions:\n";
        
        for (const auto& extension : extensions) {
            std::cout << '\t' << extension.extensionName << '\n';
        }
        
        //  Check each glfwExtension in available extension list
        for (auto i = 0; i < requiredExtensionCount; ++i) {
            auto result = std::find_if(extensions.begin(), extensions.end(), [&i, &glfwExtensions](VkExtensionProperties &extension){ return strcmp(extension.extensionName, glfwExtensions[i]) == 0; } );
            if (result == extensions.end())
                return false;
        }
        
        return true;
    }
    
    void createInstance() {
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Hello Triangle";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;
        
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
        
        if (!isAllExtensionSupported(glfwExtensions, glfwExtensionCount)) {
            throw std::runtime_error("Not all extension supported!");
        }
        
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo;
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;
        
        VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
        
        if (result != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }
    
    void initWindow() {
        glfwInit();
        
        //  No OpenGL Context
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        
        //  Disable resize window
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
        
        //  Create window
        window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
        
    }
    
    void initVulkan() {
        createInstance();
    }
    
    void mainLoop() {
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }
    
    void cleanUp() {
        vkDestroyInstance(instance, nullptr);
        
        glfwDestroyWindow(window);
        
        glfwTerminate();
    }
    
};

int main() {
    HelloTriangleApplication app;
    
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
