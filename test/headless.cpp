/*
 * Headless browser test runner for Agave
 * Loads HTML test pages, executes JS, captures results via document.title
 *
 * Usage: headless_test <html_file> [timeout_ms]
 * Output: Test results from document.title to stdout
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "macross.h"

static char g_result[4096] = {0};
static int g_result_set = 0;

static void on_alert(MaCrossView* view, const char* msg)
{
    if (msg && msg[0]) {
        strncpy(g_result, msg, sizeof(g_result) - 1);
        g_result_set = 1;
    }
}

static void on_set_title(MaCrossView* view, const char* title)
{
    if (title && title[0]) {
        strncpy(g_result, title, sizeof(g_result) - 1);
        g_result_set = 1;
    }
}

static void on_invalidate(MaCrossView* view, const MC_RECT* rect) { }
static void on_update(MaCrossView* view) { }

static char* read_file(const char* path)
{
    FILE* f = fopen(path, "rb");
    if (!f) return NULL;
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char* buf = (char*)malloc(size + 1);
    if (buf) { fread(buf, 1, size, f); buf[size] = '\0'; }
    fclose(f);
    return buf;
}

static void pump(MaCrossView* view, int ms)
{
    struct timespec ts = { 0, 10000000 };
    for (int i = 0; i < ms / 10; i++) {
        macross_event_dispatch();
        macross_view_update(view, 0);
        nanosleep(&ts, NULL);
        if (g_result_set) break;
    }
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <html_file> [timeout_ms]\n", argv[0]);
        return 2;
    }

    const char* path = argv[1];
    int timeout_ms = (argc >= 3) ? atoi(argv[2]) : 3000;

    char* html = read_file(path);
    if (!html) { fprintf(stderr, "Cannot read: %s\n", path); return 2; }

    int width = 800, height = 600, pitch = width * 4;
    macross_initialize(PIXEL_FORMAT_BGRA32, width, height);

    MC_CALLBACK_INFO cb;
    memset(&cb, 0, sizeof(cb));
    cb.cb_set_title = on_set_title;
    cb.cb_alert_box = on_alert;
    cb.cb_invalidate_rect = on_invalidate;
    cb.cb_update_view_now = on_update;
    macross_set_callback(&cb);

    unsigned char* buffer = (unsigned char*)calloc(pitch, height);
    MaCrossView* view = macross_view_create(buffer, width, height, pitch, NULL);

    /* Load page - use file:// URL for proper loading */
    char url[4096];
    if (path[0] == '/') {
        snprintf(url, sizeof(url), "file://%s", path);
    } else {
        char cwd[2048];
        getcwd(cwd, sizeof(cwd));
        snprintf(url, sizeof(url), "file://%s/%s", cwd, path);
    }
    macross_view_open_url(view, url);
    free(html);

    /* Wait for page to process and JS to execute */
    pump(view, timeout_ms);

    /* If result not captured via callback, try eval alert */
    if (!g_result_set) {
        macross_view_eval_script(view, "alert(document.title)");
        pump(view, 500);
    }

    /* Output */
    if (g_result_set && g_result[0])
        printf("%s\n", g_result);
    else
        printf("NO_RESULT\n");
    fflush(stdout);

    macross_view_destroy(view);
    free(buffer);
    _exit(g_result_set ? 0 : 1);
}
