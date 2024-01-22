
#include "knob.h"

void app_knob_callback(void* context, uint32_t index) {
    message("app_knob_callback");
    App* app = context;
    if(index == KnobEventDone) {
        view_dispatcher_send_custom_event(app->view_dispatcher, AppKnobEventDone);
    }
}

// Set a callback to invoke when knob has an event.
// @knob is a pointer to our Knob instance.
// @callback is a function to invoke when we have custom events.
void knob_set_callback(Knob* knob, KnobCallback callback, void* callback_context) {
    with_view_model(
        knob->view,
        KnobModel * model,
        {
            model->callback_context = callback_context;
            model->callback = callback;
        },
        true);
}

// Set the counter value for a given Knob instance.
// @knob pointer to a Knob instance.
void knob_set_counter(Knob* knob, uint32_t count) {
    with_view_model(
        knob->view, KnobModel * model, { model->counter = count; }, true);
}

// Sets the heading for displaying our knob.
// @knob pointer to a Knob instance.
// @heading the kind of knob.
void knob_set_heading(Knob* knob, char* heading) {
    with_view_model(
        knob->view, KnobModel * model, { model->heading = heading; }, true);
}

// Gets the current counter value for a given Knob instance.
// @knob pointer to a Knob instance.
uint32_t knob_get_counter(Knob* knob) {
    message("knob_get_counter");
    furi_assert(knob);

    uint32_t value = 0;
    with_view_model(
        knob->view, KnobModel * model, { value = model->counter; }, false);

    return value;
}

// Gets the view associated with our Knob.
// @knob pointer to a Knob instance.
View* knob_get_view(Knob* knob) {
    message("knob_get_view");
    furi_assert(knob);
    return knob->view;
}

// Free a Knob instance.
// @knob pointer to a Knob instance.
void knob_free(Knob* knob) {
    message("knob_free");
    furi_assert(knob);
    with_view_model(
        knob->view, KnobModel * model, { furi_string_free(model->buffer); }, true);
    view_free(knob->view);
    free(knob);
}

// Allocates a Knob instance.
Knob* knob_alloc() {
    message("knob_alloc");
    Knob* knob = malloc(sizeof(Knob));
    knob->view = view_alloc();

    // context passed to input_callback.
    view_set_context(knob->view, knob);

    // context passed to render.
    view_allocate_model(knob->view, ViewModelTypeLockFree, sizeof(KnobModel));
    with_view_model(
        knob->view,
        KnobModel * model,
        {
            model->buffer = furi_string_alloc();
            model->counter = 0;
            model->heading = NULL;
        },
        true);

    view_set_draw_callback(knob->view, knob_render_callback);
    view_set_input_callback(knob->view, knob_input_callback);
    return knob;
}

// Invoked by the draw callback to render the knob.
// @canvas is the canvas to draw on.
// @ctx is our model.
// static
void knob_render_callback(Canvas* canvas, void* ctx) {
    message("knob_render_callback");
    KnobModel* model = ctx;

    furi_string_printf(model->buffer, "Knob demo %d", model->counter);

    canvas_set_font(canvas, FontPrimary);

    if(model->heading) {
        canvas_draw_str_aligned(canvas, 64, 5, AlignCenter, AlignTop, model->heading);
    }

    canvas_draw_str_aligned(
        canvas, 15, 30, AlignLeft, AlignTop, furi_string_get_cstr(model->buffer));
}

// Invoked when input (button press) is detected.
// @input_even is the event the occured.
// @ctx is a pointer to our Knob instance.
//static
bool knob_input_callback(InputEvent* input_event, void* ctx) {
    message("knob_input_callback");
    Knob* knob = (Knob*)ctx;

    bool handled = false;

    if(input_event->type == InputTypePress && input_event->key == InputKeyUp) {
        bool updated = false;
        with_view_model(
            knob->view,
            KnobModel * model,
            {
                if(model->counter) {
                    model->counter--;
                    updated = true;
                }
            },
            updated);
        handled = true;
    } else if(input_event->type == InputTypePress && input_event->key == InputKeyDown) {
        with_view_model(
            knob->view,
            KnobModel * model,
            { model->counter++; },
            true); // Render new data.
        handled = true;
    } else if(input_event->type == InputTypePress && input_event->key == InputKeyOk) {
        with_view_model(
            knob->view,
            KnobModel * model,
            {
                if(model->callback) {
                    message("invoking callback");
                    model->callback(model->callback_context, KnobEventDone);
                } else {
                    message("no callback set; use knob_set_callback first.");
                }
            },
            false); // No new data.
        handled = true;
    }

    return handled;
}