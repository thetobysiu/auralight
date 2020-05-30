import socket
import numpy as np
import torch
import torch.nn.functional as F


def down_up_sample(image_matrix, scale, size=(32, 59)):
    """accept a matrix (H, W, C) and returns a matrix (H, W, C)"""
    # B = Batch size, C = Channel, H = Height, W = Width
    # nn.functional.interpolate accept tensor of (B, C, H, W) where B is the batch size, 1 in this case
    return np.rollaxis(  # 7. roll the dimensions back to (H, W, C)
        F.interpolate(  # 5. upsample
            F.interpolate(  # 4. downsample
                torch.from_numpy(  # 3. create tensor from numpy
                    np.expand_dims(  # 2. add a dimension at the beginning as the batch size
                        np.rollaxis(image_matrix, 2),  # 1. roll the dimensions of input matrix (H, W, C) -> (C, H, W)
                        axis=0)  # 2a. insert the Batch dimension at axis 0
                ),
                scale_factor=scale,  # 4a. downsample by a scale factor, the lower the smoother
                mode='area'  # 4b. interpolation mode, area with anti-aliasing
            ),
            size=size,  # 5a. upsample back to a fixed size
            mode='bilinear',  # 5b. Bilinear to interpolate between pixels
            align_corners=False  # 5c. disable align corners
        )[0].cpu().numpy(),  # 6. Copy back the tensor(use [0] to get rid of batch dimension) to cpu memory as numpy
        0, 3  # 7a. shift the first dimension (Channel) by three
    )


def image_to_signal(image_matrix):
    """Extract the bottom, left, top and right border of an image matrix (H, W, C) and flatten into one dimension"""
    return np.concatenate(  # 1. Concatenate the bottom, left, top, right border of the image
        [
            image_matrix[-1, ::-1, :],  # bottom row, the last row and invert step ::-1
            image_matrix[-1:1:-1, 0, :],  # left column, exclude first and last row(included in top and bottom), invert
            np.resize(image_matrix[0], (55, 3)),  # top row, simple resize to 55
            image_matrix[1:-1, -1, :]  # right column, exclude first the last row in the last column
        ],
        axis=0  # concatenate by first axis
    ).flatten()  # 2. flatten


class Aura:
    def __init__(self, ip, port, length, brightness, offset):
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)  # UDP
        self.target = (ip, port)
        self.brightness = brightness
        self.offset = offset
        self.data = [0 for _ in range(length * 3)]

    def set(self, array):
        self.data = np.roll(  # 3. roll the array by offset
            np.multiply(  # 1. multiply the color channel array by brightness
                array,
                self.brightness
            ).astype('uint8'),  # 2. change to byte
            self.offset * 3  # 3a. each channel after flattening becomes 3 elements, therefore multiply by 3
        )

    def send(self):
        self.sock.sendto(
            bytes(self.data),  # convert to byte representation
            self.target
        )
