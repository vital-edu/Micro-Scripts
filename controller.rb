require 'serialport'
require 'net/http'

myserial = SerialPort.new('/dev/ttyACM0')
while(1)
  myserial.write("M")
  while(myserial.read == "")
  end
  Net::HTTP.get('0.0.0.0', '/medicines/decrease_amount/3', 3000)
end
