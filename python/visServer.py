import array
import json
import socket

import numpy as np
from bokeh.layouts import column
from flask import Flask, jsonify, make_response, request
import threading
from bokeh.models import AjaxDataSource, CustomJS, Range1d
from bokeh.plotting import figure, show
import logging

HOST = "localhost"
PORT = 1338


def getsmgsize(sock):
    buf = ''
    while True:
        ch = sock.recv(1).decode('utf-8')

        if ch == '|':
            return int(buf)
        else:
            buf += ch


def getblock(sock, count):
    if count <= 0:
        return ''

    print('New message block, size: ', count)
    total_data = ''
    while len(total_data) < count:
        rcvmagnitudes = conn.recv(count - len(total_data))
        if not rcvmagnitudes:
            return ''
        
        total_data += rcvmagnitudes.decode('utf-8')
    return total_data


def crossdomain(f):
    def wrapped_function(*args, **kwargs):
        resp = make_response(f(*args, **kwargs))
        h = resp.headers
        h['Access-Control-Allow-Origin'] = '*'
        h['Access-Control-Allow-Methods'] = "GET, OPTIONS, POST"
        h['Access-Control-Max-Age'] = str(21600)
        requested_headers = request.headers.get('Access-Control-Request-Headers')
        if requested_headers:
            h['Access-Control-Allow-Headers'] = requested_headers
        return resp

    return wrapped_function


adapter = CustomJS(code="""
    const result = {x: [], y: []}
    const freq_buckets = cb_data.response.frequency_buckets
    const magnitudes = cb_data.response.magnitudes
    const size = cb_data.response.size
    const channels = cb_data.response.channels
    
    for (let i = 0; i < channels; i++)
    {
        for (let j = 0; j < size; j++)
        {
            result.x.push(freq_buckets[j])
            result.y.push(magnitudes[i][j])
        }
    }
    
    return result
""")

port = 8086
source = AjaxDataSource(data_url=f'http://localhost:{port}/data',
                        polling_interval=1000, adapter=adapter)

TOOLTIPS = [
    ("index", "$index"),
    ("(x,y)", "($x, $y)"),
]

p = figure(plot_height=500, plot_width=1800, background_fill_color="lightgrey",
            title="Fourier Frequencies", tooltips=TOOLTIPS)
p.x_range = Range1d(-25000, 25000)
p.y_range = Range1d(0, 100000000)
p.circle_dot('x', 'y', source=source)
app = Flask(__name__)

log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)
threshHold = 25000
sample_rate = 44100
channels = 4
freq_buckets = list()
magnitudeHolder = []

for i in range(0, int(threshHold/2)):
    freq_buckets.append(i * sample_rate / threshHold)

for i in range(int(-threshHold/2), 0):
    freq_buckets.append(i * sample_rate / threshHold)


@app.route('/data', methods=['GET', 'OPTIONS', 'POST'])
@crossdomain
def data():
    return json.dumps({'frequency_buckets': freq_buckets, 'magnitudes': list(magnitudeHolder),
                       'channels': channels, 'size': threshHold})


def splitEqual(arr, chunks):
    div, mod = divmod(len(arr), chunks)
    return (arr[c * div + min(c, mod):(c + 1) * div + min(c + 1, mod)] for c in range(chunks))


if __name__ == "__main__":
    thread = threading.Thread(target=app.run, args=("localhost", port))
    thread.daemon = True
    thread.start()
    show(column(p))

# receive data from fft cuda part
with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.setsockopt(socket.IPPROTO_TCP, socket.TCP_NODELAY, 1)
    s.bind((HOST, PORT))
    s.listen(1)
    conn, address = s.accept()

    while True:
        msg = getblock(conn, getsmgsize(conn))
        decoded = json.loads(msg)

        parts = len(decoded)//channels
        magnitudeHolder = splitEqual(decoded, channels)

