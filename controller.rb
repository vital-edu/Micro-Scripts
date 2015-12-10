require 'serialport'
require 'net/http'

myserial = SerialPort.new('/dev/ttyACM1')
while(1)
  myserial.write("2")
  while(myserial.read == "")
  end
  Net::HTTP.get('0.0.0.0', '/medicines/decrease_amount/5', 3000)
end
