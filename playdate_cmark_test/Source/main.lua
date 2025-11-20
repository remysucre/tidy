-- cmark CommonMark Parser Test for Playdate
import "CoreLibs/graphics"

local gfx = playdate.graphics

-- Playdate uses 'import' instead of 'require', but cmark expects 'require'
-- Set up the package system for standard Lua module loading
if not package then
    _G.package = {
        loaded = {},
        path = "",
        cpath = ""
    }
end

-- Provide loadstring for compatibility with libraries expecting Lua 5.1 behavior
if not loadstring then
    _G.loadstring = function(str, chunkname)
        -- Return nil to indicate compilation not supported
        return nil, "load not supported in Playdate"
    end
end

-- Create a custom require function
_G.require = function(modname)
    -- Check if already loaded
    if package.loaded[modname] then
        return package.loaded[modname]
    end

    -- Handle built-in Lua libraries
    local builtins = {
        coroutine = coroutine,
        string = string,
        table = table,
        math = math,
        io = io,
        os = os,
        debug = debug,
        utf8 = utf8,
    }

    if builtins[modname] then
        package.loaded[modname] = builtins[modname]
        return builtins[modname]
    end

    -- Try to load the file - playdate.file.run looks for .pdz (compiled bytecode)
    local filepath = modname:gsub("%.", "/")
    local chunk, err = playdate.file.run(filepath)

    if not chunk then
        error("module '" .. modname .. "' not found: " .. tostring(err))
    end

    -- Cache and return the module
    package.loaded[modname] = chunk or true
    return chunk
end

-- Test CommonMark content
local test_markdown = [[
# Hello Playdate!

This is a **CommonMark** test with:

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
    print("Initializing cmark CommonMark test...")

    -- Try to load and use cmark
    local success, result = pcall(function()
        -- Load cmark
        local cmark = require("cmark")

        -- Parse CommonMark to HTML
        -- cmark typically provides a simple parse function
        local html = cmark.parse_document(test_markdown)

        -- Convert parsed document to HTML string
        return cmark.render_html(html)
    end)

    if success then
        parse_success = true
        result_html = result
        print("=== CommonMark Parse SUCCESS ===")
        print("HTML Output:")
        print(result_html)
        print("=== End Output ===")
    else
        parse_success = false
        error_message = tostring(result)
        print("=== CommonMark Parse FAILED ===")
        print("Error:", error_message)
        print("=== End Error ===")
    end
end

-- Initialize the test
initialize()

function playdate.update()
    gfx.clear()

    if parse_success then
        gfx.drawText("*cmark: SUCCESS*", 10, 10)
        gfx.drawText("Parsed CommonMark to HTML", 10, 30)
        gfx.drawText("Check console for output", 10, 50)
    else
        gfx.drawText("*cmark: FAILED*", 10, 10)
        gfx.drawText("Error occurred", 10, 30)
        gfx.drawText("Check console for details", 10, 50)

        -- Show first part of error message on screen
        local short_error = string.sub(error_message, 1, 40)
        gfx.drawText(short_error, 10, 70)
    end

    playdate.drawFPS(0, 220)
end
