import pytest
import pybind_experiments as pe

# initialize arrays
a = 2.3
b = 4.2


@pytest.fixture
def baseline():
    # compute baseline
    return a + b


def test_sum(baseline):
    # check that the result is correct
    assert pe.add(a, b) == baseline


def test_sum_cpp(baseline):
    # check that the result is correct
    assert pe.add_cpp(a, b) == baseline
