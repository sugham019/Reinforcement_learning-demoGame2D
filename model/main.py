import time
import numpy as np
import torch
from multiprocessing import shared_memory

def main():
    # sh_memory = shared_memory.SharedMemory(name="rl_game")
    # shm_array = np.ndarray((2,), dtype=np.int32, buffer=sh_memory.buf)

    # print(f"The value is :{shm_array[0]}")

    # shm_array[1] = 1
    # print(f"Sending response : {shm_array[1]}")

    # sh_memory.close()
    # while(True):
    #     pyautogui.press("space")

    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')

    pass


if __name__ == "__main__":
    main()