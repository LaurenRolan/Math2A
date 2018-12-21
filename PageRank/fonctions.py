import numpy as np
from numpy import matlib as ml
import matplotlib as mpl


def create_mini_net_A():
    A = np.matrix([[0, 1/3., 1/3., 1/3.],
                   [0, 0, 1/2., 1/2.],
                   [1, 0, 0, 0],
                   [1/2., 0, 1/2., 0]]).transpose()
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



A = create_mini_net_A()
M = calculateM(A, 0.15)

gamma = float (1) / M.shape[0]
beta, mu = get_constants(A, gamma)
print("With mu=" + str(mu) + " and beta=" + str(beta))

x2 = np.matrix([[0.2], [0.2], [0.285], [0.285], [0.285]])
x_etoile = np.matrix([[0.368], [0.142], [0.288], [0.202]])
x_current = get_random_x(M.shape[0])
DF_x_etoile = DF(x_etoile, M, gamma)

points = []

print("x(0) : " + str(x_current))
print("x*   : " + str(x_etoile))
print("DF(0): " + str(DF_x_etoile))
for i in range(100):
    x_old = x_current
    DF_x_current = DF(x_current, M, gamma)
    x_current = x_current - mu * DF_x_current
    points.append(np.linalg.norm(x_current, x_old))
print(x_current)