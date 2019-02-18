//
//TKWindow.h
//2018-11-9

#include "TKWindow.h"
#include "TKBaseInfo.h"
#include <unistd.h>

TKWindow::TKWindow() {
    m_width = 0;
    m_height = 0;
    m_connection = nullptr;
    m_scene = nullptr;
}

TKWindow::~TKWindow() {
    if(m_scene != nullptr){
        delete m_scene;
    }
    xcb_destroy_window(m_connection, m_window);
    xcb_disconnect(m_connection);
}

TKWindow *TKWindow::createXcbWindow(uint32_t width, uint32_t height){
    TKWindow *window = new TKWindow;
    if(window->initXcbWindow(width, height)==false){
        delete window;
        window = nullptr;
    }
    return window;
}

bool TKWindow::initXcbWindow(uint32_t width, uint32_t height) {
    TKLog("init xcb window!\n");
    m_width = width;
    m_height = height;
    TKLog("window width=%d, height=%d\n", width, height);
    uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
    uint32_t values[2];
    m_isRunning = true;
    m_connection = xcb_connect(nullptr, nullptr);
    const xcb_setup_t *setup = xcb_get_setup(m_connection);
    xcb_screen_t *screen = xcb_setup_roots_iterator(setup).data;
    m_visualid = screen->root_visual;
    values[0] = screen->white_pixel;
    values[1] =
        XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_KEY_PRESS      |
        XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
        XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW   |
        XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE;
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, m_window, screen->root,
                      0, 0, m_width, m_height, 10, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, mask, values);
    xcb_map_window(m_connection, m_window);
    xcb_flush(m_connection);

    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window,
                        XCB_ATOM_WM_NAME, XCB_ATOM_STRING, 8, strlen("Triangle"), "Triangle");
    xcb_flush(m_connection);
    
    return true;
}


void TKWindow::initInstance() {
    uint32_t instExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &instExtensionCount, nullptr);
    std::vector<VkExtensionProperties> instExtensions(instExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &instExtensionCount, instExtensions.data());
    /*
    for(int i=0; i<instExtensionCount; ++i){
        printf("\t%s\n", instExtensions[i].extensionName);
    }
    */
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties>layerProps(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());
    /*
    for(int i=0; i<layerCount; ++i){
        printf("\t%s\n", layerProps[i].layerName);
    }
    */
    VkApplicationInfo appInfo;
    appInfo.sType               = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pNext               = nullptr;
    appInfo.pApplicationName    = "TKScene";
    appInfo.applicationVersion  = 1;
    appInfo.pEngineName         = "TKEngine";
    appInfo.engineVersion       = 1;
    appInfo.apiVersion          = VK_API_VERSION_1_1;

    VkInstanceCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.pApplicationInfo       = &appInfo;
    std::vector<const char *> enabledLayers;
    for(int i=0; i<layerCount; ++i){
        if(strcmp(layerProps[i].layerName, "VK_LAYER_LUNARG_device_simulation")){
            continue;
        }
        enabledLayers.push_back(layerProps[i].layerName);
    }
    info.enabledLayerCount      = enabledLayers.size();
    info.ppEnabledLayerNames    = enabledLayers.data();
    std::vector<const char *> enabledExtensions;
    for(int i=0; i<instExtensionCount; ++i){
        enabledExtensions.push_back(instExtensions[i].extensionName);
    }
    info.enabledExtensionCount   = enabledExtensions.size();
    info.ppEnabledExtensionNames = enabledExtensions.data();

    VkResult result = vkCreateInstance(&info, nullptr, &TKBaseInfo::Info()->instance);
    if(result != VK_SUCCESS){
        TKLog("create vulkan instance failed! ERROR: %d\n", result);
        return;
    }
    TKLog("step 1: create vulkan instance success!\n");
}

void TKWindow::initSurface(){
    TKBaseInfo::share()->enumeratePhysicalDevices();
    TKBaseInfo::share()->setGraphicsQueueIndex();
    /*
    uint32_t queueFamilyCount = UINT32_MAX;
    vkGetPhysicalDeviceQueueFamilyProperties(TKBaseInfo::Info()->physicalDevice,
                                             &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProp(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(TKBaseInfo::Info()->physicalDevice,
                                             &queueFamilyCount, queueFamilyProp.data());
    */
    VkBool32 supported = vkGetPhysicalDeviceXcbPresentationSupportKHR(TKBaseInfo::Info()->physicalDevice,
                                                                      0, m_connection, m_visualid);
    if(supported != VK_TRUE){
        TKLog("unsupported xcb window!\n");
        return;
    }
    
    VkSurfaceKHR surface; 
    VkXcbSurfaceCreateInfoKHR surfaceCreateInfo;
    surfaceCreateInfo.flags      = 0;
    surfaceCreateInfo.pNext      = nullptr;
    surfaceCreateInfo.sType      = VK_STRUCTURE_TYPE_XCB_SURFACE_CREATE_INFO_KHR;
    surfaceCreateInfo.connection = m_connection;
    surfaceCreateInfo.window     = m_window;
    VkResult result = vkCreateXcbSurfaceKHR(TKBaseInfo::Info()->instance, &surfaceCreateInfo,
                                            nullptr, &surface);
    if (result != VK_SUCCESS) {
        TKLog("Failed to create vulkan surface: %d\n", result);
        return;
    }
    TKLog("step 2 :create surface KHR success!\n");

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(TKBaseInfo::Info()->physicalDevice, surface,
                                         &formatCount, nullptr);
    TKBaseInfo::Info()->surfaceFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(TKBaseInfo::Info()->physicalDevice, surface,
                                         &formatCount, TKBaseInfo::Info()->surfaceFormats.data());
    TKBaseInfo::Info()->surface = surface;

    TKBaseInfo::share()->setPresentQueueIndex();
}

void TKWindow::startRefresh() {
    TKLog("start refresh!\n");
   
    xcb_generic_event_t *event = nullptr;
    xcb_client_message_event_t *cm;
    xcb_flush(m_connection);
     while(m_isRunning==true){ 
        event = xcb_poll_for_event(m_connection);
        if (event != nullptr){
            switch(event->response_type & ~0x80){
            case XCB_KEY_PRESS:{
                xcb_key_press_event_t *kp = (xcb_key_press_event_t *)event;
                TKLog("Key pressed in window %d\n", kp->detail);
                int keyNum = kp->detail;
                if(keyNum==9 || keyNum==24){
                    m_isRunning = false;
                }
            }
                break;
            case XCB_BUTTON_PRESS:{
            
            }
                break;
            default:
            
                break;
            }
            if(event != nullptr){
                free(event);
                event = nullptr;
            }
        }

        if(m_scene != nullptr){
            if(m_isRunning == true){
                m_scene->renderDraw();
            }else{
                m_scene->stopDraw();
            }
        }
        usleep(5000);
    }
    
    TKLog("refresh end!\n");
}

xcb_window_t TKWindow::window() const {
    return m_window;
}

xcb_connection_t* TKWindow::connection() const {
    return m_connection;
}

xcb_visualid_t TKWindow::visualid() const {
    return m_visualid;
}

void TKWindow::setScene(TKScene *scene){
    if(m_scene == nullptr){
        m_scene = scene;
    }
}
