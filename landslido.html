<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<title>LANDSLIDO – Landslide risk prediction and early warning</title>
<link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.css">
<style>
:root{--bg:#f0f3f0;--card:#fff;--ink:#17211f;--mute:#5a6967;--line:#d5ddd8;--acc:#1e6f6b;--acc-ink:#fff;--hi:#c8362b;--mid:#c98417;--lo:#3f8f4f;--nav:#0d1918;--navh:64px;--heroH:min(88vh,780px)}
@media (prefers-color-scheme:dark){:root{--bg:#0f1615;--card:#172220;--ink:#e6eeea;--mute:#9bada8;--line:#2a3a37;--acc:#4fb3ad;--acc-ink:#0b1413;--hi:#ef6b5f;--mid:#e3a63c;--lo:#6cc27c}}
*{box-sizing:border-box}html{scroll-behavior:smooth;scroll-padding-top:calc(var(--navh) + 8px)}
body{margin:0;background:var(--bg);color:var(--ink);font:16px/1.6 system-ui,-apple-system,"Segoe UI",sans-serif;position:relative}
h1,h2,h3{font-family:"Iowan Old Style",Georgia,serif;line-height:1.15;margin:0 0 .5rem;letter-spacing:.01em}
h2{font-size:1.7rem}h3{font-size:1.1rem}p{margin:.4rem 0}
a:focus-visible,button:focus-visible,input:focus-visible,select:focus-visible{outline:3px solid var(--acc);outline-offset:2px}
.top{position:sticky;top:0;z-index:20;height:var(--navh);background:var(--nav);color:#e8f0ee;border-bottom:1px solid rgba(255,255,255,.08)}
.top .in{max-width:1120px;margin:0 auto;height:100%;padding:0 18px;display:flex;align-items:center;justify-content:space-between;gap:12px}
.brand{font:700 1.25rem Georgia,serif;letter-spacing:.14em;color:#fff;text-decoration:none}
nav{display:flex;gap:2px;flex-wrap:wrap}nav a{padding:7px 12px;border-radius:99px;color:#c9d8d4;text-decoration:none;font-size:.92rem}
nav a:hover{color:#fff}nav a.on{background:rgba(255,255,255,.14);color:#fff}
@media(max-width:620px){.top{height:auto;min-height:var(--navh)}.top .in{flex-direction:column;padding:8px 12px;gap:4px}nav a{padding:5px 9px;font-size:.85rem}}
#bgwrap{position:absolute;left:0;right:0;top:var(--navh);height:var(--heroH);overflow:hidden;background:linear-gradient(135deg,#12302d,#0a1413);display:none;z-index:0}
#bgwrap.show{display:block}
#bgwrap .l{position:absolute;inset:0;background-size:cover;background-position:center;opacity:0;transition:opacity 1.6s ease}
#bgwrap .l.on{opacity:1}
#bgwrap::after{content:"";position:absolute;inset:0;background:linear-gradient(180deg,rgba(6,14,13,.6),rgba(6,14,13,.82))}
@media(prefers-reduced-motion:reduce){#bgwrap .l{transition:none}html{scroll-behavior:auto}}
main{position:relative;z-index:1}
.wrap{max-width:1120px;margin:0 auto;padding:0 18px}
.hero{min-height:var(--heroH);display:flex;flex-direction:column;justify-content:center;color:#f3f8f6;padding:56px 0 40px}
.eyebrow{font-size:.8rem;letter-spacing:.2em;text-transform:uppercase;color:#a9c9c4}
.hero h1{font-size:clamp(2.6rem,9vw,5.2rem);letter-spacing:.14em;margin:.2rem 0}
.sub{font:italic 1.25rem Georgia,serif;color:#dbe9e6}.lead{max-width:60ch;color:#c4d6d2;margin-top:.8rem}
.row{display:flex;gap:10px;flex-wrap:wrap;margin-top:16px}
.btn{border:1px solid var(--line);background:var(--card);color:var(--ink);padding:10px 18px;border-radius:8px;font:inherit;cursor:pointer;text-decoration:none;display:inline-block}
.btn.p{background:var(--acc);color:var(--acc-ink);border-color:var(--acc);font-weight:600}
.btn.ghost{background:transparent;color:#f3f8f6;border-color:rgba(255,255,255,.5)}
.btn.sm{padding:5px 11px;font-size:.85rem}
.assets{margin-top:36px;font-size:.82rem;color:#9fbab5;display:flex;gap:10px;flex-wrap:wrap;align-items:center}
.assets .btn{background:rgba(255,255,255,.08);color:#dbe9e6;border-color:rgba(255,255,255,.25)}
.sec{padding:56px 0 8px}
.card{background:var(--card);border:1px solid var(--line);border-radius:12px;padding:20px;box-shadow:0 1px 2px rgba(0,0,0,.04)}
.cols{display:grid;grid-template-columns:repeat(auto-fit,minmax(260px,1fr));gap:18px;margin-top:16px}
.two{display:grid;grid-template-columns:1fr 1fr;gap:18px;align-items:start}
@media(max-width:820px){.two{grid-template-columns:1fr}}
.steps{display:grid;grid-template-columns:repeat(auto-fit,minmax(210px,1fr));gap:16px;margin:16px 0 0;padding:0;list-style:none;counter-reset:s}
.steps li{counter-increment:s;border-left:3px solid var(--acc);padding-left:12px}.steps li::before{content:"Step " counter(s);font-size:.78rem;color:var(--mute);display:block}
.tier{display:flex;gap:12px;padding:10px 0;border-top:1px solid var(--line)}.tier:first-of-type{border-top:0}.tier b{display:block}.tier span{font-size:.88rem;color:var(--mute)}
.badge{font-size:.78rem;font-weight:700;padding:3px 9px;border-radius:99px;color:#fff;height:fit-content;white-space:nowrap;display:inline-block}
.HIGH{background:var(--hi)}.MODERATE{background:var(--mid)}.LOW{background:var(--lo)}.grey{background:var(--mute)}
.mute{color:var(--mute);font-size:.88rem}
.pagehead{padding:40px 0 4px}.pagehead p{color:var(--mute);max-width:70ch}
label{display:block;font-size:.85rem;color:var(--mute);margin-top:10px}
input,select{width:100%;padding:9px;border:1px solid var(--line);border-radius:8px;background:var(--bg);color:var(--ink);font:inherit}
.grid{display:grid;grid-template-columns:repeat(auto-fit,minmax(140px,1fr));gap:0 12px}
.res{border-left:6px solid var(--mute)}.res.HIGH{border-color:var(--hi)}.res.MODERATE{border-color:var(--mid)}.res.LOW{border-color:var(--lo)}
.stat{font:700 2rem Georgia,serif}.bar{height:8px;background:var(--line);border-radius:8px;overflow:hidden;margin-top:4px}.bar i{display:block;height:100%;background:var(--acc)}
.kv{display:grid;grid-template-columns:auto 1fr;gap:4px 14px;font-size:.92rem;margin:8px 0}.kv dt{color:var(--mute)}.kv dd{margin:0;word-break:break-word}
#map{height:440px;border-radius:10px;border:1px solid var(--line);margin-top:10px;z-index:0}
.legend{background:var(--card);color:var(--ink);padding:8px 10px;border-radius:8px;font:12px/1.6 system-ui;box-shadow:0 1px 4px rgba(0,0,0,.3)}
.legend i{display:inline-block;width:11px;height:11px;border-radius:50%;margin-right:6px;vertical-align:-1px}
.tabs{display:flex;gap:6px;flex-wrap:wrap;margin:18px 0}.tabs button{border:1px solid var(--line);background:none;color:var(--ink);padding:7px 15px;border-radius:99px;cursor:pointer;font:inherit}.tabs button.on{background:var(--ink);color:var(--bg)}
.scroll{overflow-x:auto}table{width:100%;border-collapse:collapse;font-size:.9rem}th,td{text-align:left;padding:8px 10px;border-bottom:1px solid var(--line);white-space:nowrap}th{color:var(--mute);font-weight:600}
.lcd{background:#1c3a1c;color:#b8f0a0;font:1rem/1.4 ui-monospace,Consolas,monospace;padding:12px 14px;border-radius:8px;letter-spacing:.08em;min-height:64px;border:3px solid #0d1f0d}
.leds{display:flex;gap:14px;margin:12px 0}.led{width:26px;height:26px;border-radius:50%;border:2px solid var(--line);opacity:.3}.led.on{opacity:1;box-shadow:0 0 14px currentColor}
@media(prefers-reduced-motion:no-preference){.led.on.buzz{animation:b .5s infinite}@keyframes b{50%{opacity:.4}}}
.team{display:grid;grid-template-columns:repeat(auto-fit,minmax(240px,1fr));gap:20px;margin-top:18px}
.member{text-align:center;padding:0 0 18px;overflow:hidden}
.ph{aspect-ratio:1/1;width:100%;background:linear-gradient(135deg,#1e6f6b,#12302d);display:flex;align-items:center;justify-content:center;color:#e8f4f1;font:700 3rem Georgia,serif}
.ph img{width:100%;height:100%;object-fit:cover;display:block}
.member h3{margin-top:14px}.role{color:var(--acc);font-weight:600;font-size:.92rem}.member p.mute{padding:0 16px}
footer{margin-top:56px;padding:20px 0 40px;border-top:1px solid var(--line);color:var(--mute);font-size:.85rem}
.toast{position:fixed;bottom:18px;left:50%;transform:translateX(-50%);background:var(--ink);color:var(--bg);padding:10px 18px;border-radius:8px;display:none;z-index:99}
</style>
</head>
<body>
<header class="top"><div class="in"><a class="brand" href="#home">LANDSLIDO</a>
<nav id="nav"><a href="#home" data-v="home">Home</a><a href="#about" data-v="about">About</a><a href="#predict" data-v="predict">Risk Prediction</a><a href="#monitor" data-v="monitor">Monitoring Dashboard</a><a href="#team" data-v="team">Team</a></nav></div></header>
<div id="bgwrap" aria-hidden="true"><div class="l" id="bgA"></div><div class="l" id="bgB"></div></div>
<main id="app"></main>
<div class="toast" id="toast" role="status"></div>
<script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.js"></script>
<script>
/* ============ CONFIGURATION (edit here) ============ */
const CONFIG={
  bgDir:"background image/",          // folder next to this file, discovered automatically when served over http
  teamDir:"team photo/",              // folder next to this file
  districtGeoJson:"kerala_districts.geojson", // optional: put a verified Kerala district GeoJSON next to this file
  slideMs:7000
};
// Team members. If a filename does not contain the person's key, set "photo" to the exact filename.
const TEAM=[
 {name:"Sandhya R",role:"Hardware",key:"sandhya",photo:"",desc:"Hardware integration and hardware implementation."},
 {name:"Praveen R",role:"Software",key:"praveen",photo:"",desc:"Software development and application integration."},
 {name:"Ramkishore",role:"Paper and Research Work",key:"ramkishore",photo:"",desc:"Technical research, literature review, and project documentation."}];
/* ===================================================== */
const LOC={"Munnar":[10.0889,77.0595],"Adimali":[10.0127,76.9566],"Devikulam":[10.0847,77.1124],"Pallivasal":[10.0463,77.0442],"Kallar":[9.9867,77.1233]};
const PRESET={low:{rain:30,soil:25,hum:55,temp:27,wind:10,slope:12},mod:{rain:150,soil:65,hum:80,temp:23,wind:20,slope:30},high:{rain:260,soil:92,hum:95,temp:20,wind:35,slope:48}};
const DEF_RES=[["Anil K","Munnar",10.0895,77.0601],["Bindu S","Munnar",10.0881,77.0580],["Chacko T","Pallivasal",10.0463,77.0442],["Devi R","Devikulam",10.0847,77.1124],["Emmanuel J","Adimali",10.0127,76.9566],["Fathima N","Kallar",9.9867,77.1233],["Gopal M","Munnar",10.0950,77.0700],["Hari P","Adimali",9.9500,76.9300]].map((r,i)=>({id:i+1,name:r[0],locality:r[1],lat:r[2],lng:r[3],phone:"+91 90000 0"+String(1000+i)}));
const MSG={HIGH:"LANDSLIDE DETECTED",MODERATE:"MODERATE LANDSLIDE",LOW:"LOW RISK"};
const COL={HIGH:"#c8362b",MODERATE:"#c98417",LOW:"#3f8f4f"};
const SRC="Rule-based stand-in (not the CNN+LSTM model)";
let S={hw:null,alerts:[],residents:DEF_RES,lastWarn:{},lastSeen:0,sim:false,test:null,ack:null,form:{loc:"Munnar",...PRESET.mod},img:null,imgScore:.3,tab:"overview",result:null};
try{const x=JSON.parse(localStorage.getItem("ldo1")||"null");if(x)S={...S,...x,sim:false,result:null,test:null}}catch(e){}
const save=()=>{try{localStorage.setItem("ldo1",JSON.stringify({alerts:S.alerts,residents:S.residents,lastWarn:S.lastWarn,lastSeen:S.lastSeen,form:S.form,ack:S.ack,tab:S.tab}))}catch(e){}};
const esc=s=>String(s).replace(/[&<>"]/g,c=>({"&":"&amp;","<":"&lt;",">":"&gt;",'"':"&quot;"}[c]));
const $=id=>document.getElementById(id);
const norm=s=>String(s).toLowerCase().replace(/[^a-z0-9]/g,"");
const fmt=t=>new Date(t).toLocaleTimeString([],{hour:"2-digit",minute:"2-digit",second:"2-digit"});
const fmtD=t=>new Date(t).toLocaleString([],{dateStyle:"medium",timeStyle:"medium"});
function toast(m){const t=$("toast");t.textContent=m;t.style.display="block";clearTimeout(toast.h);toast.h=setTimeout(()=>t.style.display="none",2600)}
function km(a,b,c,d){const R=6371,r=Math.PI/180,x=(c-a)*r,y=(d-b)*r,h=Math.sin(x/2)**2+Math.cos(a*r)*Math.cos(c*r)*Math.sin(y/2)**2;return 2*R*Math.asin(Math.sqrt(h))}
function predict(f,img){const n=(v,m)=>Math.min(1,v/m);const s=.3*n(f.rain,300)+.2*n(f.soil,100)+.2*n(f.slope,60)+.1*n(f.hum,100)+.05*n(f.wind,80)+.15*img;return{score:s,level:s<.4?"LOW":s<.65?"MODERATE":"HIGH"}}

/* ---------- asset discovery: backgrounds and team photos ---------- */
const IMG_RE=/\.(jpe?g|png|webp|gif|avif)$/i;
let bgList=[],teamFiles=[],assetNote="Looking for background photographs…",teamNote="";
async function listDir(dir){const r=await fetch(encodeURI(dir),{cache:"no-store"});if(!r.ok)throw new Error("no listing");
 const d=new DOMParser().parseFromString(await r.text(),"text/html");const seen={};
 return [...d.querySelectorAll("a")].map(a=>(a.getAttribute("href")||"").split("?")[0].split("/").pop()).filter(h=>{if(!h)return false;let n;try{n=decodeURIComponent(h)}catch(e){return false}if(!IMG_RE.test(n)||seen[h])return false;return seen[h]=true})
  .map(h=>({name:decodeURIComponent(h),url:encodeURI(dir)+h}))}
function validImages(items){return Promise.all(items.map(it=>new Promise(res=>{const i=new Image();i.onload=()=>res(it);i.onerror=()=>res(null);i.src=it.url}))).then(a=>a.filter(Boolean))}
function fromPicked(files){return [...files].filter(f=>IMG_RE.test(f.name)).sort((a,b)=>a.name.localeCompare(b.name,undefined,{numeric:true})).map(f=>({name:f.name,url:URL.createObjectURL(f)}))}
function pickFolder(cb){const i=document.createElement("input");i.type="file";i.webkitdirectory=true;i.multiple=true;i.onchange=()=>cb(i.files);i.click()}
const layers=[$("bgA"),$("bgB")];let front=0,bgIdx=0,bgTimer=null;
function setBg(items){bgList=items.map(i=>i.url);clearInterval(bgTimer);layers.forEach(l=>{l.classList.remove("on");l.style.backgroundImage=""});
 if(!bgList.length){assetNote="No background photographs found. Open this file through a local server next to your “background image” folder, or choose the folder.";return}
 bgIdx=0;front=0;layers[0].style.backgroundImage=`url("${bgList[0]}")`;layers[0].classList.add("on");
 assetNote=bgList.length+" background photograph"+(bgList.length>1?"s":"")+" loaded.";
 if(bgList.length>1)bgTimer=setInterval(()=>{bgIdx=(bgIdx+1)%bgList.length;const b=1-front;layers[b].style.backgroundImage=`url("${bgList[bgIdx]}")`;layers[b].classList.add("on");layers[front].classList.remove("on");front=b},CONFIG.slideMs)}
function teamPhoto(m){if(m.photo){const f=teamFiles.find(x=>x.name===m.photo);return f||null}
 const hit=teamFiles.filter(x=>norm(x.name.replace(/\.[^.]+$/,"")).includes(m.key));if(hit.length!==1)return null;
 const others=TEAM.filter(o=>o!==m&&norm(hit[0].name).includes(o.key));return others.length?null:hit[0]}
function setTeam(items){teamFiles=items;const miss=TEAM.filter(m=>!teamPhoto(m));
 const amb=TEAM.filter(m=>!m.photo&&teamFiles.filter(x=>norm(x.name).includes(m.key)).length>1).map(m=>m.name);
 teamNote=!items.length?"No team photographs found. Serve this file next to your “team photo” folder, or choose the folder.":
  (amb.length?"Several files match "+amb.join(", ")+"; set an explicit filename in TEAM[].photo. ":"")+(miss.length&&!amb.length?"No clearly matching photograph for "+miss.map(m=>m.name).join(", ")+"; set TEAM[].photo to the filename. ":"")}
async function discover(){
 try{setBg(await validImages(await listDir(CONFIG.bgDir)))}catch(e){setBg([])}
 try{setTeam(await validImages(await listDir(CONFIG.teamDir)))}catch(e){setTeam([])}
 route(true)}

/* ---------- prediction ---------- */
function run(){const f=S.form,p=predict(f,S.imgScore),[la,ln]=LOC[f.loc];const id="P"+Date.now().toString(36).toUpperCase();
 const a={id,risk:p.level,score:p.score,loc:f.loc,lat:la,lng:ln,time:Date.now(),message:MSG[p.level],source:SRC,mode:"SIMULATION",sms:[]};
 if(p.level==="HIGH"){const cool=(Date.now()-(S.lastWarn[f.loc]||0))<600000;
  if(cool)a.note="A warning for this area was already queued in the last 10 minutes.";
  else{S.lastWarn[f.loc]=Date.now();
   S.residents.map(r=>({r,d:km(la,ln,r.lat,r.lng)})).sort((x,y)=>x.d-y.d).forEach(({r,d})=>{
    if(d<=1||r.locality===f.loc)a.sms.push({to:r.name,tier:1,d,text:`HIGH landslide risk in ${f.loc}. Move to a safe place immediately.`});
    else if(d<=10)a.sms.push({to:r.name,tier:2,d,text:`HIGH landslide predicted ~${d.toFixed(1)} km from you near ${f.loc}. Avoid that area.`})})}}
 S.alerts.unshift(a);S.alerts=S.alerts.slice(0,50);S.result=a;S.test=null;save();sendPred(a);route(true)}
function dev(){const age=(Date.now()-S.lastSeen)/1000;const st=S.hw?S.hw.state:(!S.lastSeen?"WAITING":age<20?"CONNECTED":"DISCONNECTED");
 const last=S.alerts[0];let led=null,buz="Off",lcd=["Waiting for","prediction"];
 if(S.test){const t=S.test;led=["red","yellow","green"].includes(t)?t:null;buz=t==="buzzer"?"Test beep":"Off";lcd=t==="lcd"?["TEST MODE","LCD OK"]:["TEST MODE",t.toUpperCase()]}
 else if(st==="DISCONNECTED"){buz="Distinct pattern";lcd=["CONNECTION LOST","Check Wi-Fi"]}
 else if(last){led={HIGH:"red",MODERATE:"yellow",LOW:"green"}[last.risk];buz={HIGH:"Rapid / continuous",MODERATE:"Intermittent beep",LOW:"Brief occasional beep"}[last.risk];lcd=[MSG[last.risk],last.loc]}
 return{st,led,buz,lcd,last}}
const pill=st=>`<span class="badge ${st==="CONNECTED"?"LOW":st==="WAITING"?"MODERATE":"HIGH"}">ESP32 ${st.toLowerCase()}</span>`;
const delivery=a=>!a?"–":S.ack===a.id?"RECEIVED":"PENDING";

/* ---------- views ---------- */
function vHome(){
 const card=m=>{const p=teamPhoto(m);return `<article class="card member"><div class="ph">${p?`<img src="${p.url}" alt="${esc(m.name)}, ${esc(m.role)}">`:`<span role="img" aria-label="No photograph for ${esc(m.name)}">${esc(m.name.split(" ").map(w=>w[0]).join("").slice(0,2).toUpperCase())}</span>`}</div><h3>${esc(m.name)}</h3><div class="role">${esc(m.role)}</div><p class="mute">${esc(m.desc)}</p></article>`};
 return `<div class="wrap"><section class="hero"><div class="eyebrow">Kerala · Student prototype</div><h1>LANDSLIDO</h1>
 <div class="sub">AI-Powered Landslide Risk Prediction and Early Warning System</div>
 <p class="lead">Combining CNN–LSTM fusion, environmental data, and real-time monitoring to support landslide risk assessment and early warning.</p>
 <div class="row"><a class="btn p" href="#predict">Risk Prediction</a><a class="btn ghost" href="#monitor">Monitoring Dashboard</a></div>
 <div class="assets"><span>${esc(assetNote)}</span><button class="btn sm" data-pick="bg">Choose background folder</button></div></section></div>
 <div class="wrap"><section class="sec" id="about"><h2>About the project</h2>
 <p class="mute" style="max-width:72ch">LANDSLIDO is designed to estimate landslide risk from a slope photograph and environmental readings, then pass the result to a monitoring dashboard and an ESP32 warning unit.</p>
 <div class="cols"><div class="card"><h3>Risk assessment</h3><p class="mute">A CNN reads slope imagery, an LSTM reads environmental time series, and the fused output is mapped to LOW, MODERATE or HIGH.</p></div>
 <div class="card"><h3>Monitoring</h3><p class="mute">Every prediction is logged with a time, area and delivery status. The ESP32 reports a heartbeat so a lost connection is visible.</p></div>
 <div class="card"><h3>Early warning</h3><p class="mute">The warning unit shows the risk on an LCD, three LEDs and a buzzer. A lost link is never shown as LOW risk.</p></div></div>
 <ol class="steps"><li><h3>Analyse</h3><p class="mute">Add a slope photograph and readings.</p></li><li><h3>Tier</h3><p class="mute">Residents are sorted by distance and a warning queue is built.</p></li><li><h3>Warn</h3><p class="mute">Only HIGH predictions queue warnings, once per area every 10 minutes.</p></li><li><h3>Track</h3><p class="mute">The ESP32 shows the risk and confirms receipt.</p></li></ol>
 <div class="card" style="margin-top:18px"><h3>Warning distance</h3>
 <div class="tier"><span class="badge HIGH">Tier 1</span><div><b>Within 1 km, or the same locality</b><span>“HIGH landslide risk in the locality. Move to a safe place immediately.”</span></div></div>
 <div class="tier"><span class="badge MODERATE">Tier 2</span><div><b>1 to 10 km away</b><span>“HIGH landslide predicted about X km from you. Avoid that area.”</span></div></div>
 <div class="tier"><span class="badge grey">Log only</span><div><b>More than 10 km away</b><span>No message is queued. The area is logged.</span></div></div></div></section>
 <section class="sec" id="team"><h2>Team</h2><div class="team">${TEAM.map(card).join("")}</div>
 <div class="row" style="align-items:center"><button class="btn sm" data-pick="team">Choose team photo folder</button><span class="mute">${esc(teamNote)}</span></div></section></div>`}

function vPredict(){const f=S.form;
 const fld=(k,l,u,m)=>`<div><label for="f_${k}">${l} (${u})</label><input id="f_${k}" type="number" min="0" max="${m}" step="any" value="${f[k]}" data-k="${k}"></div>`;
 return `<div class="wrap"><div class="pagehead"><h2>Risk prediction</h2><p>Enter the slope readings and an optional photograph. The result, its warning queue and its location on the Kerala map appear below.</p></div>
 <div class="two"><div class="card"><label for="loc">Location</label><select id="loc">${Object.keys(LOC).map(l=>`<option ${l===f.loc?"selected":""}>${l}</option>`).join("")}</select>
 <label for="img">Slope photograph (optional)</label><input id="img" type="file" accept="image/*">
 <div id="prev" class="mute">${S.img?`<img src="${S.img}" alt="Slope preview" style="max-width:100%;border-radius:8px;margin-top:8px">`:"No image yet. Without one, a neutral terrain score is used."}</div>
 <div class="grid">${fld("rain","Rainfall","mm",400)}${fld("soil","Soil moisture","%",100)}${fld("hum","Humidity","%",100)}${fld("temp","Temperature","°C",50)}${fld("wind","Wind","km/h",150)}${fld("slope","Slope angle","°",90)}</div>
 <div class="row"><button class="btn p" id="go">Run prediction</button><button class="btn sm" data-p="low">Low sample</button><button class="btn sm" data-p="mod">Moderate sample</button><button class="btn sm" data-p="high">High sample</button></div></div>
 <div>${S.result?resCard(S.result):`<div class="card"><h3>No result yet</h3><p class="mute">Run a prediction to see the risk level and the warning queue. Nothing is assumed until a prediction exists.</p></div>`}</div></div>
 <section class="card" style="margin-top:18px"><h3>Kerala map</h3><p class="mute" id="mapnote">Drag to pan. Click the map to enable scroll-zoom. Pinch to zoom on touch screens.</p><div id="map"></div></section>
 <p class="mute" style="margin-top:12px">Prediction source: ${esc(SRC)}. A deployed system would call the trained CNN+LSTM model on a backend instead.</p></div>`}
function resCard(a){return `<div class="card res ${a.risk}"><span class="badge ${a.risk}">${a.risk}</span> <b>${esc(a.loc)}</b> <span class="mute">${fmt(a.time)} · ${a.id}</span>
 <p class="stat">${Math.round(a.score*100)}%</p><div class="bar"><i style="width:${a.score*100}%"></i></div>
 <p class="mute">Score from rainfall, soil moisture, slope, humidity, wind and image texture.</p>
 ${a.risk!=="HIGH"?`<p>No warning queued: only HIGH predictions queue warnings.</p>`:a.note?`<p>${a.note}</p>`:`<h3 style="margin-top:12px">Warning queue (${a.sms.length}, nearest first, simulated)</h3>${a.sms.map(m=>`<p><span class="badge ${m.tier===1?"HIGH":"MODERATE"}">T${m.tier}</span> <b>${esc(m.to)}</b> ${m.d.toFixed(1)} km<br><span class="mute">${esc(m.text)}</span></p>`).join("")||"<p>No residents within 10 km.</p>"}<p class="mute">No SMS is sent by this page.</p>`}</div>`}

function vMonitor(){const T=["overview","hardware","history","residents"];
 return `<div class="wrap"><div class="pagehead"><h2>Monitoring dashboard</h2><p>Prediction history, resident list and the warning unit. The panel refreshes every three seconds.</p></div>
 <div class="tabs" role="tablist">${T.map(t=>`<button role="tab" data-t="${t}" class="${S.tab===t?"on":""}">${t[0].toUpperCase()+t.slice(1)}</button>`).join("")}</div><div id="tab">${tabBody()}</div></div>`}
function leds(d){return `<div class="leds">${["red","yellow","green"].map(c=>{const v=c==="red"?"hi":c==="yellow"?"mid":"lo";return `<span class="led ${d.led===c?"on":""} ${c==="red"&&d.led===c?"buzz":""}" style="background:var(--${v});color:var(--${v})" title="${c}"></span>`}).join("")}</div>`}
function tabBody(){const d=dev(),A=S.alerts,t=S.tab,l=d.last;
 if(t==="overview"){const c=k=>A.filter(a=>a.risk===k).length;
  return `<div class="cols" style="grid-template-columns:repeat(auto-fit,minmax(130px,1fr))">${[["Predictions",A.length],["High",c("HIGH")],["Moderate",c("MODERATE")],["Low",c("LOW")],["Residents",S.residents.length]].map(x=>`<div class="card"><div class="stat">${x[1]}</div><span class="mute">${x[0]}</span></div>`).join("")}</div>
  <div class="card" style="margin-top:16px"><h3>Latest prediction</h3>${l?`<dl class="kv"><dt>Risk</dt><dd><span class="badge ${l.risk}">${l.risk}</span></dd><dt>Location</dt><dd>${esc(l.loc)}</dd><dt>Time</dt><dd>${fmtD(l.time)}</dd><dt>ID</dt><dd>${l.id}</dd><dt>Source</dt><dd>${esc(l.source)}</dd><dt>Mode</dt><dd>${l.mode}</dd></dl>`:`<p class="mute">No prediction yet. This is not LOW risk; it means nothing has been analysed.</p>`}</div>`}
 if(t==="history")return `<div class="card scroll"><table><tr><th>Time</th><th>ID</th><th>Area</th><th>Risk</th><th>Warnings queued</th><th>Delivery</th></tr>${A.map(a=>`<tr><td>${fmt(a.time)}</td><td>${a.id}</td><td>${esc(a.loc)}</td><td><span class="badge ${a.risk}">${a.risk}</span></td><td>${a.sms.length}</td><td>${delivery(a)}</td></tr>`).join("")||`<tr><td colspan="6">No predictions yet.</td></tr>`}</table></div>${A.length?`<div class="row"><button class="btn sm" id="clr">Clear history</button></div>`:""}`;
 if(t==="residents")return `<div class="card scroll"><table><tr><th>Name</th><th>Locality</th><th>Phone</th><th></th></tr>${S.residents.map(r=>`<tr><td>${esc(r.name)}</td><td>${esc(r.locality)}</td><td>${esc(r.phone)}</td><td><button class="btn sm" data-del="${r.id}">Remove</button></td></tr>`).join("")}</table></div>
 <div class="card" style="margin-top:14px"><h3>Add a resident</h3><div class="grid"><div><label for="rn">Name</label><input id="rn"></div><div><label for="rl">Locality</label><select id="rl">${Object.keys(LOC).map(x=>`<option>${x}</option>`).join("")}</select></div><div><label for="rp">Phone</label><input id="rp" placeholder="+91…"></div></div><div class="row"><button class="btn p" id="addr">Add resident</button></div></div>`;
 return `<div class="two"><div class="card"><h3>ESP32 warning unit</h3><p>${pill(d.st)}</p>
 <dl class="kv"><dt>Current risk</dt><dd>${l?`<span class="badge ${l.risk}">${l.risk}</span>`:"–"}</dd><dt>Source</dt><dd>${l?esc(l.source):"–"}</dd><dt>Location</dt><dd>${l?esc(l.loc):"–"}</dd><dt>Prediction</dt><dd>${l?fmtD(l.time)+" · "+l.id:"–"}</dd><dt>Delivery</dt><dd>${delivery(l)}</dd><dt>Last heartbeat</dt><dd>${S.lastSeen?fmtD(S.lastSeen):"none yet"}</dd></dl>
 <div class="lcd">${esc(d.lcd[0])}<br>${esc(d.lcd[1])}</div>${leds(d)}<p>Buzzer: <b>${d.buz}</b></p>
 <div class="row">${S.hw?`<span class="badge LOW">LIVE: real data from the server</span>`:`<button class="btn p" id="sim">${S.sim?"Stop ESP32 simulator":"Start ESP32 simulator"}</button>`}</div>
 <p class="mute">Connected means a heartbeat within 20 s. The simulator stands in for a board and acknowledges the latest prediction. RECEIVED is a software acknowledgement only, not proof that a physical device operated.</p></div>
 <div class="card"><h3>Manual hardware test</h3><p class="mute">Drives the display state only. It never creates or changes a prediction.</p><div class="row">${[["red","Red LED"],["yellow","Yellow LED"],["green","Green LED"],["buzzer","Buzzer"],["lcd","LCD"]].map(x=>`<button class="btn sm" data-test="${x[0]}">${x[1]}</button>`).join("")}<button class="btn sm" data-test="">End test</button></div>
 ${S.test?`<p><span class="badge MODERATE">TEST MODE</span> ${esc(S.test)}</p>`:""}</div></div>`}

/* ---------- map ---------- */
let map=null;
function drawMap(){const el=$("map");if(map){map.remove();map=null}if(!el)return;
 if(!window.L){el.innerHTML='<p class="mute" style="padding:16px">The map library could not load. An internet connection is needed.</p>';return}
 map=L.map(el,{scrollWheelZoom:false,minZoom:6,maxZoom:16}).setView([10.35,76.5],7);
 L.tileLayer("https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png",{maxZoom:18,attribution:"© OpenStreetMap contributors"}).addTo(map);
 L.control.scale().addTo(map);
 map.on("click",()=>map.scrollWheelZoom.enable());map.on("mouseout",()=>map.scrollWheelZoom.disable());
 const lg=L.control({position:"bottomright"});lg.onAdd=()=>{const d=L.DomUtil.create("div","legend");d.innerHTML=["LOW","MODERATE","HIGH"].map(k=>`<div><i style="background:${COL[k]}"></i>${k}</div>`).join("");return d};lg.addTo(map);
 const a=S.result;
 if(a){const m=L.circleMarker([a.lat,a.lng],{radius:12,color:"#fff",weight:2,fillColor:COL[a.risk],fillOpacity:.95}).addTo(map);
  m.bindPopup(`<b>${esc(a.loc)}</b><br>Risk: ${a.risk}<br>Score: ${Math.round(a.score*100)}%<br>${fmtD(a.time)}<br><small>Marker placed at the place’s reference coordinates, not a model-detected point.</small>`).openPopup();map.setView([a.lat,a.lng],9)}
 fetch(CONFIG.districtGeoJson).then(r=>r.ok?r.json():Promise.reject()).then(g=>{
  L.geoJSON(g,{style:{color:"#1e6f6b",weight:1.3,fillOpacity:.04},onEachFeature:(f,l)=>{const p=f.properties||{},n=p.DISTRICT||p.district||p.dtname||p.NAME_2||p.NAME||p.name;if(n)l.bindTooltip(String(n),{sticky:true})}}).addTo(map);
  $("mapnote").textContent="District boundaries loaded from "+CONFIG.districtGeoJson+". Drag to pan; click the map to enable scroll-zoom."
 }).catch(()=>{const n=$("mapnote");if(n)n.textContent="District boundaries are not loaded: place a verified Kerala district GeoJSON named “"+CONFIG.districtGeoJson+"” next to this file (served over http). Drag to pan; click the map to enable scroll-zoom."});
 setTimeout(()=>map&&map.invalidateSize(),60)}

/* ---------- routing and binding ---------- */
let curV="home";
function route(force){const h=location.hash.slice(1)||"home";const V=h==="predict"?"predict":h==="monitor"?"monitor":"home";
 curV=V;$("bgwrap").classList.toggle("show",V==="home"&&(bgList.length>0));
 document.querySelectorAll("#nav a").forEach(a=>a.classList.toggle("on",a.dataset.v===h||(h==="home"&&a.dataset.v==="home")));
 $("app").innerHTML=(V==="home"?vHome():V==="predict"?vPredict():vMonitor())+`<div class="wrap"><footer>LANDSLIDO is a student prototype, not a validated public warning service. Predictions on this page come from a rule-based stand-in for the CNN + LSTM fusion model, no SMS is sent, and the ESP32 is simulated unless the page is opened through server.py.</footer></div>`;
 bind();if(V==="predict")drawMap();
 if(force!==true||V!=="home"){if(h==="about"||h==="team")$(h)&&$(h).scrollIntoView();else window.scrollTo(0,0)}}
function bind(){const q=(s,fn)=>document.querySelectorAll(s).forEach(fn);
 q("[data-pick]",e=>e.onclick=()=>pickFolder(files=>{const it=fromPicked(files);
  if(e.dataset.pick==="bg"){validImages(it).then(v=>{setBg(v);if(!v.length)toast("No usable images in that folder");route(true)})}
  else validImages(it).then(v=>{setTeam(v);route(true)})}));
 q("[data-k]",e=>e.oninput=()=>{S.form[e.dataset.k]=+e.value;save()});
 const loc=$("loc");if(loc)loc.onchange=()=>{S.form.loc=loc.value;save()};
 const img=$("img");if(img)img.onchange=()=>{const f=img.files[0];if(!f)return;const r=new FileReader();r.onload=()=>{const im=new Image();im.onload=()=>{const c=document.createElement("canvas");c.width=c.height=32;const x=c.getContext("2d");x.drawImage(im,0,0,32,32);const p=x.getImageData(0,0,32,32).data;let s=0,s2=0;const n=1024;for(let i=0;i<p.length;i+=4){const l=(p[i]*.3+p[i+1]*.59+p[i+2]*.11)/255;s+=l;s2+=l*l}const m=s/n,sd=Math.sqrt(Math.max(0,s2/n-m*m));S.imgScore=Math.min(1,(1-m)*.6+sd*1.2);const t=document.createElement("canvas");t.width=240;t.height=Math.round(240*im.height/im.width);t.getContext("2d").drawImage(im,0,0,t.width,t.height);S.img=t.toDataURL("image/jpeg",.7);$("prev").innerHTML=`<img src="${S.img}" alt="Slope preview" style="max-width:100%;border-radius:8px;margin-top:8px">`;toast("Image scored for terrain texture")};im.onerror=()=>toast("That file is not a readable image");im.src=r.result};r.readAsDataURL(f)};
 q("[data-p]",e=>e.onclick=()=>{S.form={loc:S.form.loc,...PRESET[e.dataset.p]};route(true)});
 const go=$("go");if(go)go.onclick=run;
 q("[data-t]",e=>e.onclick=()=>{S.tab=e.dataset.t;save();route(true)});
 bindTab()}
function bindTab(){const q=(s,fn)=>document.querySelectorAll(s).forEach(fn);
 const clr=$("clr");if(clr)clr.onclick=()=>{S.alerts=[];S.lastWarn={};S.result=null;save();refreshTab()};
 q("[data-del]",e=>e.onclick=()=>{S.residents=S.residents.filter(r=>r.id!==+e.dataset.del);save();refreshTab()});
 const ad=$("addr");if(ad)ad.onclick=()=>{const n=$("rn").value.trim(),l=$("rl").value,p=$("rp").value.trim();if(!n||!/^\+?[\d\s]{8,16}$/.test(p)){toast("Enter a name and a valid phone number");return}const [la,ln]=LOC[l];S.residents.push({id:Date.now(),name:n,locality:l,lat:la+(Math.random()-.5)*.02,lng:ln+(Math.random()-.5)*.02,phone:p});save();refreshTab();toast("Resident added")};
 const sim=$("sim");if(sim)sim.onclick=()=>{S.sim=!S.sim;if(S.sim)beat();refreshTab()};
 q("[data-test]",e=>e.onclick=()=>{S.test=e.dataset.test||null;sendTest(e.dataset.test);refreshTab()})}
function refreshTab(){const t=$("tab");if(t){t.innerHTML=tabBody();bindTab()}}
function beat(){if(S.hw)return;S.lastSeen=Date.now();if(S.alerts[0])S.ack=S.alerts[0].id;save()}
setInterval(()=>{if(S.sim)beat()},5000);
setInterval(()=>{if(curV==="monitor"&&S.tab!=="residents"&&S.tab!=="history"||curV==="monitor"&&S.tab==="history"){const a=document.activeElement;if(!a||!/INPUT|SELECT/.test(a.tagName))refreshTab()}},3000);
/* ---- optional live link to server.py (only works when the page is opened through server.py) ---- */
function post(u,o){return fetch(u,{method:"POST",headers:{"Content-Type":"application/json"},body:JSON.stringify(o)}).catch(()=>null)}
function sendPred(a){post("/api/prediction",{id:a.id,risk:a.risk,location:a.loc,source:a.source,mode:a.mode}).then(r=>{if(r&&!r.ok)toast("Server rejected the prediction")})}
function sendTest(t){post("/api/hardware-test",{target:t||""})}
function pollHw(){fetch("/api/hardware-status",{cache:"no-store"}).then(r=>r.ok?r.json():Promise.reject()).then(j=>{if(!j||!j.state)throw 0;S.hw=j;S.sim=false;S.lastSeen=j.last_seen_age_s==null?0:Date.now()-j.last_seen_age_s*1000;S.ack=j.acked_id}).catch(()=>{S.hw=null})}
setInterval(pollHw,3000);pollHw();
addEventListener("hashchange",()=>route());
route(true);discover();
</script>
</body>
</html>
