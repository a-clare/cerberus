#ifndef CERBERUS_VIS_VIS_H_
#define CERBERUS_VIS_VIS_H_

#include <stdint.h>
#include <stdbool.h>
// We include imgui here so we can call any imgui functions (buttons, trees, checkboxes) where ever we need to
#include "imgui.h"
#include "implot.h"
/**
 * @brief Initilaize a new window. This window is where all rendering/gui draws
 * will happen
 * 
 * @param windowName the name to apepar at the top/title bar
 * @param width starting width of the window in pixels
 * @param height starting width of the window in pixels
 */
void vis_init(const char* windowName,
              uint32_t width,
              uint32_t height);


/**
 * @return true if the main window is still open/running
 * @return false if the main window is closing/closed (most likely user hit x/esc)
 */
bool vis_is_window_running();

void vis_frame_start();
void vis_frame_end();

#endif