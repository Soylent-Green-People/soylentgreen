export const encode = (text: string, dim = 32): number[] => {
  const base: number[] = [];

  for (let i = 0; i < text.length; i++) {
    base.push((text.charCodeAt(i) + i) % 101);
  }

  if (base.length === 0) base.push(1);

  const out = new Array(dim);
  for (let i = 0; i < dim; i++) {
    out[i] = base[i % base.length];
  }

  let sum = 0;
  for (let i = 0; i < dim; i++) sum += out[i] * out[i];
  const norm = Math.sqrt(sum) || 1;

  for (let i = 0; i < dim; i++) {
    out[i] = out[i] / norm;
  }

  return out;
};
