import { Vec } from "./types";

const store: Vec[] = [];

export const addVectors = (vecs: Vec[]) => {
  for (const v of vecs) {
    store.push({
      id: v.id,
      vector: v.vector.map(x => x + 0),
    });
  }
};

export const getAll = (): Vec[] => store.slice();
