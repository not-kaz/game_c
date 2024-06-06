#ifndef GFX_H
#define GFX_H

int graphics_init(void);
void graphics_finish(void);
void graphics_clear_framebuffer(void);
void graphics_present_framebuffer(void);
void graphics_get_window_size(int *width_out, int *height_out);

#endif
