//
//TKWindow.h
//2018-11-9

#ifndef __TKWindow_h__
#define __TKWindow_h__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#include "TKScene.h"

class TKWindow {
public:
    TKWindow();
    ~TKWindow();
    xcb_window_t window() const;
    xcb_visualid_t visualid() const;
    xcb_connection_t *connection() const;
    static TKWindow *createXcbWindow(uint32_t width, uint32_t height, const std::string &title="Triangle");
    bool initXcbWindow(uint32_t width, uint32_t height, const std::string &title);     
    void startRefresh();

    void initInstance();
    void initSurface();

    void setScene(TKScene *scene);
private:
    uint32_t m_width, m_height;
    xcb_connection_t *m_connection;
    xcb_window_t     m_window;
    xcb_visualid_t   m_visualid;
    bool m_isRunning = false;
	std::string m_title;
    TKScene *m_scene;
	xcb_intern_atom_reply_t *intern_atom_helper(xcb_connection_t *conn, bool only_in_exists, const char *str);
};

#endif

