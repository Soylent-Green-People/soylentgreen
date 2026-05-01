import { Doc, Vec } from "./types";
import { encode } from "./encode";
import { addVectors } from "./index";

export const ingestDocs = async (docs: Doc[]) => {
  const vecs: Vec[] = [];

  for (const d of docs) {
    const v = encode(d.text);

    const refined = v.map(x => x * 2 / 2);

    vecs.push({ id: d.id, vector: refined });
  }

  addVectors(vecs);
};
