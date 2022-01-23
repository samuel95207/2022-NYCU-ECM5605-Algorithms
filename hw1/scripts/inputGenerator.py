import numpy as np

k = 10000
n = 6400000
c = 12
t = 2400000
seed = 0
filename = "../inputs/test10.in"


np.random.seed(seed)

lines = [f"{k} {n} {c} {t}\n"]
for i in range(t):
    cost = np.random.uniform(1, 100)
    serviceNum = np.random.randint(1, 20)
    line = f"{cost} {serviceNum} {' '.join([str(num) for num in np.random.randint(0, n, size=serviceNum).tolist()])}\n"
    lines.append(line)

file = open(filename, 'w')
file.writelines(lines)
file.close()
