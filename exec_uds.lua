
-- An experiment to run lua from BUSMASTER node simulation


-- Opens a file in append mode
file = io.open("test.txt", "a")

-- sets the default output file as test.lua
io.output(file)

-- appends a word test to the last line of the file
io.write("\n-- End test write experiment")

-- closes the open file
io.close(file)

Trace("Hello from lua")

function printToFile(e)

    print(e)
end
