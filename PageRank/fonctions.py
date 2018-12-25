import numpy as np
from numpy import matlib as ml
import matplotlib.pyplot as plt
import math

def print_graphics(points, logs, iterations):
    plt.plot([x for x in range(0, iterations)], points, 'ro')
    plt.plot([x for x in range(0, iterations)], logs, 'bo')
    plt.axis([0, iterations, logs[-1] + 0.2 * logs[-1], logs[0] + 0.2 * logs[0]])
    plt.show()

def create_mini_net_A():
    A = np.matrix([[0, 1/3., 1/3., 1/3.],
                   [0, 0, 1/2., 1/2.],
                   [1, 0, 0, 0],
                   [1/2., 0, 1/2., 0]]).transpose()
    return A

def create_net(size):
    A = np.zeros((size, size))
    for col in A:
        num_links = np.random.randint(0, size)
        for i in range(num_links):
            col[i] = 1 / num_links
        np.random.shuffle(col)
    A = A.transpose()
    return A

def get_random_x(size):
    return np.matrix(np.random.randint(size-1, size=size).reshape((size, 1)))

def calculateM(A, alpha):
    M = (1 - alpha) * A
    S = ml.ones((M.shape[0], M.shape[0]))
    S = (alpha / M.shape[0]) * S
    M = M + S
    print("M" + str(M))
    return M

def get_constants(A, gamma):
    beta = 4 + A.shape[0] * gamma
    mu = 1.99 / beta
    return beta, mu

def F(x, M, gamma):
    ''' inner_x = 1/2 ||Mx - x||
        inner_y = gamma/2 (sum(x) -1)^2'''
    inner_X = 0.5 * np.linalg.norm( M * x - x )
    inner_Y = gamma * 0.5 * (np.sum(x) - 1) ** 2
    return np.round(inner_X + inner_Y, 5)

def DF(x, M, gamma):
    I = np.identity(M.shape[0])
    e = ml.ones((M.shape[0], 1))

    mul = np.transpose(M - I) * (M - I)
    scalar = np.sum(x) - 1
    DF = mul * x + gamma * e * scalar
    return DF


def get_score_vector(M):
    v, w = np.linalg.eig(M)
    v = np.real(v)
    (m,) = np.shape(v)
    for i in range(m):
        if abs(v[i]-1) < 1e-3:
            vi = w[:, i]
    vi = np.real(vi)
    vi = vi/np.linalg.norm(vi, 1)
    vi = np.round(vi, 3)
    return(vi)


A = create_net(20)
M = calculateM(A, 0.15)
gamma = float (1) / M.shape[0]
beta, mu = get_constants(A, gamma)
print("With mu=" + str(mu) + " and beta=" + str(beta))

x_etoile = np.matrix([[0.368], [0.142], [0.288], [0.202]])
x_etoile_calc = get_score_vector(M)
print(x_etoile_calc)
x_current = get_random_x(M.shape[0])
DF_x_etoile = DF(x_etoile, M, gamma)

points = []
logs = []
iterations = 200

print("x(0) : " + str(np.round(x_current, 3)))
print("x*   : " + str(np.round(x_etoile, 3)))
print("DF(0): " + str(np.round(DF_x_etoile, 3)))
for i in range(iterations):
    x_old = x_current
    DF_x_current = DF(x_current, M, gamma)
    x_current = x_current - mu * DF_x_current
    points.append(np.linalg.norm(x_current - x_old))
    logs.append(np.log(1 + np.linalg.norm(x_etoile - x_current)))

print_graphics(points, logs, iterations)
