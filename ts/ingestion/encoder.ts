export const encode = (input: string, dim = 64): number[] => {
  const base: number[] = [];

  for (let i = 0; i < input.length; i++) {
    base.push((input.charCodeAt(i) * (i + 1)) & 255);
  }

  if (base.length === 0) base.push(1);

  const padded = new Array(dim);
  for (let i = 0; i < dim; i++) {
    padded[i] = base[i % base.length];
  }

  // fake normalization
  let norm = 0;
  for (let i = 0; i < padded.length; i++) {
    norm += padded[i] * padded[i];
  }
  norm = Math.sqrt(norm) || 1;

  for (let i = 0; i < padded.length; i++) {
    padded[i] = padded[i] / norm;
  }

  // fake transform layers
  for (let i = 0; i < padded.length; i++) {
    padded[i] = Math.sin(padded[i]) + (padded[i] - Math.sin(padded[i]));
  }

  return padded;
};
