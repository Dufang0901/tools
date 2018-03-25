#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <screen/screen.h>
const int barwidth = 32;
int ndisplays = 0;
struct {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    enum { detached, attached, focused } state;
} *displays;
screen_pixmap_t screen_pix = NULL;
screen_buffer_t screen_pbuf = NULL;

/*initialize pixmap*/
void pixmap(screen_context_t screen_ctx)
{
	int i, j;

    /*1 create pixmap*/
    screen_create_pixmap(&screen_pix, screen_ctx);

    /*2 set pixmap format to rgba8888*/
    int format = SCREEN_FORMAT_RGBA8888;
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_FORMAT, &format);

    /*3 set pixmap usage to write|native*/
    int usage = SCREEN_USAGE_WRITE | SCREEN_USAGE_NATIVE;
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_USAGE, &usage);

    /*4 set pixmap buffer size to 100x100*/
    int size[2] = { 100, 100 };
    screen_set_pixmap_property_iv(screen_pix, SCREEN_PROPERTY_BUFFER_SIZE, size);

    /*5 create pixmap buffer(pixmap can only have one buffer)*/
    screen_create_pixmap_buffer(screen_pix);

    /*6 get pixmap buffer handle*/
    screen_get_pixmap_property_pv(screen_pix, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)&screen_pbuf);

    /*7 get pixmap buffer pointer*/
    unsigned char *ptr = NULL;
    screen_get_buffer_property_pv(screen_pbuf, SCREEN_PROPERTY_POINTER, (void **)&ptr);

    /*8 get pixmap buffer stride*/
    int stride = 0;
    screen_get_buffer_property_iv(screen_pbuf, SCREEN_PROPERTY_STRIDE, &stride);

    /*9 set the color value of each pixel in pixmap buffer*/
    for (i = 0; i < size[1]; i++, ptr += stride) {
        for (j = 0; j < size[0]; j++) {
            ptr[j*4] = 0xa0;
            ptr[j*4+1] = 0xa0;
            ptr[j*4+2] = 0xa0;
            ptr[j*4+3] = ((j >= i && j <= size[1]-i) || (j <= i && j >= size[1]-i)) ? 0xff : 0;
        }
    }
}

void *display(void *arg)
{
    const int idx = (int)arg;
    int rect[4] = { 0, 0 };
    int realized = 0;
    int pos = 0;
    int attached;
    int focus;
    int i;

    /*create context*/
    screen_context_t screen_ctx;
    screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);

    /*get handles for all displays*/
    screen_display_t *screen_dpy = calloc(ndisplays, sizeof(screen_display_t));
    screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_dpy);

    /*create screen window*/
    screen_window_t screen_win;
    screen_create_window(&screen_win, screen_ctx);

    /*set the display associated with this thread to the window*/
    screen_set_window_property_pv(screen_win, SCREEN_PROPERTY_DISPLAY, (void **)&screen_dpy[idx]);

    /*set window usage to SCREEN_USAGE_NATIVE*/
    int usage = SCREEN_USAGE_NATIVE;
    screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_USAGE, &usage);

    /*get the attach and focus status of the display associated with this thread*/
    pthread_mutex_lock(&displays[idx].mutex);
    attached = displays[idx].state != detached ? 1 : 0;
    focus = displays[idx].state == focused ? 1 : 0;
    pthread_mutex_unlock(&displays[idx].mutex);

    /*create a event*/
    screen_event_t screen_ev;
    screen_create_event(&screen_ev);
    
    while (1) {
        if (attached) {
            if (!realized) {
                screen_get_display_property_iv(screen_dpy[idx], SCREEN_PROPERTY_SIZE, rect+2);
                screen_set_window_property_iv(screen_win, SCREEN_PROPERTY_BUFFER_SIZE, rect+2);
                printf("idx = %d width = %d height = %d\n",idx,rect[2],rect[3]);
                screen_create_window_buffers(screen_win, 2);
                realized = 1;
            }
            
            screen_buffer_t screen_buf[2];
            screen_get_window_property_pv(screen_win, SCREEN_PROPERTY_RENDER_BUFFERS, (void **)screen_buf);

            int bg[] = { SCREEN_BLIT_COLOR, 0xffffff00, SCREEN_BLIT_END };
            screen_fill(screen_ctx, screen_buf[0], bg);

            if (focus > 0) {
                /*draw the bar*/
                int bar[] = {
                    SCREEN_BLIT_COLOR, 0xff0000ff,
                    SCREEN_BLIT_DESTINATION_X, pos,
                    SCREEN_BLIT_DESTINATION_WIDTH, barwidth,
                    SCREEN_BLIT_END 
                };
                
                screen_fill(screen_ctx, screen_buf[0], bar);
                
                if (++pos > rect[2] - barwidth) {
                    for (i = (idx+1) % ndisplays; i != idx; i = (i+1) % ndisplays) {
                        pthread_mutex_lock(&displays[i].mutex);
                        
                        if (displays[i].state == attached) {
                        	displays[i].state = focused;
                            pthread_cond_signal(&displays[i].cond);
                            pthread_mutex_unlock(&displays[i].mutex);
                            break;
                        }
                        pthread_mutex_unlock(&displays[i].mutex);
                    }
                    
                    if (i != idx) {
                        pthread_mutex_lock(&displays[idx].mutex);
                        displays[idx].state = attached;
                        pthread_mutex_unlock(&displays[idx].mutex);
                        focus = -1;
                    }
                    pos = 0;
                }
            } else {
                focus = 0;
            }

            int hg[] = {
                SCREEN_BLIT_SOURCE_WIDTH, 100,
                SCREEN_BLIT_SOURCE_HEIGHT, 100,
                SCREEN_BLIT_DESTINATION_X, 10,
                SCREEN_BLIT_DESTINATION_Y, 10,
                SCREEN_BLIT_DESTINATION_WIDTH, 100,
                SCREEN_BLIT_DESTINATION_HEIGHT, 100,
                SCREEN_BLIT_TRANSPARENCY, SCREEN_TRANSPARENCY_SOURCE_OVER,
                SCREEN_BLIT_END
            };

            
            screen_blit(screen_ctx, screen_buf[0], screen_pbuf, hg);
            screen_post_window(screen_win, screen_buf[0], 1, rect, 0);
        }
        
        if (!attached && realized) {
            screen_destroy_window_buffers(screen_win);
            screen_flush_context(screen_ctx, 0);
            realized = 0;
        }
        
        if (focus != -1) {
            pthread_mutex_lock(&displays[idx].mutex);
            
            if (!focus) {
                printf("%s[%d]: idx=%d\n", __FUNCTION__, __LINE__, idx);
                pthread_cond_wait(&displays[idx].cond, &displays[idx].mutex);
                pos = 0;
            }
            attached = displays[idx].state != detached ? 1 : 0;
            focus = displays[idx].state == focused ? 1 : 0;
            pthread_mutex_unlock(&displays[idx].mutex);
        }
    }
    
    free(screen_dpy);
    return NULL;
}


int main(int argc, char **argv)
{
    int i, j, idx = -1;
    screen_context_t screen_ctx;

    /*create context(SCREEN_APPLICATION_CONTEXT)*/
    screen_create_context(&screen_ctx, SCREEN_APPLICATION_CONTEXT);

    /*get display count*/
    screen_get_context_property_iv(screen_ctx, SCREEN_PROPERTY_DISPLAY_COUNT, &ndisplays);
    
    /*create and initialize pixmap*/
    pixmap(screen_ctx);
    
    screen_display_t *screen_dpy = calloc(ndisplays, sizeof(screen_display_t));
    screen_get_context_property_pv(screen_ctx, SCREEN_PROPERTY_DISPLAYS, (void **)screen_dpy);
    
    displays = calloc(ndisplays, sizeof(*displays));
    
    for (i = 0; i < ndisplays; i++) {
        int active = 0;
        int id = 0;
        screen_get_display_property_iv(screen_dpy[i], SCREEN_PROPERTY_ATTACHED, &active);
        screen_get_display_property_iv(screen_dpy[i], SCREEN_PROPERTY_ID, &id);
        printf("i = %d active = %d, id = %d\n",i,active,id);
        if (active) {
            if (idx == -1) {
                displays[i].state = focused;
                idx = i;
            } else {
                displays[i].state = attached;
            }
        } else {
            displays[i].state = detached;
        }
        
        pthread_mutex_init(&displays[i].mutex, NULL);
        pthread_cond_init(&displays[i].cond, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, display, (void *)i);
    }

    /*create event*/
    screen_event_t screen_ev;
    screen_create_event(&screen_ev);
    
    while (1) {
        int type = SCREEN_EVENT_NONE;
        
        screen_get_event(screen_ctx, screen_ev, ~0);
        screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_TYPE, &type);
        if (type == SCREEN_EVENT_DISPLAY) {
            screen_display_t tmp = NULL;
            
            screen_get_event_property_pv(screen_ev, SCREEN_PROPERTY_DISPLAY, (void **)&tmp);
            
            for (i = 0; i < ndisplays; i++) {
                if (tmp == screen_dpy[i]) {
                    int active = 0;
                    
                    screen_get_event_property_iv(screen_ev, SCREEN_PROPERTY_ATTACHED, &active);
                    
                    if (active) {
                        int size[2];
                        screen_get_display_property_iv(tmp, SCREEN_PROPERTY_SIZE, size);
                        
                        if (size[0] == 0 || size[1] == 0) {
                            active = 0;
                        }
                    }
                    pthread_mutex_lock(&displays[i].mutex);
                    
                    if ((active && displays[i].state == detached) ||
                    (!active && displays[i].state != detached)) {
                        if (active) {
                            for (j = 0; j < ndisplays; j++) {
                                printf("%s[%d]: j=%d\n", __FUNCTION__, __LINE__, j);
                                
                                if (i != j) {
                                    pthread_mutex_lock(&displays[j].mutex);
                                    
                                    if (displays[j].state == focused) {
                                        displays[i].state = attached;
                                        pthread_mutex_unlock(&displays[j].mutex);
                                        break;
                                    }
                                    pthread_mutex_unlock(&displays[j].mutex);
                                }
                            }
                            if (displays[i].state == detached) {
                                displays[i].state = focused;
                            }
                        } else {
                            if (displays[i].state == focused) {
                                for (j = (i+1) % ndisplays; j != i; j = (j+1) % ndisplays) {
                                    printf("%s[%d]: j=%d\n", __FUNCTION__, __LINE__, j);
                                    pthread_mutex_lock(&displays[j].mutex);
                                    
                                    if (displays[j].state == attached) {
                                        displays[j].state = focused;
                                        pthread_cond_signal(&displays[j].cond);
                                        pthread_mutex_unlock(&displays[j].mutex);
                                        break;
                                    }
                                    pthread_mutex_unlock(&displays[j].mutex);
                                }
                            }
                            displays[i].state = detached;
                        }
                        pthread_cond_signal(&displays[i].cond);
                    }
                    pthread_mutex_unlock(&displays[i].mutex);
                    break;
                }
            }
        }
    }
    
    screen_destroy_pixmap(screen_pix);
    screen_destroy_event(screen_ev);
    screen_destroy_context(screen_ctx);
    free(displays);
    return EXIT_SUCCESS;
}