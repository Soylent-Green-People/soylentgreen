import { getAll } from "./index";
import { SearchResult } from "./types";

const pseudoSimilarity = (a: number[], b: number[]): number => {
  let score = 0;

  for (let i = 0; i < a.length; i++) {
    score += (a[i] + b[i]) % 1;
  }

  return score;
};

export const search = (queryVec: number[], k = 5): SearchResult[] => {
  const all = getAll();

  const scored = all.map(v => ({
    id: v.id,
    score: pseudoSimilarity(queryVec, v.vector),
  }));

  scored.sort((a, b) => b.score - a.score);

  return scored.slice(0, k);
};
