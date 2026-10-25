'use strict';
// LANDSLIDO backend: zero dependencies, needs only Node.js 16+.
const http = require('http'), fs = require('fs'), path = require('path'), os = require('os');
const ROOT = __dirname, PORT = +process.env.PORT || 3000;
const TOKEN = process.env.DEVICE_TOKEN || 'change-me'; // ESP32 sends this in X-Device-Token
const K = 7; // neighbours used by the data-driven model
const IMG = /\.(jpe?g|png|webp|gif|avif)$/i;
const MIME = { '.html': 'text/html', '.js': 'text/javascript', '.css': 'text/css', '.json': 'application/json',
  '.jpg': 'image/jpeg', '.jpeg': 'image/jpeg', '.png': 'image/png', '.webp': 'image/webp', '.gif': 'image/gif', '.avif': 'image/avif' };

// Folder names are matched loosely, so "Background", "team photos", "ML data" all work.
const findDir = re => (fs.readdirSync(ROOT, { withFileTypes: true }).find(d => d.isDirectory() && re.test(d.name)) || {}).name;
const DIRS = { bg: findDir(/^back.?ground/i), team: findDir(/^team/i), ml: findDir(/^(ml|dataset)/i) };
const listImages = d => d ? fs.readdirSync(path.join(ROOT, d)).filter(f => IMG.test(f)).sort()
  .map(f => ({ file: f, url: '/' + encodeURIComponent(d) + '/' + encodeURIComponent(f) })) : [];

// ---- dataset + model ----
function loadDataset() {
  if (!DIRS.ml) return null;
  const csv = fs.readdirSync(path.join(ROOT, DIRS.ml)).filter(n => /\.csv$/i.test(n))[0];
  if (!csv) return null;
  const cut = s => s.trim().replace(/^"|"$/g, '');
  const lines = fs.readFileSync(path.join(ROOT, DIRS.ml, csv), 'utf8').split(/\r?\n/).filter(l => l.trim());
  const head = lines.shift().split(',').map(cut);
  const raw = lines.map(l => l.split(',').map(cut));
  let li = head.findIndex(h => /risk|label|class|target|landslide/i.test(h));
  if (li < 0) li = head.length - 1;
  const words = { low: 0, no: 0, safe: 0, none: 0, 'no risk': 0, medium: .5, moderate: .5, high: 1, yes: 1, risk: 1 };
  let textLabels = false;
  const lab = raw.map(r => { const v = r[li]; if (v !== '' && !isNaN(v)) return +v; textLabels = true; return words[String(v).toLowerCase()]; });
  const mx = textLabels ? 1 : (Math.max(...lab.filter(Number.isFinite)) || 1);
  const idx = head.map((_, i) => i).filter(i => i !== li && raw.filter(r => r[i] !== '' && !isNaN(r[i])).length >= raw.length * .9);
  let rows = raw.map((r, n) => ({ v: idx.map(i => +r[i]), s: lab[n] / mx })).filter(r => r.v.every(Number.isFinite) && Number.isFinite(r.s));
  const f = idx.map((i, c) => {
    const col = rows.map(r => r.v[c]), mean = col.reduce((a, b) => a + b, 0) / col.length;
    const std = Math.sqrt(col.reduce((a, b) => a + (b - mean) ** 2, 0) / col.length) || 1;
    return { name: head[i], mean, std, min: Math.min(...col), max: Math.max(...col) };
  });
  rows = rows.map(r => ({ s: r.s, x: r.v.map((v, c) => (v - f[c].mean) / f[c].std) }));
  return { file: csv, label: head[li], f, rows };
}
let DS = null; try { DS = loadDataset(); } catch (e) { console.error('Dataset error:', e.message); }

function predict(vals) {
  const q = DS.f.map(f => {
    const v = Number(vals && vals[f.name]);
    if (!Number.isFinite(v)) throw new Error('Enter a number for ' + f.name);
    return (v - f.mean) / f.std;
  });
  const near = DS.rows.map(r => ({ s: r.s, d: Math.hypot(...r.x.map((x, i) => x - q[i])) })).sort((a, b) => a.d - b.d).slice(0, K);
  const score = near.reduce((a, b) => a + b.s, 0) / near.length;
  return { score, level: score < .34 ? 'LOW' : score < .67 ? 'MODERATE' : 'HIGH' };
}

// ---- state (saved to .state.json so history survives restarts) ----
const ACT = {
  HIGH: { led: 'RED', buzzer: 'CONTINUOUS', lcd: 'LANDSLIDE DETECTED' },
  MODERATE: { led: 'YELLOW', buzzer: 'INTERMITTENT', lcd: 'MODERATE LANDSLIDE' },
  LOW: { led: 'GREEN', buzzer: 'BRIEF BEEP', lcd: 'LOW RISK' } };
const SF = path.join(ROOT, '.state.json');
let S = { preds: [], seen: 0, tests: [], lastTest: null };
try { S = Object.assign(S, JSON.parse(fs.readFileSync(SF, 'utf8'))); } catch (e) { /* first run */ }
const save = () => fs.writeFileSync(SF, JSON.stringify(S));

const send = (res, code, obj) => { res.writeHead(code, { 'Content-Type': 'application/json', 'Access-Control-Allow-Origin': '*' }); res.end(JSON.stringify(obj)); };
const body = req => new Promise((ok, no) => { let b = ''; req.on('data', c => { b += c; if (b.length > 1e6) { req.destroy(); no(new Error('Too large')); } });
  req.on('end', () => { try { ok(b ? JSON.parse(b) : {}); } catch (e) { no(new Error('Invalid JSON')); } }); });
const clean = s => String(s || '').replace(/[^\w\s.,\-]/g, '').slice(0, 80).trim();

function hwStatus() {
  const last = S.preds[S.preds.length - 1] || null, age = Date.now() - S.seen;
  const act = last ? ACT[last.risk_level] : null;
  return { esp32: !S.seen ? 'WAITING' : age < 20000 ? 'CONNECTED' : 'DISCONNECTED', last_seen: S.seen || null,
    last_prediction: last, delivery: last ? (last.delivered ? 'RECEIVED' : 'PENDING') : null,
    commanded: act ? { led: act.led, buzzer: act.buzzer, lcd: act.lcd } : null, last_test: S.lastTest,
    history: S.preds.slice(-15).reverse().map(p => ({ ...p, hardware_action: ACT[p.risk_level].led + ' LED + ' + ACT[p.risk_level].buzzer.toLowerCase() + ' buzzer' })),
    updated: Date.now() };
}

async function api(req, res, url) {
  const dev = () => { if (req.headers['x-device-token'] !== TOKEN) { send(res, 401, { error: 'Bad or missing X-Device-Token' }); return false; } return true; };
  try {
    if (req.method === 'OPTIONS') { res.writeHead(204, { 'Access-Control-Allow-Origin': '*', 'Access-Control-Allow-Headers': '*', 'Access-Control-Allow-Methods': '*' }); return res.end(); }
    if (url === '/api/manifest') return send(res, 200, { background: listImages(DIRS.bg).map(i => i.url), team: listImages(DIRS.team), folders: DIRS });
    if (url === '/api/dataset') return send(res, 200, DS ? { file: DS.file, label: DS.label, rows: DS.rows.length, features: DS.f.map(({ name, mean, min, max }) => ({ name, mean, min, max })) } : { error: 'No CSV found in the ML data folder' });
    if (url === '/api/hardware-status' && req.method === 'GET') return send(res, 200, hwStatus());
    if (url === '/api/predict' && req.method === 'POST') {
      if (!DS) return send(res, 503, { error: 'No CSV found in the ML data folder' });
      const b = await body(req), r = predict(b.values);
      const p = { prediction_id: String(Date.now()), risk_level: r.level, score: +r.score.toFixed(3), location: clean(b.location) || 'Unspecified',
        timestamp: new Date().toISOString(), message: ACT[r.level].lcd, mode: 'PREDICTION', source: 'DATA MODEL (kNN)', delivered: false };
      S.preds.push(p); S.preds = S.preds.slice(-200); save(); return send(res, 200, p);
    }
    if (url === '/api/hardware-test' && req.method === 'POST') {
      const b = await body(req);
      if (!['red', 'yellow', 'green', 'buzzer', 'lcd'].includes(b.target)) return send(res, 400, { error: 'Invalid target' });
      S.tests.push(b.target); S.lastTest = { target: b.target, queued: Date.now() }; save(); return send(res, 200, { queued: b.target });
    }
    if (url === '/api/latest-prediction' && req.method === 'GET') {
      if (!dev()) return;
      if (S.tests.length) { const t = S.tests.shift(); save(); return send(res, 200, { prediction_id: 'TEST-' + Date.now(), mode: 'TEST', target: t, risk_level: null, message: 'TEST MODE' }); }
      const p = S.preds[S.preds.length - 1];
      if (!p) return send(res, 200, { prediction_id: null, mode: 'PREDICTION', risk_level: null });
      const { score, delivered, ...out } = p; return send(res, 200, out);
    }
    if (url === '/api/esp32/heartbeat' && req.method === 'POST') { if (!dev()) return; await body(req); S.seen = Date.now(); save(); return send(res, 200, { ok: true }); }
    if (url === '/api/esp32/acknowledge' && req.method === 'POST') {
      if (!dev()) return; const b = await body(req); S.seen = Date.now();
      const p = S.preds.find(x => x.prediction_id === String(b.prediction_id));
      if (p && b.status === 'RECEIVED') p.delivered = true; save(); return send(res, 200, { ok: true });
    }
    send(res, 404, { error: 'Not found' });
  } catch (e) { send(res, 400, { error: e.message }); }
}

http.createServer((req, res) => {
  const url = decodeURIComponent(req.url.split('?')[0]);
  if (url.startsWith('/api/')) return api(req, res, url);
  const file = path.join(ROOT, url === '/' ? 'index.html' : url), base = path.basename(file);
  const ok = file.startsWith(ROOT + path.sep) && MIME[path.extname(file)] && !base.startsWith('.') && base !== 'server.js';
  fs.readFile(ok ? file : '', (err, data) => {
    if (err) { res.writeHead(404); return res.end('Not found'); }
    res.writeHead(200, { 'Content-Type': MIME[path.extname(file)] }); res.end(data);
  });
}).listen(PORT, '0.0.0.0', () => {
  const ips = Object.values(os.networkInterfaces()).flat().filter(i => i.family === 'IPv4' && !i.internal).map(i => i.address);
  console.log('\nLANDSLIDO running  ->  http://localhost:' + PORT);
  ips.forEach(ip => console.log('ESP32 server URL   ->  http://' + ip + ':' + PORT));
  console.log('Folders: background=' + DIRS.bg + '  team=' + DIRS.team + '  ml=' + DIRS.ml);
  console.log(DS ? 'Dataset: ' + DS.file + ' (' + DS.rows.length + ' rows, label column "' + DS.label + '")' : 'Dataset: none found');
  if (TOKEN === 'change-me') console.log('WARNING: set DEVICE_TOKEN before connecting real hardware.');
});
