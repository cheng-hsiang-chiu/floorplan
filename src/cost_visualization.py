import matplotlib
import matplotlib.pyplot as plt
import numpy as np
import json
import sys

jsonfile = sys.argv[1]


with open(jsonfile) as f:
  data = json.load(f)


#print(data['cost'])

cost = data['cost']

x = np.arange(0, len(cost), 1)
y = cost

fig, ax = plt.subplots()
#ax.plot(x, y)
ax.semilogx(x, y)

fig.savefig("test.png")
plt.show()

'''
# Data for plotting
t = np.arange(0.0, 2.0, 0.01)
s = 1 + np.sin(2 * np.pi * t)

fig, ax = plt.subplots()
ax.plot(t, s)

ax.set(xlabel='time (s)', ylabel='voltage (mV)',
       title='About as simple as it gets, folks')
ax.grid()

fig.savefig("test.png")
plt.show()

'''
