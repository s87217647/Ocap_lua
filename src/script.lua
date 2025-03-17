


--a = array.new(1000)
--for i = 1, 1000 do
--    array.set(a, i, i % 2 == 0) -- a[i] = (i % 2 == 0)
--end
--print(array.get(a, 10)) --> true
--print(array.get(a, 11)) --> false
--print(array.size(a)) --> 1000


 -- testing IO

-- here is it's right to read, how do I limit this? Can I overload it?
-- can I make something that's completely seamless to use, just like the regular lua?


function createCounter()
    local count = 0  -- Private upvalue

    return {
        increment = function()
            count = count + 1
        end,
        getCount = function()
            return count
        end
    }
end

local counter = createCounter()
counter.increment()
counter.increment()
print(counter.getCount())  -- Output: 2
print(counter.count)  -- Output: nil (count is private)

-- sandboxing
local sandbox = {
    print = print,   -- Allow print
    pairs = pairs,   -- Allow pairs function
    ipairs = ipairs, -- Allow ipairs function
    tonumber = tonumber, -- Allow tonumber
    math = {
        abs = math.abs,
        sqrt = math.sqrt
    }
}

-- Set the environment of the function to the sandbox
local code = "return math.sqrt(16)"  -- Simple user code
local func = load(code)
--setfenv(func, sandbox)
-- print(func())  -- This will run safely within the sandbox




local newgt = {}


setmetatable(newgt, {__index = table})

print(newgt.name)


local print = print
a = 15
--_ENV = {g = _G}
a = 1

--print(g.a, _ENV.a)

--function factory (_ENV)
--    return function () return a end
--end

--f1 = factory (_ENV)
--print(f1())

--f1 = factory{a = 6}
--f2 = factory{a = 7}
--print(f1())


function mysterious()
    local x = 1

end

a = mysterious()

--print(a.z)







--print(x)
print(print)

--print(ocap)
--
--os = require(os)

print("Here I am, rock you like a hurricane")
for k, v in pairs(_ENV) do
    print(k, v)
end

print(os)


--function ocapp()
--    table = {
--    }
--
--end
Ocap = {
    --function new( obj, rights) end
    obj,
    rights,

    get = function() return "from ocap get" end

}
ocapInstance = {}

setmetatable(ocapInstance, {__index = Ocap})

local sndInstance = {__index = ocapInstance}

function Ocap.new(o)
    self.o = o or {}
    setmetatable(o, sndInstance)
    return o
end


print(ocapInstance.get())




--ocapObj = ocapp()

--print (ocapObj)

--x = {
--    local val = 5
--
--}

Account = {balance = 0}

function Account:withdraw(v)
    print(self.balance)
end

Account = { balance = 0,
            withdraw = function (self, v)
                self.balance = self.balance - v
            end
}

function  Account: deposit(v)
    self.balance = self.balance + v
end

function Account:new(o)
    o = o or {}
    setmetatable(o, {__index = Account})
    return o
end


a = Account.new ({balance = 0})
b = Account.new ({balance = 0})


Account.withdraw(a, 100)
Account.withdraw(b, 1)

print(a, b, a.balance, b.balance)

--Account.deposit(Account, 20)
--Account.withdraw(Account, 10)
--print(Account.balance)


print(ocap)

a = array.new(1000)
for i = 1, 1000 do
    array.set(a, i, i % 2 == 0) -- a[i] = (i % 2 == 0)
end



function createCounter()
    local count = 0  -- Private upvalue

    return {
        increment = function()
            count = count + 1
        end,
        getCount = function()
            return count
        end
    }
end

print(createCounter())



b = array.new(50)
for i = 1, 50 do
    array.set(b, i, i % 2 == 0) -- a[i] = (i % 2 == 0)
end

array.set(b, 10, false)
array.set(b, 11, true)


print(array.get(a, 10)) --> true
print(array.get(a, 11)) --> false
print(array.size(a)) --> 1000

print(array.get(b, 10)) --> true
print(array.get(b, 11)) --> false
print(array.size(b)) --> 1000
--


x = {
    val = 5,

    get = function(self)
        return val
    end
}

print("The exciting part")

print(x)

--oCapObjx = ocap.newOcapObj(x)

--print(ocap.getWrappedObj(oCapObjx))

--for k,v in pairs(x) do
--    print(k, v)
--end

--print(table.unpack(x))

oo = ocap.newOcapObj(x);
print(oo)

print("the more exciting part")

caps = ocap.getCapabilities(oo)
print(caps)
print(table.unpack(caps));
print("Coz it's empty")

caps = ocap.receiveCap(oo, "os", os)
print(caps.os)
--print(table.unpack(caps))
for k, v in pairs(caps) do
    print(k, v)
end

print("end")

--print(table.unpack(ocap.getCapabilities(oo)))



--x.val = 6
--
--t1 = ocap.getWrappedObj(oo)
--
---- it's a shallow copy
--print(x.val)
--print(t1.val)
--
--
----print(ocap.getWrappedObj("str"))
--
--print(ocap.print())


--for k, v in pairs(arr) do
--    print(k, v)
--end


--print(table.unpack(x))

function printTable(tb)
    for k,v in pairs(tb) do
        print(k, v)
    end

end


---- third party code, verify credit card #, but no internet IO
---- in abstract execute a code we don't trust with limited authority
---- but then, simple sandbox will do
---- so, what it looks like
--- part 1: without any restriction

--- let's say for now, the valid credit card number is just 15, 16, digits #

str1 = "1234 5678 2234 5678"
str2 = "1234 5678 2234 567"
str3 = "aaaa"

verifier = {
    val = "value from verifier";

    isValid = function(str)
        digitsCount = 0

        for i = 1, #str do
            char = string.sub(str, i, i);
            if tonumber(char) ~= nil then
                digitsCount = digitsCount + 1
            end
        end

        return digitsCount == 15 or digitsCount == 16
    end;

    -- nice func, let's say read, or something not in io
    -- like print or math or something
    goodFunc = function(num)
        print("sqrt of input is: ", math.sqrt(num))
    end,

    evilFunc = function(str)
        -- do evil shit
        --local socket = require("socket")
        --for simplicity sake, write to a file
        file = io.open("./f.txt", "a+")
        io.output(file)
        io.write(str.. " from the evil function \r")
        io.close(file)
    end
}





print(verifier.isValid(str1)) -- verifying
verifier.evilFunc(str1)


--- Part 2, sandbox
-- 2.1 lua native sand box
    -- Define a safe environment
local sandbox_env = {
    print = print,
    tonumber = tonumber,
    tostring = tostring,
    math = {
        abs = math.abs,
        sqrt = math.sqrt,
        floor = math.floor
    },
    table = {
        insert = table.insert,
        remove = table.remove
    },
    _G = nil  -- Disable access to the global environment
}

-- Remove dangerous libraries
sandbox_env.os = nil
sandbox_env.io = nil
sandbox_env.debug = nil
sandbox_env.package = nil

-- User-provided code to be executed in the sandbox


local safe_code = [[
    print("This is a safe sandbox!")
    print(math.sqrt(49))
]]

local risky_code = [[
        file = io.open("./f.txt", "a+")
        io.output(file)
        io.write("love from risky code\r")
        io.close(file)
]]

-- Load the user code into a sandbox
local boxedSafe, err1 = load(safe_code, "sandbox", "t", sandbox_env)
local boxedRisky, err2 = load(risky_code, "sandbox", "t", sandbox_env)

boxedSafe()
--boxedRisky()


--- part3 ---- ocap

--- do something before falling back

wrappedV2 = {}

function connector(table, key, fallback)
    print("Do something before connecting")
    return fallback[key]
end

mt = {__index = function(tbl, key) return connector(tbl, key, verifier) end}
setmetatable(wrappedV2, mt)
print(wrappedV2.evilFunc("2355 pure evil stuff"))


print("--- Set up _ENV")
wrappedV = ocap.altNew(verifier)

--print(ocap.altReceiveCap(wrappedV, "io", io))
--os = nil
--print(verifier)
--print(wrappedV.goodFunc(49))
print(wrappedV)
rt1, rt2 = wrappedV.adsfadsf
print("rt of connector", rt1, rt2)
print("verifier", verifier)
print(wrappedV.goodFunc)
print(verifier.goodFunc(81))

---- alright, connector is set, now it's only env
---- Goal: creating temp env for temp execution
---- give caps to caped Obj. remove from global, run stuff
--- but shit, for fucks sake, now it only pick the func from fall back table, without actually executing it
--- gonna design different cases? if it's just picking something from table

--print("***Before setup***")
--print("_ENV", _ENV)
--print("_G", _G)
--
--setUpEnvRt = ocap.setUpEnv(wrappedV)
---- let's say it return the _env it uses internally, it should have the valid os
---- I can mess with _ENV however I like, coz I can use _G to restore it
---- or add it, then delete it
---- add os back to _ENV


--
--print("***After setup***")
--print(setUpEnvRt)
--print("_ENV", _ENV)
--print("_G", _G)
--printTable(_G)
--print(print)
--print(setUpEnvRt) -- yes, now it sets up the local _ENV without affecting the global one

local somethingShouldNotBeInG = "not in G"
--print(_ENV.somethingShouldNotBeInG)

--print("The _ENV")
--printTable(_ENV)
--
--print("The _G")
--printTable(_ENV._G)

print("End of Env setup")

