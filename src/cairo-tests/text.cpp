/*
 * Copyright 2014 © Samsung Research America, Silicon Valley
 *
 * Use of this source code is governed by the 3-Clause BSD license
 * specified in the COPYING file included with this source code.
 */
#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cairo.h>

#include "caskbench.h"
#include "caskbench_context.h"
#include "cairo-shapes.h"

static int max_dim;
static char rand_text_array[19][100];

void gen_random(char *s, const int len) {
    static const char alphanum[] =
        "ABCD EFG HIJKL MNOPQ RSTUVW XYZ";

    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    s[len] = 0;
}

int
ca_setup_text(caskbench_context_t *ctx)
{
    max_dim = MIN (ctx->canvas_width, ctx->canvas_height)/2;
    for(int i = 0;i<19;i++)
    {
        gen_random (rand_text_array[i],i+18);
    }
    return 1;
}

void
ca_teardown_text(void)
{
}

int
ca_test_text(caskbench_context_t *ctx)
{
    cairo_t *cr = ctx->cairo_cr;

    for (int i = 0; i < ctx->size; i++)
    {
        double font_size = 18;
        int ypos = 0, xpos = 0;

        cairo_set_source_rgb (cr, 0, 0, 0);
        cairo_rectangle (cr, 0, 0, ctx->canvas_width ,ctx->canvas_height);
        cairo_fill (cr);

        for(font_size = 18; font_size <= 36; font_size++)
        {
            cairo_set_font_size (cr, font_size);
            cairoRandomizeColor(ctx);

            cairo_status_t status;
            cairo_scaled_font_t *font;
            font = cairo_get_scaled_font (cr);
            cairo_glyph_t *glyphs = NULL;
            int num_glyphs;
            cairo_text_cluster_t *clusters = NULL;
            int num_clusters;
            cairo_text_cluster_flags_t cluster_flags;

            char text[(int)font_size];
            gen_random (text,font_size);

            status = cairo_scaled_font_text_to_glyphs (font,
                                                       xpos, ypos,
                                                       text, font_size,
                                                       &glyphs, &num_glyphs,
                                                       &clusters, &num_clusters, &cluster_flags);

            if (status == CAIRO_STATUS_SUCCESS) {
                cairo_show_text_glyphs (cr,
                                        rand_text_array[(int)font_size-18],font_size,
                                        glyphs, num_glyphs,
                                        clusters, num_clusters, cluster_flags);
            }
            ypos += (font_size/2);
        }

        for(font_size = 36; font_size >= 18; font_size--)
        {
            cairo_set_font_size (cr, font_size);
            cairoRandomizeColor(ctx);

            cairo_status_t status;
            cairo_scaled_font_t *font;
            font = cairo_get_scaled_font (cr);
            cairo_glyph_t *glyphs = NULL;
            int num_glyphs;
            cairo_text_cluster_t *clusters = NULL;
            int num_clusters;
            cairo_text_cluster_flags_t cluster_flags;

            char text[(int)font_size];
            gen_random (text,font_size);

            status = cairo_scaled_font_text_to_glyphs (font,
                                                       xpos, ypos,
                                                       text, font_size,
                                                       &glyphs, &num_glyphs,
                                                       &clusters, &num_clusters, &cluster_flags);

            if (status == CAIRO_STATUS_SUCCESS) {
            cairo_show_text_glyphs (cr,
                                    rand_text_array[(int)font_size-18],font_size,
                                    glyphs, num_glyphs,
                                    clusters, num_clusters, cluster_flags);
            }
            ypos += (font_size/2);
        }
    }

    return 1;
}

/*
  Local Variables:
  mode:c++
  c-file-style:"stroustrup"
  c-file-offsets:((innamespace . 0)(inline-open . 0)(case-label . +))
  indent-tabs-mode:nil
  fill-column:99
  End:
*/
// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=8:softtabstop=4:fileencoding=utf-8:textwidth=99 :
