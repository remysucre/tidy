//
//  main.c
//  TidyTest - Testing HTML Tidy library integration with Playdate
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"
#include "tidy.h"
#include "tidybuffio.h"

static int update(void* userdata);
static PlaydateAPI* pd = NULL;

// Test HTML to be tidied
const char* test_html =
    "<html><head><title>Test</title></head>"
    "<body><p>Hello, <b>Playdate!</b>"
    "<br>This HTML is malformed"
    "</body></html>";

// Buffer to store the result
static char result_buffer[4096];
static int tidy_result = -1;

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    (void)arg;

    if (event == kEventInit)
    {
        pd = playdate;

        // Test HTML Tidy library
        TidyBuffer output = {0};
        TidyBuffer errbuf = {0};
        int rc = -1;
        Bool ok;

        TidyDoc tdoc = tidyCreate();

        pd->system->logToConsole("Starting HTML Tidy test...");

        // Set options
        ok = tidyOptSetBool(tdoc, TidyXhtmlOut, yes);
        if (!ok) {
            pd->system->logToConsole("Failed to set TidyXhtmlOut option");
        }

        ok = tidyOptSetInt(tdoc, TidyWrapLen, 0);
        if (!ok) {
            pd->system->logToConsole("Failed to set TidyWrapLen option");
        }

        ok = tidySetErrorBuffer(tdoc, &errbuf);
        if (!ok) {
            pd->system->logToConsole("Failed to set error buffer");
        }

        // Parse the input
        rc = tidyParseString(tdoc, test_html);
        if (rc >= 0) {
            pd->system->logToConsole("Parse successful");
            rc = tidyCleanAndRepair(tdoc);
            if (rc >= 0) {
                pd->system->logToConsole("Clean and repair successful");
                rc = tidyRunDiagnostics(tdoc);
                if (rc >= 0) {
                    pd->system->logToConsole("Diagnostics successful");

                    // Save output
                    rc = tidySaveBuffer(tdoc, &output);
                    if (rc >= 0) {
                        pd->system->logToConsole("Save buffer successful");

                        // Copy output to our result buffer
                        if (output.size < sizeof(result_buffer) - 1) {
                            memcpy(result_buffer, output.bp, output.size);
                            result_buffer[output.size] = '\0';
                            tidy_result = 0;

                            pd->system->logToConsole("=== Tidy Output ===");
                            pd->system->logToConsole(result_buffer);
                            pd->system->logToConsole("=== End Output ===");
                        } else {
                            pd->system->logToConsole("Output too large for buffer");
                            tidy_result = -2;
                        }
                    } else {
                        pd->system->logToConsole("tidySaveBuffer failed");
                    }
                } else {
                    pd->system->logToConsole("tidyRunDiagnostics failed");
                }
            } else {
                pd->system->logToConsole("tidyCleanAndRepair failed");
            }
        } else {
            pd->system->logToConsole("tidyParseString failed");
        }

        // Print errors if any
        if (errbuf.bp) {
            pd->system->logToConsole("=== Tidy Errors ===");
            pd->system->logToConsole((char*)errbuf.bp);
            pd->system->logToConsole("=== End Errors ===");
        }

        // Cleanup
        tidyBufFree(&output);
        tidyBufFree(&errbuf);
        tidyRelease(tdoc);

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
    if (tidy_result == 0) {
        status_text = "HTML Tidy: SUCCESS";
    } else if (tidy_result == -2) {
        status_text = "HTML Tidy: Buffer too small";
    } else {
        status_text = "HTML Tidy: FAILED";
    }

    pd->graphics->drawText(status_text, strlen(status_text), kASCIIEncoding, 10, 10);
    pd->graphics->drawText("Check console for output", 23, kASCIIEncoding, 10, 30);

    pd->system->drawFPS(0, 0);

    return 1;
}
