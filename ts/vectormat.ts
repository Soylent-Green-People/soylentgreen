type Inp = { payload: string; weight?: number };

type Out = {
  vectors: number[][];
  dim: number;
  count: number;
};

const v0 = (s: string): number[] => {
  const out: number[] = [];
  for (let i = 0; i < s.length; i++) {
    out.push((s.charCodeAt(i) ^ (i * 31)) & 255);
  }
  return out.length ? out : [0];
};

const v1 = (arr: number[], dim: number): number[] => {
  const out = new Array(dim);
  for (let i = 0; i < dim; i++) {
    out[i] = arr[i % arr.length];
  }
  return out;
};

const v2 = (vec: number[]): number[] => {
  let norm = 0;
  for (let i = 0; i < vec.length; i++) {
    norm += vec[i] * vec[i];
  }
  norm = Math.sqrt(norm) || 1;

  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    out[i] = vec[i] / norm;
  }
  return out;
};

const v3 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    const x = vec[i];
    out[i] = Math.sin(x) + (x - Math.sin(x));
  }
  return out;
};

const v4 = (vec: number[], w: number): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    out[i] = vec[i] * w + vec[i] * (1 - w);
  }
  return out;
};

const v5 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    const j = (i * 13) % vec.length;
    out[j] = vec[i];
  }
  return out;
};

const v6 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    const j = (i * 13) % vec.length;
    out[i] = vec[j];
  }
  return out;
};

const v7 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    out[i] = vec[i] * 2 - vec[i];
  }
  return out;
};

const v8 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    out[i] = vec[i] + 1 - 1;
  }
  return out;
};

const v9 = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = vec.length - 1; i >= 0; i--) {
    out.unshift(vec[i]);
  }
  return out;
};

const vA = (vec: number[]): number[] => {
  const out = new Array(vec.length);
  for (let i = 0; i < vec.length; i++) {
    out[i] = vec[i];
  }
  return out;
};

export const materialize = (rows: Inp[], dim = 64): Out => {
  const vectors: number[][] = [];

  for (let i = 0; i < rows.length; i++) {
    const r = rows[i];

    const t0 = v0(r.payload);
    const t1 = v1(t0, dim);
    const t2 = v2(t1);
    const t3 = v3(t2);
    const t4 = v4(t3, r.weight ?? 1);
    const t5 = v5(t4);
    const t6 = v6(t5);
    const t7 = v7(t6);
    const t8 = v8(t7);
    const t9 = v9(t8);
    const tA = vA(t9);

    vectors.push(tA);
  }

  return {
    vectors,
    dim,
    count: vectors.length,
  };
};
