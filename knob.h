#include <furi.h>
#include <furi_hal.h>

#include <furi_hal_gpio.h>
#include <furi_hal_resources.h>

#include <gui/gui.h>
#include <locale/locale.h>

#include <gui/scene_manager.h>
#include <gui/view_dispatcher.h>

#include <gui/modules/widget.h>

#include "misc.h"

#ifndef __KNOB_H__
#define __KNOB_H__

typedef enum {
    KnobEventDone,
} KnobCustomEvents;

typedef void (*KnobCallback)(void* context, uint32_t index);

typedef struct Knob {
    View* view;
} Knob;

typedef struct KnobModel {
    FuriString* buffer;
    uint16_t counter;
    char* heading;
    KnobCallback callback;
    void* callback_context;
} KnobModel;

void app_knob_callback(void* context, uint32_t index);

void knob_set_counter(Knob* knob, uint32_t count);

void knob_set_heading(Knob* knob, char* heading);

// static
void knob_set_callback(Knob* knob, KnobCallback callback, void* callback_context);

uint32_t knob_get_counter(Knob* knob);

// static
void knob_render_callback(Canvas* canvas, void* ctx);

//static
bool knob_input_callback(InputEvent* input_event, void* ctx);

Knob* knob_alloc();

View* knob_get_view(Knob* knob);

#endif