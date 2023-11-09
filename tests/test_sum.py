import pytest
import numpy as np
import pybind_experiments as pe

# initialize arrays
np.random.seed(42)
a = np.random.random(3)
b = np.random.random(3)


@pytest.fixture
def baseline():
    # compute baseline
    return pe.add_arrays(a, b)


def test_sum_python(baseline):
    # check that the result is correct
    assert np.isclose(a + b, baseline).all()
