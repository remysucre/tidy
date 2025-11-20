//
//  main.c
//  cmarkTest - Testing cmark CommonMark library integration with Playdate
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"
#include "cmark.h"

static int update(void* userdata);
static PlaydateAPI* pd = NULL;

// Test CommonMark to be parsed
const char* test_markdown =
    "# Hello Playdate!\n\n"
    "This is a **CommonMark** test with:\n\n"
    "- Item 1\n"
    "- Item 2\n"
    "- Item 3\n\n"
    "## Features\n\n"
    "Here's some *italic* text and **bold** text.\n\n"
    "A [link](https://play.date) to Playdate's website.\n\n"
    "### Code\n\n"
    "```lua\n"
    "local x = 42\n"
    "print(x)\n"
    "```\n\n"
    "> This is a blockquote.\n"
    "> It can span multiple lines.\n";

// Buffer to store the result
static char result_buffer[8192];
static int cmark_result = -1;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    (void)arg;

    if (event == kEventInit)
    {
        pd = playdate;

        pd->system->logToConsole("Starting cmark CommonMark test...");
        pd->system->logToConsole("cmark version: %s", CMARK_VERSION_STRING);

        // Use the simple cmark API to convert markdown to HTML
        size_t len = strlen(test_markdown);
        char* html = cmark_markdown_to_html(test_markdown, len, 0);

        if (html != NULL)
        {
            // Check if output fits in our buffer
            size_t html_len = strlen(html);
            if (html_len < sizeof(result_buffer) - 1)
            {
                memcpy(result_buffer, html, html_len);
                result_buffer[html_len] = '\0';
                cmark_result = 0;

                pd->system->logToConsole("=== cmark Output ===");
                pd->system->logToConsole(result_buffer);
                pd->system->logToConsole("=== End Output ===");
            }
            else
            {
                pd->system->logToConsole("Output too large for buffer (%zu bytes)", html_len);
                cmark_result = -2;
            }

            // Free the HTML buffer allocated by cmark
            free(html);
        }
        else
        {
            pd->system->logToConsole("cmark_markdown_to_html returned NULL");
            cmark_result = -1;
        }

        pd->system->setUpdateCallback(update, pd);
    }

    return 0;
}

static int update(void* userdata)
{
    PlaydateAPI* pd = userdata;

    pd->graphics->clear(kColorWhite);

    // Display result on screen
    const char* status_text;
    if (cmark_result == 0) {
        status_text = "cmark: SUCCESS";
    } else if (cmark_result == -2) {
        status_text = "cmark: Buffer too small";
    } else {
        status_text = "cmark: FAILED";
    }

    pd->graphics->drawText(status_text, strlen(status_text), kASCIIEncoding, 10, 10);
    pd->graphics->drawText("Check console for output", 23, kASCIIEncoding, 10, 30);

    pd->system->drawFPS(0, 0);

    return 1;
}
