import tinyflow as tf
import numpy as np

def check_ewise(ufunc):
    x = tf.placeholder(tf.float32)
    y = tf.placeholder(tf.float32)
    z = ufunc(x, y)
    ax = np.ones((2, 3))
    ay = np.ones((2, 3)) * 4
    sess = tf.Session()
    az = sess.run(z, feed_dict={x:ax, y:ay})
    np.testing.assert_almost_equal(az, ufunc(ax, ay))

def test_ewise():
    check_ewise(lambda x, y: x+y)
    check_ewise(lambda x, y: x*y)

def test_softmax():
    x = tf.placeholder(tf.float32)
    y = tf.nn.softmax(x)
    ax = np.ones((2, 4))
    sess = tf.Session()
    ay = sess.run(y, feed_dict={x:ax})
    np.testing.assert_almost_equal(
        ay, ax / np.sum(ax, axis=1, keepdims=True))

def test_bias_add():
    x = tf.placeholder(tf.float32)
    b = tf.placeholder(tf.float32)
    y = tf.nn.bias_add(x, b)
    ax = np.random.uniform(size=(2, 3))
    ab = np.random.uniform(size=(3, ))
    sess = tf.Session()
    ay = sess.run(y, feed_dict={x:ax, b:ab})
    np.testing.assert_almost_equal(
        ay, ax + ab)


if __name__ == "__main__":
    pass