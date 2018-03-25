#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <screen/screen.h>
const char *hg_id_string = "hourglass";
const char *bar_id_string = "bar";
const int barwidth = 32;
screen_window_t screen_bg_win = NULL;
screen_window_t screen_hg_win = NULL;
screen_window_t screen_bar_win = NULL;
screen_display_t screen_disp = NULL;


screen_window_t create_bg_window(const char *group, int dims[2], screen_context_t screen_ctx)
{
    /* Start by creating the application window and window group. */
    screen_window_t screen_win;
    screen_create_window(&screen_win, screen_ctx);
    screen_create_window_group(screen_win, group);
    
#if 1
    screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY,(void **)&screen_disp);
#endif

    /*set window property SCREEN_PROPERTY_VISIBLE to 0*/
    int vis = 0;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);

    /*set window property SCREEN_PROPERTY_COLOR to 0xffffff00(yellow)*/
    int color = 0xffffff00;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_COLOR, &color);

    /*set window property SCREEN_PROPERTY_BUFFER_SIZE and SCREEN_PROPERTY_SOURCE_SIZE*/
    int rect[4] = { 0, 0, 1, 1 };
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_SIZE, dims);

    /*set window property SCREEN_PROPERTY_SOURCE_POSITION*/
    int pos[2] = { -dims[0], -dims[1] };
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_POSITION, pos);

    /*create window buffer and get buffer handle*/
    screen_buffer_t screen_buf;
    screen_create_window_buffers(screen_win, 1);
    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);

    /*post window*/
    screen_post_window(screen_win, screen_buf, 1, rect, 0);
    return screen_win;
}

void create_bar_window(const char *group, const char *id, int dims[2])
{
    /*create context*/
    screen_context_t screen_ctx;
    screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);

    /*create child window*/
    screen_window_t screen_win;
    screen_create_window_type(&screen_win, screen_ctx, SCREEN_CHILD_WINDOW);

    /*join the child window to the group*/
    screen_join_window_group(screen_win, group);

    /*set window property SCREEN_PROPERTY_ID_STRING to "bar"*/
    screen_set_window_property_cv(screen_win, SCREEN_PROPERTY_ID_STRING, strlen(id), id);
    
#if 1
    screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY,(void **)&screen_disp);
#endif

    /*set window property SCREEN_PROPERTY_VISIBLE to 0*/
    int vis = 0;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);

    /*set window property SCREEN_PROPERTY_COLOR to 0xff0000ff(blue)*/
    int color = 0xff0000ff;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_COLOR, &color);

    /*set window property SCREEN_PROPERTY_BUFFER_SIZE to (1,1)*/
    int rect[4] = { 0, 0, 1, 1 };
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);

    /*set window property SCREEN_PROPERTY_SOURCE_POSITION to (-1,-1)*/
    int pos[2] = { -rect[2], -rect[3] };
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_SOURCE_POSITION, pos);

    /*create window buffer and get buffer handle*/
    screen_buffer_t screen_buf;
    screen_create_window_buffers(screen_win, 1);
    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);

    /*post window*/
    screen_post_window(screen_win, screen_buf, 1, rect, 0);
}

void create_hg_window(const char *group, const char *id, int dims[2])
{
    int i, j;

    /*create context*/
    screen_context_t screen_ctx;
    screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);

    /*create child window*/
    screen_window_t screen_win;
    screen_create_window_type(&screen_win, screen_ctx, SCREEN_CHILD_WINDOW);

    /*join child window to the group*/
    screen_join_window_group(screen_win, group);

    /*set window property SCREEN_PROPERTY_ID_STRING to "hourglass"*/
    screen_set_window_property_cv(screen_win, SCREEN_PROPERTY_ID_STRING, strlen(id), id);
    
#if 1
    screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY,(void **)&screen_disp);
#endif

    /*set window property SCREEN_PROPERTY_STATIC to 1*/
    int flag = 1;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_STATIC, &flag);

    /*set window property SCREEN_PROPERTY_VISIBLE to 0*/
    int vis = 0;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_VISIBLE, &vis);

    /*set window property SCREEN_PROPERTY_FORMAT to SCREEN_FORMAT_RGBA8888*/
    int format = SCREEN_FORMAT_RGBA8888;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_FORMAT, &format);

    /*set window property SCREEN_PROPERTY_USAGE to SCREEN_USAGE_WRITE*/
    int usage = SCREEN_USAGE_WRITE;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);

    /*set window property SCREEN_PROPERTY_TRANSPARENCY to SCREEN_TRANSPARENCY_SOURCE_OVER*/
    int transparency = SCREEN_TRANSPARENCY_SOURCE_OVER;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_TRANSPARENCY, &transparency);

    /*set window property SCREEN_PROPERTY_BUFFER_SIZE to (100,100)*/
    int rect[4] = { 0, 0, 100, 100 };
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);

    /*create window buffer*/
    screen_buffer_t screen_buf;
    screen_create_window_buffers(screen_win, 1);
    screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_buf);

    /*get window buffer pointer*/
    char *ptr = NULL;
    screen_get_buffer_property_pv(screen_buf, SCREEN_PROPERTY_POINTER, (void **)&ptr);

    /*get window buffer stride*/
    int stride = 0;
    screen_get_buffer_property_iv(screen_buf, SCREEN_PROPERTY_STRIDE, &stride);

    /*draw the hourglass pixmap*/
    for (i = 0; i < rect[3]; i++, ptr += stride) {
        for (j = 0; j < rect[2]; j++) {
            ptr[j*4] = 0xa0;
            ptr[j*4+1] = 0xa0;
            ptr[j*4+2] = 0xa0;
            ptr[j*4+3] = ((j >= i && j <= rect[3]-i) || (j <= i && j >= rect[3]-i)) ? 0xff : 0;
        }
    }

    /*post window*/
    screen_post_window(screen_win, screen_buf, 1, rect, 0);
}

int main(int argc, char **argv)
{
    int pos[2], size[2];
    int vis = 0;
    int type;
    screen_context_t screen_ctx;
    screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);
    
    int count = 0;
    screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &count);
    screen_display_t *screen_disps = calloc(count, sizeof(screen_display_t));
    screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_disps);
    screen_disp = screen_disps[1];
    free(screen_disps);
    
    int dims[2] = { 0, 0 };
    screen_get_display_property_iv(screen_disp, SCREEN_PROPERTY_SIZE, dims);
    
    char str[16];
    snprintf(str, sizeof(str), "%d", getpid());
    screen_bg_win = create_bg_window(str, dims, screen_ctx);
    create_bar_window(str, bar_id_string, dims);
    create_hg_window(str, hg_id_string, dims);
    
    screen_event_t screen_ev;
    screen_create_event(&screen_ev);
    
    while (1) {
        do {
            screen_get_event(screen_ctx, screen_ev, vis ? 0 : ~0);
            screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TYPE, &type);
            if(0 != type)
                printf("####### screen event type is %d\n",type);
            
            if (type == SCREEN_EVENT_POST) {
                screen_window_t screen_win;
                screen_get_event_property_pv(screen_ev, SCREEN_PROPERTY_WINDOW, (void **)&screen_win);
                screen_get_window_property_cv(screen_win, SCREEN_PROPERTY_ID_STRING, sizeof(str), str);
                printf("########### posted window string id is %s\n",str);
                
                if (!screen_bar_win && !strcmp(str, bar_id_string)) {
                    screen_bar_win = screen_win;
                } else if (!screen_hg_win && !strcmp(str, hg_id_string)) {
                    screen_hg_win = screen_win;
                }
                
                if (screen_bar_win && screen_hg_win) {
                    printf("######### %s:%d\n",__func__,__LINE__);
                    vis = 1;
                    /*set hg window size*/
                    screen_get_window_property_iv(screen_hg_win, SCREEN_PROPERTY_BUFFER_SIZE, size);
                    printf("########### hg window buffer size is %d,%d\n",size[0],size[1]);
                    screen_set_window_property_iv(screen_hg_win, SCREEN_PROPERTY_SIZE, size);

                    /*set hourglass window position to (10,10)*/
                    pos[0] = pos[1] = 10;
                    screen_set_window_property_iv(screen_hg_win, SCREEN_PROPERTY_POSITION, pos);

                    /*set bar window position to (0,0)*/
                    pos[0] = pos[1] = 0;
                    screen_set_window_property_iv(screen_bar_win, SCREEN_PROPERTY_POSITION, pos);

                    /*set hourglass window position to (0,0)*/
                    screen_set_window_property_iv(screen_bg_win, SCREEN_PROPERTY_POSITION, pos);

                    /*set bar window size to 32,720*/
                    size[0] = barwidth;
                    size[1] = dims[1];
                    screen_set_window_property_iv(screen_bar_win, SCREEN_PROPERTY_SIZE, size);

                    /*set backgroung window size to 1920,720*/
                    size[0] = dims[0];
                    screen_set_window_property_iv(screen_bg_win, SCREEN_PROPERTY_SIZE, size);

                    /*set window zorder*/
                    int zorder = 0;
                    screen_set_window_property_iv(screen_bg_win, SCREEN_PROPERTY_ZORDER, &zorder);
                    zorder++;
                    screen_set_window_property_iv(screen_bar_win, SCREEN_PROPERTY_ZORDER, &zorder);
                    zorder++;
                    screen_set_window_property_iv(screen_hg_win, SCREEN_PROPERTY_ZORDER, &zorder);

                    /*set 3 windows visible*/
                    screen_set_window_property_iv(screen_bg_win, SCREEN_PROPERTY_VISIBLE, &vis);
                    screen_set_window_property_iv(screen_hg_win, SCREEN_PROPERTY_VISIBLE, &vis);
                    screen_set_window_property_iv(screen_bar_win, SCREEN_PROPERTY_VISIBLE, &vis);
                    screen_flush_context(screen_ctx, SCREEN_WAIT_IDLE);
                    printf("######### %s:%d\n",__func__,__LINE__);
                }
            } else if (type == SCREEN_EVENT_CLOSE) {
                screen_window_t screen_win;
                screen_get_event_property_pv(screen_ev, SCREEN_PROPERTY_WINDOW, (void **)&screen_win);
                
                if (screen_win == screen_bar_win) {
                    screen_bar_win = NULL;
                } else if (screen_win == screen_hg_win) {
                    screen_hg_win = NULL;
                }
                
                screen_destroy_window(screen_win);
                
                if (!screen_bar_win || !screen_hg_win) {
                    vis = 0;
                }
            }
        } while (type != SCREEN_EVENT_NONE);
        
        if (vis) {
            if (++pos[0] > dims[0] - barwidth) {
                pos[0] = 0;
            }
                printf("######### %s:%d\n",__func__,__LINE__);
                screen_set_window_property_iv(screen_bar_win, SCREEN_PROPERTY_POSITION, pos);
                screen_flush_context(screen_ctx, SCREEN_WAIT_IDLE);
        }
    }
    
    screen_destroy_event(screen_ev);
    screen_destroy_context(screen_ctx);
    return EXIT_SUCCESS;
}




