
#ifndef RELEASE
    #define NK_INCLUDE_FIXED_TYPES
    #include "SDL_render.h"
#define NK_INCLUDE_STANDARD_IO
    #define NK_INCLUDE_STANDARD_VARARGS
    #define NK_INCLUDE_DEFAULT_ALLOCATOR
    #define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
    #define NK_INCLUDE_FONT_BAKING
    #define NK_INCLUDE_DEFAULT_FONT
    #define NK_IMPLEMENTATION
    #define NK_ZERO_COMMAND_MEMORY
    #include <nuklear.h>
    #include <stddef.h>
    #include <stdio.h>
#endif
#include "password_input.h"
#include "utils.h"
#include <string.h>

#include <SDL.h>


static nk_bool nk_filter_template(const struct nk_text_edit *box,
                                  nk_rune unicode) {
    NK_UNUSED(box);
    if (unicode == 'A' || unicode == 'a' || unicode == 'S' || unicode == 'n') {
        return nk_true;
    }
    return nk_false;
}

static void remove_spaces(char *s) {
    char *d = s;
    do {
        while (*d == ' ') {
            ++d;
        }
    } while (*s++ = *d++);
}


void zeropass_ui(struct nk_context *ctx, int w, int h) {

    static char username[255] = "\0";
    static char site[255] = "\0";
    static char master_password[16];
    static char template[5] = "ASan";

    static int chk_capital = nk_true;
    static int chk_lower = nk_true;
    static int chk_number = nk_true;
    static int chk_special = nk_true;
    static int chk_template = nk_false;
    static int password_len = 16;

    static int rounds = 1;
    static char custom_template[33];

    if (nk_begin(ctx, "ZeroPass", nk_rect(0, 0, w, h), NK_WINDOW_NO_SCROLLBAR)) {

        /* ------------------------------------------------------------------------ */
        nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 2);

        nk_layout_row_push(ctx, 0.33f);
        nk_label(ctx, "UserName: ", NK_TEXT_CENTERED | NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 0.67f);
        nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, username, 255, NULL);

        nk_layout_row_end(ctx);

        /* ------------------------------------------------------------------------ */
        nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 2);

        nk_layout_row_push(ctx, 0.33f);
        nk_label(ctx, "WebSite:", NK_TEXT_CENTERED | NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 0.67f);
        nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, site, 255, NULL);

        nk_layout_row_end(ctx);

        /* ------------------------------------------------------------------------ */
        nk_layout_row_begin(ctx, NK_DYNAMIC, 0, 2);

        nk_layout_row_push(ctx, 0.33f);
        nk_label(ctx, "Master Password:", NK_TEXT_CENTERED | NK_TEXT_LEFT);

        nk_layout_row_push(ctx, 0.67f);
        password_input(ctx, NK_EDIT_FIELD, master_password, 16, NULL);

        nk_layout_row_end(ctx);
        /* ------------------------------------------------------------------------ */

        nk_layout_row_dynamic(ctx, 0, 4);
        nk_checkbox_label(ctx, "A-Z", &chk_capital);
        nk_checkbox_label(ctx, "a-z", &chk_lower);
        nk_checkbox_label(ctx, "0-9", &chk_number);
        nk_checkbox_label(ctx, "!@#", &chk_special);

        /* ------------------------------------------------------------------------ */

        nk_layout_row_dynamic(ctx,0,1);
        nk_label(ctx,"A(UpperCase) a(LowerCase) n(Number) S($%@)",NK_TEXT_ALIGN_CENTERED);

        /* ------------------------------------------------------------------------ */

        nk_layout_row_dynamic(ctx, 0, 2);
        nk_checkbox_label(ctx, "Custom Template", &chk_template);
        if (chk_template) {
            nk_edit_string_zero_terminated(ctx, NK_EDIT_FIELD, custom_template,
                                           33, nk_filter_template);
        }

        nk_layout_row_dynamic(ctx, 0, 2 - chk_template); // if chk_template is checked then -1

        if (!chk_template) {
            nk_property_int(ctx, "Length", strlen(custom_template),
                            &password_len, 32, 1, 1);
        } 

        nk_property_int(ctx, "Rounds", 0, &rounds, 1000, 1, 3);

        template[0] = (chk_capital) ? 'A' : ' ';
        template[1] = (chk_special) ? 'S' : ' ';
        template[2] = (chk_number ) ? 'n' : ' ';
        template[3] = (chk_lower  ) ? 'a' : ' ';

        nk_layout_row_dynamic(ctx, 0, 1);
        if (nk_button_label(ctx, "Generate & Copy")) {
            remove_spaces(template);
            char *master_key =
                get_masterkey(username, site, master_password, rounds);
            char *password = get_password(
                (chk_template == 1) ? custom_template : template, master_key,
                (chk_template == 1) ? strlen(custom_template) : (size_t)password_len);
            
            SDL_SetClipboardText(password);
            free(master_key);
            free(password);
        }
    }
    nk_end(ctx);
}
