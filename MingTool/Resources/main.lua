function __G__TRACKBACK__(errorMessage)
    print("----------------------------------------")
    print("LUA ERROR: " .. tostring(errorMessage) .. "\n")
    print(debug.traceback("", 2))
    print("----------------------------------------")
end

str = "I am so cool"  
tbl = {name = "shun", id = 20114442}  

function add(a,b)  
	print("add a+b")
    return a+b  
end


add(1,2)

luaTest("123",2)