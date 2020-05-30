import d3dshot
from aura_tools import Aura, image_to_signal, down_up_sample
import time


ip = "192.168.0.230"
port = 3000
length = 174
fps = 75
brightness = 255
offset = 144
scale = 0.02


def run():
    d = d3dshot.create(capture_output="numpy_float")
    d.capture(region=(0, 0, 1920, 1080))
    s = Aura(ip, port, length, brightness, offset)
    s.send()
    time.sleep(1)
    while 1:
        # last_time = time.time()
        s.set(image_to_signal(down_up_sample(d.get_latest_frame(), scale)))
        s.send()
        time.sleep(1 / fps)
        # print("fps: {}".format(1 / (time.time() - last_time)))


if __name__ == "__main__":
    run()
