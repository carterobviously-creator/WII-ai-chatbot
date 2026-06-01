#include <gccore.h>
#include <stdio.h>
#include <string.h>
#include <wiiuse/wpad.h>

#include "tv_chat.h"

static void* xfb = NULL;
static GXRModeObj* rmode = NULL;

int main(int argc, char** argv)
{
    (void)argc;
    (void)argv;

    VIDEO_Init();
    WPAD_Init();
    WPAD_SetDataFormat(WPAD_CHAN_0, WPAD_FMT_BTNS);

    rmode = VIDEO_GetPreferredMode(NULL);
    xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));
    console_init(xfb, 20, 20, rmode->fbWidth, rmode->xfbHeight, rmode->fbWidth * VI_DISPLAY_PIX_SZ);

    VIDEO_Configure(rmode);
    VIDEO_SetNextFramebuffer(xfb);
    VIDEO_SetBlack(FALSE);
    VIDEO_Flush();
    VIDEO_WaitVSync();
    if (rmode->viTVMode & VI_NON_INTERLACE) VIDEO_WaitVSync();

    /* No WiFi needed — everything runs on the TV */
    tv_chat_init();
    tv_chat_run();

    printf("\nExiting WII-AI Chatbot.\n");
    WPAD_Shutdown();
    return 0;
}

