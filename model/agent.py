import torch
from dqn_model import Model
import numpy

class Agent:
    
    def __init__(self, gamma: float, epsilon: float, learning_rate: float, actions: int, input_format: torch.Tensor,
            batch_size: int, max_mem_size=10000, eps_min=0.01, eps_dec=5e-4, device: torch.device = torch.device("cpu")):
            
        self.gamma = gamma
        self.epsilon = epsilon
        self.eps_decay = eps_dec
        self.eps_min = eps_min
        self.learning_rate = learning_rate

        self.total_actions = len(actions)
        self.actions = [i for i in range(actions)]
        self.device = device
        self.mem_size = max_mem_size
        self.mem_ctr = 0
        self.batch_size = batch_size
        self.eval_model = Model(input_format, self.total_actions).to(device)

        self.state_memory = numpy.zeros(shape=(self.mem_size, input_format.size(1), input_format.size(2)), dtype=numpy.float32)
        self.next_state_memory = numpy.zeros(shape=(self.mem_size, input_format.size(1), input_format.size(2)), dtype=numpy.float32)
        self.reward_memory = numpy.zeros(shape=self.mem_size, dtype=numpy.float32)
        self.action_memory = numpy.zeros(shape=self.mem_size, dtype=numpy.int32)

    def __store_transition(self, state, action, reward, next_state, is_terminal):
        index = self.mem_ctr & self.mem_size
        self.state_memory[index] = state
        self.action_memory[index] = action
        self.next_state_memory[index] = next_state
        self.reward_memory[index] = reward
        self.mem_ctr += 1

    def __chose_action(self, observation):
        if numpy.random.random() > self.epsilon:
            state = torch.tensor([observation]).to(self.device)
            action = self.eval_model.forward(state)
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
        next_state_batch = torch.tensor(self.next_state_memory).to(self.device)
        reward_batch = torch.tensor(self.reward_memory[batch]).to(self.device)
        
        action_batch = self.action_memory[batch]

        state_output = self.eval_model.forward(state_batch)[batch_index, action_batch]
        next_state_ouptut = self.eval_model.forward(next_state_batch)
        target = reward_batch + self.gamma * torch.max(next_state_ouptut, dim=1)[0]

        loss = self.eval_model.loss_func(target, state_output).to(self.device)
        loss.backward()
        self.eval_model.optimizer.step()
        self.epsilon = self.epsilon - self.eps_decay if self.epsilon > self.eps_min else self.eps_min



        
