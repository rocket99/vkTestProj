//
//TKWindow.h
//2018-11-9

#include "TKWindow.h"
#include "TKBaseInfo.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <unistd.h>

#ifdef __cplusplus
}
#endif
	
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

TKWindow *TKWindow::createXcbWindow(uint32_t width, uint32_t height, const std::string &title){
    TKWindow *window = new TKWindow;
    if(window->initXcbWindow(width, height, title)==false){
        delete window;
        window = nullptr;
    }
    return window;
}

bool TKWindow::initXcbWindow(uint32_t width, uint32_t height, const std::string &title) {
    TKLog("init xcb window!\n");
    m_width = width;
    m_height = height;
	m_title = title;
    TKLog("window width=%d, height=%d\n", width, height);
    m_isRunning = true;
	int scr;
    m_connection = xcb_connect(nullptr, &scr);
	if(m_connection == nullptr){
		printf("Could not find a compatible Vulkan ICD!\n");
		return false;
	}
    const xcb_setup_t *setup = xcb_get_setup(m_connection);
    xcb_screen_iterator_t iter = xcb_setup_roots_iterator(setup);
	while(scr-- > 0){
		xcb_screen_next(&iter);
	}
	xcb_screen_t *screen = iter.data;

	uint32_t value_mask, value_list[32];

	value_mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
	m_visualid = screen->root_visual;
    value_list[0] = screen->black_pixel;
    value_list[1] =
        XCB_EVENT_MASK_KEY_RELEASE  |
		XCB_EVENT_MASK_KEY_PRESS    |
        XCB_EVENT_MASK_EXPOSURE     |
		XCB_EVENT_MASK_STRUCTURE_NOTIFY |
        XCB_EVENT_MASK_POINTER_MOTION   |
		XCB_EVENT_MASK_BUTTON_PRESS |
        XCB_EVENT_MASK_KEY_RELEASE;
    m_window = xcb_generate_id(m_connection);
    xcb_create_window(m_connection, XCB_COPY_FROM_PARENT, m_window, screen->root,
                      0, 0, m_width, m_height, 0, XCB_WINDOW_CLASS_INPUT_OUTPUT,
                      screen->root_visual, value_mask, value_list);

	xcb_intern_atom_reply_t *reply = this->intern_atom_helper(m_connection, true, "WM_PROTOCOLS");
	xcb_intern_atom_reply_t *atom_wm_delete_window = this->intern_atom_helper(m_connection, false, "WM_DELETE_WINDOW");
	xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window, reply->atom,
						4, 32, 1, &atom_wm_delete_window->atom);    
	free(reply);
    xcb_change_property(m_connection, XCB_PROP_MODE_REPLACE, m_window, XCB_ATOM_WM_NAME,
						XCB_ATOM_STRING, 8, strlen(m_title.c_str()), m_title.c_str());

    xcb_map_window(m_connection, m_window);
    xcb_flush(m_connection);

    return true;
}


void TKWindow::initInstance() {
    uint32_t instExtensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &instExtensionCount, nullptr);
    std::vector<VkExtensionProperties> instExtensions(instExtensionCount);
    vkEnumerateInstanceExtensionProperties(nullptr, &instExtensionCount, instExtensions.data());
/*    
    for(uint32_t i=0; i<instExtensionCount; ++i){
        printf("\t%s\n", instExtensions[i].extensionName);
    }
*/  
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties>layerProps(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, layerProps.data());
/*  
    for(uint32_t i=0; i<layerCount; ++i){
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
    appInfo.apiVersion          = VK_API_VERSION_1_0;

    VkInstanceCreateInfo info;
    info.sType                  = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    info.pNext                  = nullptr;
    info.flags                  = 0;
    info.pApplicationInfo       = &appInfo;

	std::vector<const char *> enabledLayers;
	enabledLayers.push_back("VK_LAYER_LUNARG_standard_validation");
    info.enabledLayerCount      = enabledLayers.size();
    info.ppEnabledLayerNames    = enabledLayers.data();
	
    std::vector<const char *> enabledExtensions;
	enabledExtensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
	enabledExtensions.push_back("VK_KHR_surface");
	enabledExtensions.push_back("VK_KHR_xcb_surface");
	info.enabledExtensionCount   = enabledExtensions.size();
    info.ppEnabledExtensionNames = enabledExtensions.data();

	printf("enabled layers:\n");
	for(uint32_t i=0; i<enabledLayers.size(); ++i){
		printf("\t%s\n", enabledLayers[i]);
	}
	printf("enabled extensions:\n");
	for(uint32_t i=0; i<enabledExtensions.size(); ++i){
		printf("\t%s\n", enabledExtensions[i]);
	}
	
    VkResult result = vkCreateInstance(&info, nullptr, &VK_INFO->instance);
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
    vkGetPhysicalDeviceQueueFamilyProperties(VK_INFO->physicalDevice,
                                             &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilyProp(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(VK_INFO->physicalDevice,
                                             &queueFamilyCount, queueFamilyProp.data());
    */
    VkBool32 supported = vkGetPhysicalDeviceXcbPresentationSupportKHR(VK_INFO->physicalDevice,
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
    VkResult result = vkCreateXcbSurfaceKHR(VK_INFO->instance, &surfaceCreateInfo,
                                            nullptr, &surface);
    if (result != VK_SUCCESS) {
        TKLog("Failed to create vulkan surface: %d\n", result);
        return;
    }
    TKLog("step 2 :create surface KHR success!\n");

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(VK_INFO->physicalDevice, surface,
                                         &formatCount, nullptr);
    VK_INFO->surfaceFormats.resize(formatCount);
    vkGetPhysicalDeviceSurfaceFormatsKHR(VK_INFO->physicalDevice, surface,
                                         &formatCount, VK_INFO->surfaceFormats.data());
    VK_INFO->surface = surface;
    TKBaseInfo::share()->setPresentQueueIndex();
}

void TKWindow::startRefresh() {
    TKLog("start refresh!\n");
    xcb_generic_event_t *event = nullptr;
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
        usleep(10000);
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

xcb_intern_atom_reply_t *TKWindow::intern_atom_helper(xcb_connection_t *conn, bool only_if_exists, const char *str){
	xcb_intern_atom_cookie_t cookie = xcb_intern_atom(conn, only_if_exists, strlen(str), str);
	return xcb_intern_atom_reply(conn, cookie, nullptr);
}

