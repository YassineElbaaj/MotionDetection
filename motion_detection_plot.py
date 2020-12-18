import numpy as np
import matplotlib.pyplot as plt

output_file = open("differences.txt", "r")
X_axis = []

for line in output_file:
    X_axis.append(int(line))
    

np_x_axis = np.array(X_axis)
np_y_axis = np.arange(1, len(X_axis)+1)

plt.plot(np_y_axis, np_x_axis)

plt.xlabel('Frames count')
plt.ylabel('Difference')

plt.show()
