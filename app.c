#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/widget.h>
#include <gui/modules/variable_item_list.h>
#include <notification/notification.h>
#include <notification/notification_messages.h>

#include "misc.h"

#include "scene_manager.h"


/**
 * @brief Initialize application data
 * 
 * @return App* 
 */
App* app_alloc() {
    // Create app
    App* app = malloc(sizeof(App));

    app->volume = 50;
    app->frequency = 440;

    // Create scene manager
    app->scene_manager = scene_manager_alloc(&app_scene_handlers, app);

    // Create view dispatcher
    app->view_dispatcher = view_dispatcher_alloc();
    view_dispatcher_enable_queue(app->view_dispatcher);
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, app_scene_custom_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, app_back_event_callback);

    // Create modules
    app->knob = knob_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewKnob, knob_get_view(app->knob));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, AppViewWidget, widget_get_view(app->widget));

    // Returning fully initialized app
    return app;
}

/**
 * @brief Free application data
 * 
 * @param app 
 */
void app_free(App* app) {
    // Destroy modules
    view_dispatcher_remove_view(app->view_dispatcher, AppViewKnob);
    free(app->knob);

    view_dispatcher_remove_view(app->view_dispatcher, AppViewWidget);
    free(app->widget);

    // Destroy view dispatcher
    view_dispatcher_free(app->view_dispatcher);

    // Destroy scene manager
    scene_manager_free(app->scene_manager);

    // Destroy app
    free(app);
}

/**
 * @brief Main entry point
 * 
 * @param p 
 * @return int32_t 
 */
int32_t main_scenes(void* p) {
    UNUSED(p);

    // Create app object
    App* app = app_alloc();

    // Attach GUI and view dispatcher
    Gui* gui = furi_record_open(RECORD_GUI);
    view_dispatcher_attach_to_gui(app->view_dispatcher, gui, ViewDispatcherTypeFullscreen);

    // Show first scene
    scene_manager_next_scene(app->scene_manager, AppSceneSetVolume);
    view_dispatcher_run(app->view_dispatcher);

    // Free resources
    app_free(app);
    furi_record_close(RECORD_GUI);

    return 0;
}

/**
 * @brief Message log with a delay
 * 
 * @param message 
 */
void message(char* message) {
    FURI_LOG_I(TAG, message);
    furi_delay_ms(10);
}