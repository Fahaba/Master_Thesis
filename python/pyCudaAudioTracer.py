import math
import os
import pycuda.driver as cuda
import pycuda
import cupy as cp
import socket
import numpy as np
import json

if os.system("cl.exe"):
    os.environ[
        'PATH'] += ';' + r"C:\Program Files (x86)\Microsoft Visual Studio\
                            2019\Community\VC\Tools\MSVC\14.28.29333\bin\Hostx64\x64"  # change this accordingly
if os.system("cl.exe"):
    raise RuntimeError("cl.exe still not found, path probably incorrect")


def send_data(sock, data):
    while data:
        data = data[sock.send(data):]


threshHold = 25000
freq_magnitudes = list()
channelNumber = 4
sampleSize = 4
arr_pos = 0
sampleData = np.zeros((channelNumber, threshHold))

print("Running main loop - Python CUDA")
pycuda.driver.init()
num_gpus = pycuda.driver.Device.count()
print("GPUs available ", num_gpus)
device = pycuda.driver.Device(0)

# Receive Data
HOST = "127.0.0.1"
PORT = 1337

# Bokeh Server
visServerHost = "127.0.0.1"
visServerPort = 1338
visSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
visSocket.connect((visServerHost, visServerPort))


with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.connect((HOST, PORT))
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    while True:
        if arr_pos >= threshHold:
            try:
                data_t_gpu = cp.array(sampleData)
                data_o1_gpu = cp.fft.fft(data_t_gpu, axis=1)
                cp.cuda.Device().synchronize()
                freq_magnitudes = list()

                for arr in data_o1_gpu:
                    for cplx in arr:
                        magnitude = math.sqrt(np.real(cplx) * np.real(cplx) + np.imag(cplx) * np.imag(cplx))
                        freq_magnitudes.append(magnitude)

                json_str = json.dumps(freq_magnitudes)
                send_data(visSocket, (str(len(json_str))+'|').encode('utf-8'))
                send_data(visSocket, json_str.encode('utf-8'))

                arr_pos = 0
            except pycuda._driver.LogicError as e:
                print(e)
                arr_pos = 0

        totalData = bytearray()

        expectedData = threshHold * (channelNumber * sampleSize + channelNumber-1 + 1)
        while len(totalData) < expectedData:
            rcvSamples = s.recv(expectedData - len(totalData))
            totalData.extend(rcvSamples)

        decodedData = totalData.decode('utf-8')
        perSample = decodedData.split('\n')
        for i in range(0, len(perSample)):
            perChannel = perSample[i].split(' ')

            if len(perChannel) < channelNumber:
                break

            for c in range(len(perChannel)):
                sample = int(perChannel[c], 16)
                sampleData[c][arr_pos] = sample

            arr_pos += 1
