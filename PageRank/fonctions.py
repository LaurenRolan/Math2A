import numpy as np
import matplotlib as mpl


def create_mini_net_A():
    A = np.matrix([[0, 1/3., 1/3., 1/3.],
                   [0, 0, 1/2., 1/2.],
                   [1, 0, 0, 0],
                   [1/2., 0, 1/2., 0]])
    return A

def get_random_x(size):
    return np.random.randint(size-1, size=size)

def calculateM(A, alpha):
    M = A * (1 - alpha)
    S = np.ones((M.shape[0], M.shape[0]))
    S = S * (alpha * 1 / M.shape[0])
    M = M + S
    return M

def get_constants(A, gamma):
    beta = 4 + A.shape[0] * gamma
    mu = 1.99 / beta
    return beta, mu

def hessiene(M, gamma):
    I = np.ones((M.shape[0], M.shape[0]))
    hessiene = np.transpose(M - I)
    hessiene = hessiene * (M - I)
    hessiene += gamma * M.shape[0]
    return hessiene

def F(x, M, gamma):
    I = np.ones((M.shape[0], M.shape[0]))
    X = np.subtract(M, I)
    X = np.multiply(X, x)
    inner_X = np.sum(X*X)
    inner_X = (inner_X ** 2) / 2

    inner_Y = np.sum(x) - 1
    inner_Y = (inner_Y ** 2) / 2
    inner_Y *= gamma

    return inner_X + inner_Y

def get_score_vector(M):
    n = M.shape[0]
    v, w = np.linalg.eig(M)
    i = np.where(np.abs(v-1) < 1e-5)
    score = w[:, i].reshape((n, 1))
    score /= score.sum()
    return np.transpose(score.real) #added transpose

A = create_mini_net_A()
M = calculateM(A, 0.15)
gamma = float (1) / M.shape[0]
mu, beta = get_constants(A, gamma)
x_etoile = get_score_vector(M)
x_random = get_random_x(M.shape[0])
print("x*   : " + str(x_etoile))
print("x    : " + str(x_random))
print(hessiene(M, gamma))
print("F(x*):" + str(F(x_etoile, M, gamma)))
print("F(x) :" + str(F(x_random, M, gamma)))
