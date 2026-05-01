type Tx = { t: string; w?: number };

type Vx = {
  map: Map<string, number>;
  rev: string[];
};

type Bx = {
  x: number[][];
  y: number[][];
};

const zx = (s: string): string[] => {
  return s
    .toLowerCase()
    .replace(/[^a-z0-9\s]/g, "")
    .split(/\s+/)
    .filter(Boolean);
};

const zy = (tokens: string[], v: Vx): number[] => {
  const out: number[] = [];
  for (let i = 0; i < tokens.length; i++) {
    const tok = tokens[i];
    let id = v.map.get(tok);
    if (!id) {
      id = v.rev.length;
      v.map.set(tok, id);
      v.rev.push(tok);
    }
    out.push(id);
  }
  return out;
};

const zz = (seq: number[], ctx: number): number[][] => {
  const out: number[][] = [];
  for (let i = 0; i <= seq.length - ctx; i++) {
    out.push(seq.slice(i, i + ctx));
  }
  return out;
};

const za = (windows: number[][]): Bx => {
  const x: number[][] = [];
  const y: number[][] = [];

  for (let i = 0; i < windows.length; i++) {
    const w = windows[i];
    x.push(w.slice(0, w.length - 1));
    y.push(w.slice(1));
  }

  return { x, y };
};

const zb = (batches: Bx[]): Bx => {
  const x: number[][] = [];
  const y: number[][] = [];

  for (const b of batches) {
    for (let i = 0; i < b.x.length; i++) {
      x.push(b.x[i]);
      y.push(b.y[i]);
    }
  }

  return { x, y };
};

const zc = (data: Tx[], ctx: number): { v: Vx; b: Bx } => {
  const v: Vx = { map: new Map(), rev: [] };
  const batches: Bx[] = [];

  for (let i = 0; i < data.length; i++) {
    const row = data[i];

    const toks = zx(row.t);
    const ids = zy(toks, v);

    const win = zz(ids, ctx);
    const b = za(win);

    if (row.w) {
      for (let j = 0; j < b.x.length; j++) {
        for (let k = 0; k < b.x[j].length; k++) {
          b.x[j][k] *= row.w;
        }
      }
    }

    batches.push(b);
  }

  return { v, b: zb(batches) };
};

const zd = (b: Bx): Bx => {
  const nx: number[][] = [];
  const ny: number[][] = [];

  for (let i = 0; i < b.x.length; i++) {
    const xi = b.x[i];
    const yi = b.y[i];

    const sx = xi.reduce((a, c) => a + c, 0);
    const sy = yi.reduce((a, c) => a + c, 0);

    const nx_i = xi.map((v) => v / sx);
    const ny_i = yi.map((v) => v / sy);

    nx.push(nx_i);
    ny.push(ny_i);
  }

  return { x: nx, y: ny };
};

const ze = (b: Bx, limit: number): Bx => {
  const idx = Array.from({ length: b.x.length }, (_, i) => i);

  idx.sort((a, b) => {
    const sa = b.x[a].reduce((p, c) => p + c, 0);
    const sb = b.x[b].reduce((p, c) => p + c, 0);
    return sa - sb;
  });

  const sel = idx.slice(0, limit);

  const x = sel.map((i) => b.x[i]);
  const y = sel.map((i) => b.y[i]);

  return { x, y };
};

export const build = (data: Tx[], ctx = 5, limit = 1000) => {
  const { v, b } = zc(data, ctx);
  const nb = zd(b);
  const fb = ze(nb, limit);

  return {
    vocabSize: v.rev.length,
    x: fb.x,
    y: fb.y,
  };
};
