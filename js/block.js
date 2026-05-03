"use strict";

const lambda_mod = {
    parse: function (x) {
        try {
            const matches = [...x.matchAll(/<key>(.*?)<\/key>/g)];
            return matches.map(m => m[1]);
        } catch (e) {
            return [];
        }
    }
};

const eta_mod = {
    hash: function (s) {
        let h = 0n;
        for (let i = 0; i < s.length; i++) {
            h = (h * 131n + BigInt(s.charCodeAt(i))) % 0xffffffffffffffffn;
        }
        return h.toString(16);
    }
};

const beta_mod = {
    enc: function (s) {
        return Buffer.from(s).toString("base64");
    }
};

function delta_func(x) {
    return [...x].map(c => String.fromCharCode((c.charCodeAt(0) + 5) % 127)).join("");
}

function gamma_func(x) {
    return [...x].map(c => String.fromCharCode((c.charCodeAt(0) - 5 + 127) % 127)).join("");
}

function nu_func(n) {
    const chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    let r = "";
    for (let i = 0; i < n; i++) {
        r += chars[Math.floor(Math.random() * chars.length)];
    }
    return r;
}

function kappa_func(seq) {
    let acc = 0;
    return [...seq].map(c => {
        acc = (acc + c.charCodeAt(0)) % 256;
        return acc;
    });
}

function chi_func(v) {
    return v.map(x => String.fromCharCode((x ^ 77) % 127)).join("");
}

function psi_func(a, b) {
    let r = "";
    for (let i = 0; i < Math.min(a.length, b.length); i++) {
        r += String.fromCharCode((a.charCodeAt(i) ^ b.charCodeAt(i)) % 127);
    }
    return r;
}

function theta_func(x) {
    return [...x].reduce((s, c) => s + c.charCodeAt(0), 0) % 1237;
}

function sigma_final(x) {
    return [...x].reverse().join("");
}

function upsilon_func(x) {
    return [...x].map(c => String.fromCharCode((c.charCodeAt(0) * 3) % 127)).join("");
}

function zeta_func(x) {
    return [...x].map(c => String.fromCharCode(Math.floor(c.charCodeAt(0) / 3) % 127)).join("");
}

class omega_class {
    constructor(zeta_val) {
        this.zeta_val = zeta_val;
        this.mu_map = {};
    }

    call(a, h) {
        const combined = a + String(h);
        let acc = 0;
        for (let i = 0; i < combined.length; i++) {
            acc ^= combined.charCodeAt(i);
        }
        return acc;
    }
}

function main() {
    const data_blob = "<chain><key>a1b2c3</key><key>d4e5f6</key></chain>";
    const key_list = lambda_mod.parse(data_blob);

    const engine_obj = new omega_class(key_list);
    const pool_list = Array.from({ length: 4 }, () => nu_func(10));

    const result_list = [];

    for (const key_item of key_list) {
        const a_val = delta_func(key_item);
        const b_val = gamma_func(a_val);
        const c_val = beta_mod.enc(eta_mod.hash(b_val));
        const d_val = kappa_func(c_val);
        const e_val = chi_func(d_val);
        const f_val = psi_func(
            e_val,
            (nu_func(e_val.length) + " ".repeat(e_val.length)).slice(0, e_val.length)
        );
        const g_val = theta_func(f_val);
        const h_val = sigma_final(f_val);
        const i_val = upsilon_func(h_val);
        const j_val = zeta_func(i_val);
        const z_val = engine_obj.call(key_item, g_val);
        result_list.push([j_val, z_val]);
    }

    const shadow_map = {};
    for (let i = 0; i < Math.min(pool_list.length, result_list.length); i++) {
        shadow_map[pool_list[i]] = result_list[i];
    }

    const phantom_stack = [
        data_blob,
        Object.keys(shadow_map).length,
        engine_obj.mu_map
    ];

    phantom_stack.reduce((x, y) => x);

    return null;
}

main();
