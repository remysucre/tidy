-- Lunamark Markdown Parser Test for Playdate
import "CoreLibs/graphics"

local gfx = playdate.graphics

-- Playdate uses 'import' instead of 'require', but lunamark expects 'require'
-- Set up the package system for standard Lua module loading
if not package then
    _G.package = {
        loaded = {},
        path = "",
        cpath = ""
    }
end

-- Create a custom require function
_G.require = function(modname)
    -- Check if already loaded
    if package.loaded[modname] then
        return package.loaded[modname]
    end

    -- Try to load the file
    local filepath = modname:gsub("%.", "/") .. ".lua"
    local chunk, err = playdate.file.run(filepath)

    if not chunk then
        error("module '" .. modname .. "' not found: " .. tostring(err))
    end

    -- Cache and return the module
    package.loaded[modname] = chunk or true
    return chunk
end

-- Test markdown content
local test_markdown = [[
# Hello Playdate!

This is a **markdown** test with:

- Item 1
- Item 2
- Item 3

## Features

Here's some *italic* text and **bold** text.

A [link](https://play.date) to Playdate's website.

### Code

```lua
local x = 42
print(x)
```

> This is a blockquote.
> It can span multiple lines.
]]

local result_html = ""
local parse_success = false
local error_message = ""

function initialize()
    print("Initializing Lunamark markdown test...")

    -- Try to load and use lunamark
    local success, result = pcall(function()
        -- Load lunamark
        local lunamark = require("lunamark")

        -- Create HTML writer
        local opts = {}
        local writer = lunamark.writer.html.new(opts)

        -- Create markdown parser
        local parse = lunamark.reader.markdown.new(writer, opts)

        -- Parse the test markdown
        return parse(test_markdown)
    end)

    if success then
        parse_success = true
        result_html = result
        print("=== Markdown Parse SUCCESS ===")
        print("HTML Output:")
        print(result_html)
        print("=== End Output ===")
    else
        parse_success = false
        error_message = tostring(result)
        print("=== Markdown Parse FAILED ===")
        print("Error:", error_message)
        print("=== End Error ===")
    end
end

-- Initialize the test
initialize()

function playdate.update()
    gfx.clear()

    if parse_success then
        gfx.drawText("*Lunamark: SUCCESS*", 10, 10)
        gfx.drawText("Parsed markdown to HTML", 10, 30)
        gfx.drawText("Check console for output", 10, 50)
    else
        gfx.drawText("*Lunamark: FAILED*", 10, 10)
        gfx.drawText("Error occurred", 10, 30)
        gfx.drawText("Check console for details", 10, 50)

        -- Show first part of error message on screen
        local short_error = string.sub(error_message, 1, 40)
        gfx.drawText(short_error, 10, 70)
    end

    playdate.drawFPS(0, 220)
end
