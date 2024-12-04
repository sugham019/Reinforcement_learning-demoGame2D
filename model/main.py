import sys
import time
import numpy as np
from agent import Agent
import torch
import pyautogui
import cv2 as cv
import subprocess
from enum import Enum
from multiprocessing import shared_memory

class Action(Enum):
    JUMP = "space"
    SHOOT = "f"

def getGameFrame() -> np.array:
    screenshot = pyautogui.screenshot()
    grayscale = cv.cvtColor(np.array(screenshot), cv.COLOR_RGB2GRAY)
    resize = cv.resize(grayscale, (200, 200))
    extra_dim = np.expand_dims(resize, axis=0)
    return resize

def dispatchAction(action: Action):
    pyautogui.press(action.value)

def getActinByCode(actionCode: int) -> Action:
    if actionCode < 0 or actionCode > 1:
        raise ValueError("Invalid action code")
    
    return Action.JUMP if actionCode == 0 else Action.SHOOT

def main():
    sh_memory = shared_memory.SharedMemory(name="rl_game17")
    shm_array = np.ndarray((1,), dtype=np.int32, buffer=sh_memory.buf)
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    total_plays = 20
    batch_size = 8
    input_format = torch.rand(size=(1, 200, 200))

    agent = Agent(gamma=0.9, epsilon=1.0, actions=2, input_format=input_format, batch_size=batch_size, device=device)
    for i in range(total_plays):
        frame = getGameFrame()
        reward = 0
        while reward != -5:
            action = agent.chose_action(frame)
            dispatchAction(getActinByCode(action))
            next_frame = getGameFrame()
            reward = shm_array[0]
            shm_array[0] = 0

            agent.store_transition(frame, action, reward, next_frame)
            if agent.mem_ctr > batch_size:
                agent.learn()

            frame = next_frame

        print(f"Current play : {i}")

    agent.save()
    sh_memory.close() 

if __name__ == "__main__":
    main()