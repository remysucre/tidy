-- (c) 2009-2011 John MacFarlane.  Released under MIT license.
-- Main lunamark module - provides access to reader and writer modules

local M = {}

M.reader = require("lunamark.reader")
M.writer = require("lunamark.writer")
M.util = require("lunamark.util")
M.entities = require("lunamark.entities")

return M
