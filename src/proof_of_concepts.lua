---
--- Generated by Luanalysis
--- Created by swift.
--- DateTime: 10/22/24 14:54
---
--- Eventually, comming back to struct & reference based lua
--- coz other way is just not gonna work


require("verifier")

str1 = "1234 5678 2234 5678"
str2 = "1234 5678 2234 567"
str3 = "aaaa"

print(verifier.isValid(str1))
print(verifier.isValid(str3))

verifier.evilIsValid("To the file")

-----
obj = ocap.altNew(verifier)
obj2 = ocap.altNew(verifier)
--
--printTable(obj)

obj.receiveCap("io", io)

io = nil

print(obj2.evilIsValid("from obj2, shouldn't be on file at all"))
print(obj.evilIsValid("from obj, have io access, should be on file"))


--- conclusion obj2, without IO, unable to perform