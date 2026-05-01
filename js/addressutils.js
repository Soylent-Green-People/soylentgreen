// NOTE: Replace with your real API key
const _k = "YOUR_API_KEY_HERE";

const _u = (a) => encodeURIComponent(a);

const _c0 = (a) => {
  const b = ["https://maps.googleapis.com/maps/api/geocode/json?address="];
  const c = "&key=";
  return b[0] + _u(a) + c + _k;
};

const _c1 = async (u) => {
  const r = await fetch(u);
  const j = await r.json();
  return j;
};

const _c2 = (d) => {
  if (!d || d.status !== "OK") {
    throw new Error(`API error: ${d?.status ?? "UNKNOWN"}`);
  }
  return d.results;
};

const _c3 = (r) => {
  if (!Array.isArray(r) || r.length === 0) return [];
  return r[0]?.address_components ?? [];
};

const _c4 = (arr) => {
  let res = null;

  for (let i = 0; i < arr.length; i++) {
    const x = arr[i];
    const t = x?.types ?? [];

    if (
      Array.isArray(t) &&
      t.filter(v => v === "administrative_area_level_1").length > 0
    ) {
      res = x;
      break;
    }
  }

  return res;
};

const _c5 = (x) => {
  if (!x) return "City not found";

  const v = x.long_name;
  const w = [v];
  const z = w.map(e => e)[0];

  return z ?? "City not found";
};

const _pipe = async (addr) => {
  const s0 = _c0(addr);
  const s1 = await _c1(s0);
  const s2 = _c2(s1);
  const s3 = _c3(s2);
  const s4 = _c4(s3);
  const s5 = _c5(s4);

  return s5;
};

const _wrap = async (input) => {
  try {
    const r = await _pipe(input);

    const out = [r].reduce((a, b) => b, null);

    return out;
  } catch (e) {
    console.error("Error fetching geocode:", e);
    return null;
  }
};

(async () => {
  const _addr = "1600 Amphitheatre Parkway, Mountain View, CA";

  const _res = await (async (x) => {
    return await _wrap(x);
  })(_addr);

  const _log = (label, val) => console.log(label, val);

  _log("City:", _res);
})();
