--define the gpio pins
--the flash button on the dev board
buttonpin=3
--the led on the dev board
ledpin=4

--the port number that presses will be broadcast on
rec_port_no=4950

--set wifi details
-- in Jules's house. 
--ssid="VM6339503"
--key="zdhzmcFCy2vr"
-- in the house
--ssid="SKYB2595"
--key="XNTRRWRQYB"
-- in the office
ssid="Mitchsoft"
key="davethecat"

--turn on the led
gpio.mode(ledpin,gpio.OUTPUT)
gpio.write(ledpin, gpio.LOW)

--tell the wifi modlule that it is a station
wifi.setmode(wifi.STATION)

--connect to wifi
wifi.sta.config(ssid,key)
wifi.sta.connect()
print("Connecting to "..ssid)

tmr.alarm(1, 1000, 1, function()
    if wifi.sta.getip() == nil then
        print("IP unavaiable, Waiting...")
    else
        tmr.stop(1)
        gpio.write(ledpin, gpio.HIGH)
        --print("ESP8266 mode is: " .. wifi.getmode())
        --print("The module MAC address is: " .. wifi.ap.getmac())
        print("Connected, my IP is "..wifi.sta.getip())
        udpSocket = net.createUDPSocket()
        udpSocket:listen(rec_port_no)
        udpSocket:on("receive", function(s, data, port, ip)
            recport, recip = udpSocket:getaddr()
            print(string.format("received '%s' from %s on port %d", data, ip, recport))
            if recport == rec_port_no then
                gpio.write(ledpin, gpio.LOW)
                print("It must be a doorbell button press!")
                gpio.write(ledpin, gpio.HIGH)
            end
        end)
    end
end)