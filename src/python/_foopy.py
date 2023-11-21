import numpy as np


def add(a: float, b: float) -> float:
    """
    Add two numbers.

    Parameters
    ----------
    a : float
        The first number to be added.
    b : float
        The second number to be added.

    Returns
    -------
    float
        The sum of the two numbers.
    """
    return a + b


def add_arrays(a: np.ndarray, b: np.ndarray) -> np.ndarray:
    """
    Adds two arrays element-wise.

    Parameters
    ----------
    a : numpy.ndarray
        The first input array.
    b : numpy.ndarray
        The second input array.

    Returns
    -------
    numpy.ndarray
        The element-wise sum of the input arrays.
    """
    return a + b
