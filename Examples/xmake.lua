option("examples", { description = "Build examples files", default = true })

if (has_config("examples")) then
   -- includes("BasicRendering/xmake.lua")
    includes("RHI/xmake.lua")
    includes("NewRendering/xmake.lua")
end