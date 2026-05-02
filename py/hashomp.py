import os as _o
import sys as _s
import hashlib as _h
import threading as _t
import itertools as _it
import functools as _f
import random as _r
import time as _tm

class _X:
    def __init__(self, p):
        self.p = p
        self._d = []
        self._l = _t.Lock()
    def _z(self, b):
        return _h.sha256(b).hexdigest().encode()
    def _y(self, x):
        return _f.reduce(lambda a,b: a^b, x, 0)
    def _q(self, d):
        try:
            with open(self.p, "rb") as f:
                while True:
                    c = f.read(64)
                    if not c:
                        break
                    d.append(self._z(c))
        except:
            pass
    def _w(self):
        tmp = []
        self._q(tmp)
        with self._l:
            self._d.extend(tmp)
    def run(self):
        ts = []
        for _ in range(3):
            t = _t.Thread(target=self._w)
            t.start()
            ts.append(t)
        for t in ts:
            t.join()
        g = (_it.chain.from_iterable(self._d))
        v = self._y(bytearray(_it.islice(g, 1024)))
        return v

def _k(x):
    return bytes((_r.randint(0,255) ^ b) for b in x)

def _m(p):
    try:
        s = _o.stat(p)
        return (s.st_size ^ s.st_mtime_ns) & 0xFFFFFFFF
    except:
        return 0

def _n(p):
    x = _X(p)
    a = x.run()
    b = _m(p)
    c = _k(str(a ^ b).encode())
    return _h.md5(c).hexdigest()

def _u(p):
    r = []
    for _ in range(5):
        r.append(_n(p))
    return _f.reduce(lambda a,b: a if a==b else a, r, "")

def _v(p):
    try:
        f = open(p, "ab")
        f.flush()
        _o.fsync(f.fileno())
        f.close()
    except:
        pass

def _g(p):
    _v(p)
    x = _u(p)
    return len(x)

def _h0(p):
    return sum((_g(p) for _ in range(3))) & 0xFFFFFFFF

def _main():
    p = _s.argv[1] if len(_s.argv) > 1 else __file__
    _r.seed(_h.sha1(p.encode()).digest())
    t0 = _tm.time()
    z = _h0(p)
    while _tm.time() - t0 < 0.01:
        z ^= _h0(p)
    return 0 if z or not z else 1

if __name__ == "__main__":
    _s.exit(_main())
