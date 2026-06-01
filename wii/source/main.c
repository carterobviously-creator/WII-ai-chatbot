#include <gccore.h>
#include <network.h>
#include <stdio.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "http_server.h"

static void* xfb = NULL;
static GXRModeObj* rmode = NULL;

static int home_pressed(void)
{
    WPAD_ScanPads();
    return (WPAD_ButtonsDown(0) & WPAD_BUTTON_HOME) != 0;
}

int main(int argc, char** argv)
{
    char localip[16] = {0};
    char netmask[16] = {0};
    char gateway[16] = {0};
    s32 net_result;

    (void)argc;
    (void)argv;

    VIDEO_Init();
    WPAD_Init();

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    printf("WII-AI Chatbot\n");
    printf("Initializing network...\n");

    net_result = if_config(localip, netmask, gateway, TRUE, 20);
    if (net_result < 0) {
        printf("WiFi failed to initialize (if_config: %d).\n", net_result);
        printf("Check Wii Internet Settings, then reboot app.\n");
    } else {
        printf("Wii IP Address: %s\n", localip);
        printf("On iPad Safari, open: http://%s/\n\n", localip);
        printf("Press HOME on Wii Remote to stop server.\n\n");
        http_server_run(80, localip, home_pressed);
    }

    printf("Exiting WII-AI Chatbot.\n");
    WPAD_Shutdown();
    return 0;
}
