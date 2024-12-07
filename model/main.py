import sys
import time
import numpy as np
from agent import Agent
import torch
import pyautogui
import pywinctl
import cv2 as cv
import subprocess
from enum import Enum
from multiprocessing import shared_memory

game_window_x = 0
game_window_y = 0
game_window_width = 1000
game_window_height = 1000

def getGameFrame() -> np.array:
    screenshot = pyautogui.screenshot(region=(game_window_x, game_window_y, game_window_width, game_window_height))
    grayscale = cv.cvtColor(np.array(screenshot), cv.COLOR_RGB2GRAY)
    resize = cv.resize(grayscale, (600, 600))
    extra_dim = np.expand_dims(resize, axis=0)
    return extra_dim

def dispatchAction(action_code: int):
    if action_code < 0 or action_code > 2:
        raise ValueError("Invalid action code")
    
    if action_code == 0:
        pyautogui.press("space")
    elif action_code == 1:
        pyautogui.press("f")

def main():
    sh_memory = shared_memory.SharedMemory(name="rl_game1")
    shm_array = np.ndarray((1,), dtype=np.int32, buffer=sh_memory.buf)
    device = torch.device('cuda' if torch.cuda.is_available() else 'cpu')
    game_window = pywinctl.getWindowsWithTitle("RL-Game")[0].rect
    game_window_x, game_window_y, game_window_width, game_window_height = game_window
    game_window_width -= game_window_x
    game_window_height -= game_window_y

    total_plays = 100
    batch_size = 16
    input_format = torch.rand(size=(1, 600, 600))
    agent = Agent(gamma=0.9, epsilon=1.0, actions=3, input_format=input_format, batch_size=batch_size, device=device)

    for i in range(total_plays):
        
        frame = getGameFrame()
        reward = 0
        score = 0
        while reward != -25:
            action = agent.chose_action(frame)
            dispatchAction(action)
            next_frame = getGameFrame()
            reward = shm_array[0]
            shm_array[0] = 0
            agent.store_transition(frame, action, reward, next_frame)

            frame = next_frame
            score += 1

        if agent.mem_ctr > batch_size:
            agent.learn()
        
        print(f"Current play : {i} Score : {score}")
        pyautogui.press("enter")

    agent.save()
    sh_memory.close() 

if __name__ == "__main__":
    main()