//
//  main.c
//  LexborTest - Testing lexbor HTML parser library integration with Playdate
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "pd_api.h"
#include "lexbor/html/html.h"

static int update(void* userdata);
static PlaydateAPI* pd = NULL;

// Test HTML to be parsed
const char* test_html =
    "<html><head><title>Test</title></head>"
    "<body><p>Hello, <b>Playdate!</b>"
    "<br>This HTML is malformed"
    "</body></html>";

// Result storage
static int parse_result = -1;
static int element_count = 0;

// Callback to count elements
static lxb_status_t
count_callback(lxb_dom_node_t *node, void *ctx)
{
    int *count = (int *)ctx;
    if (node->type == LXB_DOM_NODE_TYPE_ELEMENT) {
        (*count)++;
    }
    return LXB_STATUS_OK;
}

// Walk the DOM tree and count elements
static void
walk_tree(lxb_dom_node_t *node, int *count)
{
    while (node != NULL) {
        if (node->type == LXB_DOM_NODE_TYPE_ELEMENT) {
            (*count)++;
        }

        if (node->first_child != NULL) {
            walk_tree(node->first_child, count);
        }

        node = node->next;
    }
}

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    (void)arg;

    if (event == kEventInit)
    {
        pd = playdate;

        pd->system->logToConsole("Starting lexbor HTML parser test...");

        // Create HTML document
        lxb_html_document_t *document = lxb_html_document_create();
        if (document == NULL) {
            pd->system->logToConsole("Failed to create HTML Document");
            parse_result = -1;
        } else {
            pd->system->logToConsole("HTML Document created");

            // Parse HTML
            lxb_status_t status = lxb_html_document_parse(document,
                (const lxb_char_t *)test_html, strlen(test_html));

            if (status != LXB_STATUS_OK) {
                pd->system->logToConsole("Failed to parse HTML");
                parse_result = -2;
            } else {
                pd->system->logToConsole("HTML parsed successfully");
                parse_result = 0;

                // Get the document body
                lxb_dom_element_t *body = lxb_dom_interface_element(document->body);
                if (body != NULL) {
                    pd->system->logToConsole("Found document body");

                    // Count elements by walking the tree
                    element_count = 0;
                    walk_tree(lxb_dom_interface_node(document), &element_count);

                    char count_msg[64];
                    snprintf(count_msg, sizeof(count_msg), "Total elements found: %d", element_count);
                    pd->system->logToConsole(count_msg);
                }

                // Get the title
                size_t title_len;
                const lxb_char_t *title_text = lxb_html_document_title(document, &title_len);
                if (title_text != NULL && title_len > 0) {
                    char title_msg[128];
                    snprintf(title_msg, sizeof(title_msg), "Document title: %.*s", (int)title_len, title_text);
                    pd->system->logToConsole(title_msg);
                }
            }

            // Cleanup
            lxb_html_document_destroy(document);
            pd->system->logToConsole("Document destroyed, test complete");
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
    if (parse_result == 0) {
        status_text = "Lexbor: SUCCESS";
    } else if (parse_result == -1) {
        status_text = "Lexbor: Create failed";
    } else if (parse_result == -2) {
        status_text = "Lexbor: Parse failed";
    } else {
        status_text = "Lexbor: Unknown error";
    }

    pd->graphics->drawText(status_text, strlen(status_text), kASCIIEncoding, 10, 10);

    char elem_text[64];
    snprintf(elem_text, sizeof(elem_text), "Elements: %d", element_count);
    pd->graphics->drawText(elem_text, strlen(elem_text), kASCIIEncoding, 10, 30);

    pd->graphics->drawText("Check console for details", 25, kASCIIEncoding, 10, 50);

    pd->system->drawFPS(0, 0);

    return 1;
}
