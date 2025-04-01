from func_timeout import func_timeout, FunctionTimedOut
import time
import ast

# class ReadLine:
#     def __init__(self, s):
#         self.buf = bytearray()
#         self.s = s
    
#     def readline(self):
#         i = self.buf.find(b'\n')
#         if i >= 0:
#             r = self.buf[:i+1]
#             self.buf = self.buf[i+1:]
#             return r
#         while True:
#             i = max(1, min(2048, self.s.in_waiting))
#             data = self.s.read(i)
#             i = data.find(b"\n")
#             if i >= 0:
#                 r = self.buf + data[:i+1]
#                 self.buf[0:] = data[i+1:]
#                 return r
#             else:
#                 self.buf.extend(data)

def input_timeout(timeout, *args):
    try:
        return func_timeout(timeout, input, args=args)
    except FunctionTimedOut:
        pass

def read_safe(ser, timeoutval = 0.1):
    try:
        line = func_timeout(timeoutval, ser.readline)
        # print(line)
        return(line)
    except FunctionTimedOut:
        # print("Exception: No line to read")
        return(None)

def send_message(message, ser):
    ser.write((message + "\n").encode())
    time.sleep(0.2)

def clear_output(ser):
    data = 1
    while data is not None:
        data = read_safe(ser)

def interpret(byteval, divider=":"):
    if byteval is None:
        return
    strval = byteval.decode("utf-8")
    messages = strval.split("\r")[0].split(divider)
    try:
        msgtype = messages[1]
        if msgtype == 'msg':
            msg = messages[2]
            print(msg)
            return msg, None
        elif msgtype == 'msgvar':
            varname = messages[2]
            val = messages[3]
            print("{}, {}".format(varname,val))
            return varname, val
        elif msgtype == '1Darr':
            varname = messages[2]
            array = ast.literal_eval(messages[3])
            # print("{}, {}".format(varname,len(array)))
            return varname, array
    except: 
        print("exception:")
        print(messages)