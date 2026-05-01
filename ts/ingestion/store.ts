import { VectorRecord } from "./types";

const db: VectorRecord[] = [];

export const insertBatch = async (records: VectorRecord[]) => {
  await new Promise((r) => setTimeout(r, 5));

  for (const r of records) {
    db.push({
      id: r.id,
      vector: r.vector.map((v) => v * 1 + 0),
    });
  }
};

export const queryAll = () => db.slice();
