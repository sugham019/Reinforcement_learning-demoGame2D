import torch
from dqn_model import Model
import numpy
import os

class Agent:
    
    def __init__(self, gamma: float, epsilon: float, actions: int, input_format: torch.Tensor,
            batch_size: int, save_file="model.pth", device: torch.device = torch.device("cpu"), learning_rate=0.001, max_mem_size=1000, eps_min=0.01, eps_dec=5e-4):

        if input_format.dim() != 3:
            raise ValueError("Expected dimension of input format 3D (channel, height, width)")
        
        self.gamma = gamma
        self.epsilon = epsilon
        self.eps_decay = eps_dec
        self.eps_min = eps_min
        self.learning_rate = learning_rate
        self.total_actions = actions
        self.actions = [i for i in range(actions)]
        self.device = device
        self.mem_size = max_mem_size
        self.mem_ctr = 0
        self.batch_size = batch_size
        self.save_file = save_file
        self.eval_model = Model(input_format, self.total_actions, learning_rate).to(device)
        
        if(os.path.exists(save_file)):
            self.__load()

        self.state_memory = numpy.zeros(shape=(self.mem_size, *input_format.size()), dtype=numpy.float32)
        self.next_state_memory = numpy.zeros(shape=(self.mem_size, *input_format.size()), dtype=numpy.float32)
        self.reward_memory = numpy.zeros(shape=self.mem_size, dtype=numpy.int32)
        self.action_memory = numpy.zeros(shape=self.mem_size, dtype=numpy.int32)


    def __load(self):
        agent_state = torch.load(self.save_file)
        self.eval_model.load_state_dict(agent_state['model_state_dict'])
        self.epsilon = agent_state['epsilon']
        print(self.epsilon)

    def save(self):
        agent_state = {
            'model_state_dict': self.eval_model.state_dict(),
            'epsilon': self.epsilon
        }
        torch.save(agent_state, self.save_file)

    def store_transition(self, state: numpy.ndarray, action: int, reward: int, next_state: numpy.ndarray):
        index = self.mem_ctr % self.mem_size
        self.state_memory[index] = state
        self.action_memory[index] = action
        self.next_state_memory[index] = next_state
        self.reward_memory[index] = reward
        self.mem_ctr += 1

    def chose_action(self, frame: numpy.ndarray):
        if numpy.random.random() > self.epsilon:
            state = torch.tensor([frame], dtype=torch.float).unsqueeze(0).to(self.device)
            action = self.eval_model(state)
            action = torch.argmax(action).item()
        else:
            action = numpy.random.choice(self.actions)
        
        return action
    
    def learn(self):
        self.eval_model.optimizer.zero_grad()
        max_mem = min(self.mem_ctr, self.mem_size)
        batch = numpy.random.choice(max_mem, self.batch_size, replace=False)
        batch_index = numpy.arange(self.batch_size, dtype=numpy.int32)

        state_batch = torch.tensor(self.state_memory[batch]).to(self.device)
        next_state_batch = torch.tensor(self.next_state_memory[batch]).to(self.device)

        reward_batch = torch.tensor(self.reward_memory[batch]).to(self.device)        
        action_batch = self.action_memory[batch]

        state_output = self.eval_model(state_batch)[batch_index, action_batch]
        next_state_ouptut = self.eval_model.forward(next_state_batch)
        target = reward_batch + self.gamma * torch.max(next_state_ouptut, dim=1)[0]

        loss = self.eval_model.loss_func(target, state_output).to(self.device)
        loss.backward()
        self.eval_model.optimizer.step()
        self.epsilon = self.epsilon - self.eps_decay if self.epsilon > self.eps_min else self.eps_min



        
