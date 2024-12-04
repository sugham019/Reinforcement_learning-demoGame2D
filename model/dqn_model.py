import torch.nn as nn
import torch

class Model(nn.Module):
    
    def __init__(self, input_format: torch.Tensor, total_actions: int, learning_rate: float):

        super(Model, self).__init__()
        self.loss_func = torch.nn.MSELoss()
        self.leaky_relu = nn.LeakyReLU(negative_slope=0.01)

        self.conv1 = nn.Conv2d(in_channels=input_format.size(0), out_channels=5, kernel_size=3, padding=1)
        self.pool1 = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(in_channels=5, out_channels=5, kernel_size=3, padding=1)
        self.fc1 = nn.Linear(5 * (input_format.size(1)//2) * (input_format.size(2)//2), 32)
        self.fc2 = nn.Linear(32, 16)
        self.output = nn.Linear(16, total_actions)

        self.optimizer = torch.optim.SGD(self.parameters(), lr=learning_rate)


    def forward(self, x):
        x = self.leaky_relu(self.conv1(x))
        x = self.pool1(x)
        x = self.leaky_relu(self.conv2(x))
        
        x = x.view(x.size(0), -1) 
        x = self.leaky_relu(self.fc1(x))
        x = self.leaky_relu(self.fc2(x))
        x = self.output(x)
        return x