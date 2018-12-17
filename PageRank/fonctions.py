import numpy as np
import matplotlib as mpl


def create_mini_net_A():
    A = np.matrix([[0, 1/3., 1/3., 1/3.],
                   [0, 0, 1/2., 1/2.],
                   [1, 0, 0, 0],
                   [1/2., 0, 1/2., 0]])
    return A

def get_random_x(size):
    return np.random.randint(size-1, size=size).reshape((size, 1))

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

def hessiene(x, M, gamma):
    I = np.ones((M.shape[0], M.shape[0]))
    hessiene = np.transpose(M - I)
    hessiene = hessiene * (M - I) * x
    hessiene += gamma * M.shape[0] * x
    return np.transpose(hessiene)

def F(x, M, gamma):
    ''' inner_x = 1/2 ||Mx - x||
        inner_y = gamma/2 (sum(x) -1)^2'''
    inner_X = 0.5 * np.linalg.norm( M * x - x )
    inner_Y = gamma * 0.5 * (np.sum(x) - 1) ** 2
    return np.round(inner_X + inner_Y, 5)

def DF(x, M, gamma):
    delta_x = 1e-10 * x
    DF = F(x, M, gamma) + hessiene(x, M, gamma) * delta_x
    return np.round(DF, 5)

def get_score_vector(M):
    n = M.shape[0]
    v, w = np.linalg.eig(M)
    i = np.where(np.abs(v-1) < 1e-5)
    score = w[:, i].reshape((n, 1))
    score /= score.sum()
    return score.real

A = create_mini_net_A()
M = calculateM(A, 0.15)
gamma = float (1) / M.shape[0]
beta, mu = get_constants(A, gamma)
print("With mu=" + str(mu) + " and beta=" + str(beta))
x_etoile = get_score_vector(M)
x_random = get_random_x(M.shape[0])
DF_x_etoile = DF(x_etoile, M, gamma)
diff = 1
print("x(0) : " + str(x_random))
print("x*   : " + str(x_etoile))
print("DF(0): " + str(DF_x_etoile))
while(diff > 1e-10):
    DF_x_random = DF(x_random, M, gamma)
    print("DF(x): " + str(DF_x_random))
    x_random = x_random - mu * DF(x_random, M, gamma)
    print("x(n+1): " + str(x_random))
    diff = DF(x_random, M, gamma)