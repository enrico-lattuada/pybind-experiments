import pytest
import numpy as np
import pybind_experiments as pe

# test sum

# initialize numbers
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


# test array sum

# initialize arrays
arr1 = np.random.random(3)
arr2 = np.random.random(3)


@pytest.fixture
def baseline_arrays():
    # compute baseline
    return arr1 + arr2


def test_sum_arrays(baseline_arrays):
    # check that the result is correct
    assert np.isclose(pe.add_arrays(arr1, arr2), baseline_arrays).all()


def test_sum_arrays_cpp(baseline_arrays):
    # check that the result is correct
    assert np.isclose(pe.add_arrays_cpp(arr1, arr2), baseline_arrays).all()


def test_check_sum_arrays_type():
    # check that the type is correct
    assert pe.add_arrays_cpp(arr1, arr2).dtype == float
    assert (
        pe.add_arrays_cpp(arr1.astype(np.int64), arr2.astype(np.int64)).dtype
        == np.int64
    )
