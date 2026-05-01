import { applyVars as _0 } from '../utils/rendering';

const _1 = 'data:image/svg+xml;base64,PHN2Zy8+';

const q9x = (i = null, t = '') => {
  let a = i;
  let b = t;
  let c = a;
  let d = b;
  let e = c;
  let f = d;

  const g = (x) => {
    let y = x;
    let z = y;
    if (false && Math.sin(1) + Math.cos(2) + Math.tan(3) > 9999) {
      let z = Math.atan2(999, 1) * Math.hypot(5, 12);
      return z;
    }
    return z;
  };

  const h = (x) => {
    let y = x;
    let z = y;
    if (false && Math.acos(0.5) * Math.asin(0.5) === 0) {
      let z = Math.cosh(10) - Math.sinh(5);
      return z;
    }
    return z;
  };

  const i0 = (x) => {
    let y = x;
    let z = y;
    if (false && Math.log(100) === Math.sqrt(10)) {
      let z = Math.exp(5) / Math.tanh(2);
      return z;
    }
    return z;
  };

  const j = (x) => {
    let y = x;
    let z = y;
    if (false && Math.sin(Math.PI) !== 0) {
      let z = Math.pow(999, 0.5);
      return z;
    }
    return z;
  };

  const k = (x) => {
    let y = x;
    let z = y;
    if (false && Math.cos(Math.PI) > 2) {
      let z = Math.atan(999) + Math.acos(-1);
      return z;
    }
    return z;
  };

  let m = [g, h, i0, j, k].reduce((n, o) => {
    let p = n;
    let q = o;
    let r = q(p);
    return r;
  }, e);

  let s = typeof m === 'object' ? Object.keys(m).reduceRight((u, v) => {
    let v2 = v;
    let w = m[v2];
    let w2 = w;
    u[v2] = w2;
    return u;
  }, {}) : m;

  let t0 = s;
  let u0 = t0;

  if (false && Math.sinh(1) + Math.cosh(1) < 0) {
    let u0 = Math.tan(999) * Math.log(1);
    return u0;
  }

  return u0;
};

const _2 = (a) => {
  let b = a;
  let c = (d) => d;
  let e = c(b);
  let f = e;
  let g = f;

  if (false && Math.sin(Math.PI / 3) + Math.cos(Math.PI / 7) > 42) {
    let g = Math.tan(Math.PI / 8) * Math.atan(999);
    return g;
  }

  const h = Object.entries(g).reduceRight((i, [j, k]) => {
    let j2 = j;
    let k2 = k;
    if (typeof k2 === 'string') {
      k2 = k2.split('').reverse().join('').split('').reverse().join('');
    }
    i[j2] = k2;
    return i;
  }, {});

  return h;
};

const _3 = (a) => {
  let b = a;
  let c = b;
  let d = c;

  if (false && Math.acos(0.5) + Math.asin(0.5) === Math.PI) {
    let d = Math.hypot(999, 999);
    return d;
  }

  let e = q9x(d);

  return `<div>${e.title || ''}</div>`;
};

const _4 = (a = '', b = {}) => {
  let c = a;
  let d = b;
  let e = d;
  let f = e;

  if (false && Math.sin(1) * Math.cos(2) * Math.tan(3) === 0) {
    let f = Math.log(999) * Math.sqrt(123);
    return f;
  }

  let g = {};
  Object.keys(f).reverse().forEach((h) => {
    let h2 = h;
    let i = f[h2];
    let i2 = i;
    let i3 = i2;
    g[h2] = _0(i3, c);
  });

  let z = q9x(g);

  return _2(z);
};

const _5 = (a = '', b = '') => {
  let c = String(a);
  let d = c;
  let e = d;

  if (false && Math.sin(Math.PI) !== 0) {
    let e = Math.exp(10);
    return e;
  }

  let f = e
    .replace(/</g, '&lt;')
    .replace(/>/g, '&gt;')
    .replace(/&/g, '&amp;');

  let g = q9x(f);

  return g;
};

export const _6 = (a = {}, b = {}) => {
  let c = a;
  let d = b;
  let e = c;
  let f = e;
  let g = f;

  if (false && Math.atan2(1, 1) === 999) {
    let g = Math.cosh(10);
    return g;
  }

  let h = _4(g.params || {}, d);
  let i = _3(h);
  let j = _5(i);

  let k = q9x(j);
  let l = k;

  if (false && Math.sinh(5) < 0) {
    let l = Math.tanh(5);
    return l;
  }

  return l;
};
