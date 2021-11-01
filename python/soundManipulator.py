import math
import numpy as np
import scipy.io.wavfile as wavfile
from pycuda.compiler import SourceModule
import pycuda.driver as drv
import sounddevice as sd
import keyboard
from dataclasses import dataclass


@dataclass
class Position:
    rotation: int
    distance: float


# parse WAV file and copy signal into helper arrays
fs_rate, signal = wavfile.read("../2channelKnownFreqTest.wav")
num_channels = 2
original = signal.copy()
transitionHelper = signal.copy()
print("Using output device: ", sd.default.device)

# initialize CUDA context
drv.init()
print("GPUs available ", drv.Device.count())
device = drv.Device(0)
ctx = device.make_context()

# used globals
index = 0  # idx to track upcoming audio samples
block_size = 1024  # random picked power of 10
blockStream = False  # bool to block using currently manipulated stream
MAX_CUDA_KERNELS = 1024
p = Position(0, 0)


def callback(in_data, out_data, frames, time, status):
    global index, blockStream

    # print errors
    if status:
        print(status)

    # loop the audio signal
    if index > len(signal) - 1:
        index = 0

    block_increase = block_size

    # check for end of audio file and fill rest with zeroes to match blocksize
    if index + block_increase > len(signal) - 1:
        block_increase = len(signal) - index

    next_block = np.zeros_like(out_data)
    if blockStream:
        next_block[:block_increase] = transitionHelper[index:index + block_increase]
    else:
        next_block[:block_increase] = signal[index:index + block_increase]

    out_data[:] = next_block[:]
    index += block_increase


mod = SourceModule("""
    __global__ void manipulate(int *dest, int *src, int &rotation, float &distance, int &parts, int &numChannels)
    {
        const int idx = threadIdx.x;
        float modRot = rotation / 180.f;
        for (int i = idx * parts; i < idx * parts + parts; i+=numChannels)
        {
            for (int ch = 0; ch < numChannels; ch++)
            {
                int chIdx = i + ch;
                
                dest[chIdx] = (src[chIdx] - src[chIdx] * modRot + 
                    src[i + abs((chIdx-1) % numChannels)] * modRot) / numChannels;
                
                if (distance > 1 || distance < -1)
                    dest[chIdx] = dest[chIdx] / distance;
            }
        }
    }
""")

manipulate = mod.get_function("manipulate")
stream = sd.Stream(device=sd.default.device, samplerate=fs_rate, channels=num_channels,
                   callback=callback, blocksize=block_size, dtype='int16')

with stream:
    while True:
        if keyboard.is_pressed('left'):
            p.rotation = (p.rotation - 1) % 180
        elif keyboard.is_pressed('right'):
            p.rotation = (p.rotation + 1) % 180
        elif keyboard.is_pressed('up'):
            p.distance += 0.1
        elif keyboard.is_pressed('down'):
            p.distance -= 0.1
        else:
            continue

        print("calculate new positional data")
        print("p(", p.rotation, ",", p.distance, ")")
        transitionHelper = signal.copy()
        blockStream = True
        signal_input = np.array(original.flatten(), dtype=np.int32)
        dest = np.zeros_like(signal_input)
        parts = math.ceil(len(signal_input) / MAX_CUDA_KERNELS)

        manipulate(drv.Out(dest), drv.In(signal_input),
                   drv.In(np.int32(p.rotation)), drv.In(np.float32(p.distance)),
                   drv.In(np.int32(parts)), drv.In(np.int32(num_channels)),
                   block=(MAX_CUDA_KERNELS, 1, 1), grid=(1, 1))

        signal = np.array(dest.reshape(len(original), num_channels), dtype='int16')
        print(signal)
        print("manipulation done")
        blockStream = False
