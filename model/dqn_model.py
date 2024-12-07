import torch.nn as nn
import torch

class Model(nn.Module):
    
    def __init__(self, input_format: torch.Tensor, total_actions: int, learning_rate: float):

        super(Model, self).__init__()
        self.loss_func = torch.nn.MSELoss()
        self.leaky_relu = nn.LeakyReLU(negative_slope=0.1)

        self.conv1 = nn.Conv2d(in_channels=input_format.size(0), out_channels=10, kernel_size=3, padding=1)
        self.pool1 = nn.MaxPool2d(2, 2)
        self.conv2 = nn.Conv2d(in_channels=10, out_channels=20, kernel_size=3, padding=1)
        self.pool2 = nn.MaxPool2d(2, 2)
        self.conv3 = nn.Conv2d(in_channels=20, out_channels=40, kernel_size=3, padding=1)
        self.pool3 = nn.MaxPool2d(2, 2)

        self.fc1 = nn.Linear(40 * (input_format.size(1)//2//2//2) * (input_format.size(2)//2//2//2), 256)
        self.fc2 = nn.Linear(256, 128)
        self.fc3 = nn.Linear(128, 64)
        self.fc4 = nn.Linear(64, 32)
        self.fc5 = nn.Linear(32, 16)
        self.output = nn.Linear(16, total_actions)

        self.optimizer = torch.optim.SGD(self.parameters(), lr=learning_rate)


    def forward(self, x):
        x = self.leaky_relu(self.conv1(x))
        x = self.pool1(x)
        x = self.leaky_relu(self.conv2(x))
        x = self.pool2(x)
        x = self.leaky_relu(self.conv3(x))
        x = self.pool3(x)

        x = x.view(x.size(0), -1) 
        x = self.leaky_relu(self.fc1(x))
        x = self.leaky_relu(self.fc2(x))
        x = self.leaky_relu(self.fc3(x))
        x = self.leaky_relu(self.fc4(x))
        x = self.leaky_relu(self.fc5(x))
        x = self.output(x)
        return x