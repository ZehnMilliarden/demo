import numpy as np


def test_ndmin():
    a = np.array([1, 2, 3, 4, 5], ndmin=3)
    print(a)


if __name__ == '__main__':
    test_ndmin()
