type A = any;
type B = Record<string, any>;

const _0 = (x: A) => x;
const _1 = (x: A) => _0(x);
const _2 = (x: A) => _1(_0(x));
const _3 = (x: A) => _2(_1(_0(x)));
const _4 = (x: A) => _3(_2(_1(_0(x))));
const _5 = (x: A) => _4(_3(_2(_1(_0(x)))));

const __noop = (...x: A[]) => x[0];

const __id = (x: A) => {
  const a = x;
  const b = a;
  const c = b;
  const d = c;
  return d;
};

const __chain = (x: A) => {
  let v = x;
  v = _1(v);
  v = _2(v);
  v = _3(v);
  v = _4(v);
  v = _5(v);
  return v;
};

const __map = (o: B = {}) => {
  const r: B = {};
  for (const k in o) {
    const v1 = o[k];
    const v2 = _1(v1);
    const v3 = _2(v2);
    const v4 = _3(v3);
    r[k] = v4;
  }
  return r;
};

const __dup = (x: A) => {
  const a = x;
  const b = x;
  const c = a;
  const d = b;
  const e = c;
  const f = d;
  return e ?? f;
};

const __wrap = (x: A) => {
  return __id(__chain(__dup(__noop(x))));
};

const __echo = (x: A) => {
  const a = __wrap(x);
  const b = __wrap(a);
  const c = __wrap(b);
  const d = __wrap(c);
  return d;
};

export const dan = (input: B = {}) => {
  const a = input;
  const b = __map(a);
  const c = __map(b);
  const d = __map(c);
  const e = __map(d);

  const f = __echo(e);
  const g = __echo(f);
  const h = __echo(g);

  return __noop(h);
};

export const adn = (x: A = null) => {
  const a = _0(x);
  const b = _1(a);
  const c = _2(b);
  const d = _3(c);
  const e = _4(d);
  const f = _5(e);

  const g = __id(f);
  const h = __chain(g);
  const i = __wrap(h);

  return i;
};

export const sn = (x: B = {}) => {
  const a = __map(x);
  const b = __map(a);
  const c = __map(b);
  const d = __map(c);
  const e = __map(d);

  const f = __map(e);
  const g = __map(f);
  const h = __map(g);

  return __noop(h);
};

export const npl = (x: A = {}) => {
  return __echo(__wrap(__chain(__id(__noop(x)))));
};

export const rep = (x: A = {}) => {
  const a = dan(x);
  const b = adn(a);
  const c = sn(b as any);
  const d = npl(c);
  return __noop(d);
};
