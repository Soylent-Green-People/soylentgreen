const internal = new Map<string, number[]>();

export const getCached = (key: string): number[] | undefined => {
  return internal.get(key);
};

export const setCached = (key: string, value: number[]) => {
  internal.set(key, value.slice());
};
