"""
Build an LSTM-ready landslide dataset for Kerala.

Input : kerala_cleaned_data.xlsx  (sheet Events_summary, rows with status INCLUDE)
Output: kerala_lstm_dataset.npz   X/y arrays (train / val / test, scaled)
        kerala_samples.csv        one row per sample (event or no-event), with split + label
        kerala_sequences.csv      the 14-day windows in long format (unscaled), easy to inspect
        scaler.json               means and stds from the training set (reuse on the website)

Needs internet (downloads daily history from NASA POWER).  pip install pandas numpy requests openpyxl
Run:   python build_lstm_dataset.py
"""
import json, time, sys
import numpy as np, pandas as pd, requests

# ---------------- settings you can change ----------------
IN_FILE     = "kerala_cleaned_data.xlsx"
SEQ_LEN     = 14        # days of history fed to the LSTM
LEAD_DAYS   = 1         # window ends this many days BEFORE the reference date (1 = predict one day ahead)
REF_OFFSET  = 1         # reference date = window start + REF_OFFSET (1 = middle of the 3-day event window)
NEG_PER_POS = 3         # no-landslide samples per landslide
EXCLUDE_DAYS = 30       # a negative must be this far from every known event at the same place
HIST_START, HIST_END = "2019-12-01", None   # None = up to today
SEED = 42
TRAIN_END, VAL_END = 2023, 2024   # train <= 2023, val = 2024, test >= 2025 (split by year, not random)
RAW = {  # NASA POWER name -> our name
    "T2M": "temp_c", "RH2M": "rh_pct", "WS2M": "wind_ms",
    "GWETROOT": "soil_root", "GWETTOP": "soil_surface", "PRECTOTCORR": "rain_mm",
}
FEATURES = ["temp_c", "rh_pct", "wind_ms", "soil_root", "soil_surface", "rain_mm",
            "rain_3d", "rain_7d", "rain_14d"]
# ----------------------------------------------------------

def fetch_power(lat, lon, start, end):
    """Daily weather for one point from NASA POWER. Returns a DataFrame indexed by date."""
    url = "https://power.larc.nasa.gov/api/temporal/daily/point"
    params = dict(parameters=",".join(RAW), community="AG", longitude=lon, latitude=lat,
                  start=pd.Timestamp(start).strftime("%Y%m%d"), end=pd.Timestamp(end).strftime("%Y%m%d"),
                  format="JSON")
    for attempt in range(4):
        try:
            r = requests.get(url, params=params, timeout=90)
            r.raise_for_status()
            p = r.json()["properties"]["parameter"]
            df = pd.DataFrame({RAW[k]: pd.Series(v) for k, v in p.items() if k in RAW})
            df.index = pd.to_datetime(df.index, format="%Y%m%d")
            return df.replace(-999, np.nan).sort_index()
        except Exception as e:
            print(f"  retry {attempt+1} for ({lat},{lon}): {e}", file=sys.stderr)
            time.sleep(3 * (attempt + 1))
    raise RuntimeError(f"NASA POWER failed for {lat},{lon}")

def add_features(df):
    df = df.interpolate(limit=3, limit_direction="both")
    for n in (3, 7, 14):
        df[f"rain_{n}d"] = df["rain_mm"].rolling(n, min_periods=n).sum()
    return df

def window(df, last_day):
    w = df.loc[last_day - pd.Timedelta(days=SEQ_LEN - 1): last_day, FEATURES]
    return w if len(w) == SEQ_LEN and not w.isna().any().any() else None

def main():
    ev = pd.read_excel(IN_FILE, sheet_name="Events_summary")
    ev = ev[ev.status == "INCLUDE"].copy()
    ev["start"], ev["end"] = pd.to_datetime(ev["start"]), pd.to_datetime(ev["end"])
    ev["ref_date"] = ev["start"] + pd.Timedelta(days=REF_OFFSET)
    end = pd.Timestamp(HIST_END) if HIST_END else pd.Timestamp.today().normalize() - pd.Timedelta(days=7)
    rng = np.random.default_rng(SEED)

    series, rows, wins = {}, [], []
    for i, e in enumerate(ev.itertuples(), 1):
        key = (round(e.lat, 4), round(e.lon, 4))
        if key not in series:
            print(f"[{i}/{len(ev)}] downloading {key}")
            series[key] = add_features(fetch_power(e.lat, e.lon, HIST_START, end))
            time.sleep(1)
    event_days = ev.groupby(ev.apply(lambda r: (round(r.lat, 4), round(r.lon, 4)), axis=1))["ref_date"].apply(list).to_dict()

    sid = 0
    def add(kind, key, ref, label, eid):
        nonlocal sid
        w = window(series[key], ref - pd.Timedelta(days=LEAD_DAYS))
        if w is None: return False
        sid += 1
        rows.append(dict(sample_id=sid, event_id=eid, kind=kind, label=label, ref_date=ref.date(),
                         year=ref.year, month=ref.month, lat=key[0], lon=key[1]))
        t = w.reset_index(names="date"); t.insert(0, "sample_id", sid); t["step"] = range(1, SEQ_LEN + 1)
        wins.append(t)
        return True

    for e in ev.itertuples():
        key = (round(e.lat, 4), round(e.lon, 4))
        if not add("landslide", key, e.ref_date, 1, e.event_id):
            print(f"  skipped {e.event_id}: incomplete weather window"); continue
        s = series[key]
        ok = s.index[(s.index >= s.index[0] + pd.Timedelta(days=SEQ_LEN + 3))]
        far = [d for d in ok if all(abs((d - x).days) > EXCLUDE_DAYS for x in event_days[key])]
        monsoon = [d for d in far if d.month in (6, 7, 8, 9)]
        other = [d for d in far if d.month not in (6, 7, 8, 9)]
        n_m = NEG_PER_POS // 2 + NEG_PER_POS % 2
        picks = list(rng.choice(monsoon, min(n_m, len(monsoon)), replace=False)) if monsoon else []
        picks += list(rng.choice(other, min(NEG_PER_POS - len(picks), len(other)), replace=False)) if other else []
        for d in picks: add("no_landslide", key, pd.Timestamp(d), 0, f"NEG-{e.event_id}")

    meta = pd.DataFrame(rows)
    meta["split"] = np.where(meta.year <= TRAIN_END, "train", np.where(meta.year <= VAL_END, "val", "test"))
    seq = pd.concat(wins, ignore_index=True)

    X = np.stack([g.sort_values("step")[FEATURES].to_numpy(np.float32) for _, g in seq.groupby("sample_id")])
    y = meta.sort_values("sample_id").label.to_numpy(np.int64)
    sp = meta.sort_values("sample_id").split.to_numpy()

    tr = sp == "train"
    mu, sd = X[tr].reshape(-1, len(FEATURES)).mean(0), X[tr].reshape(-1, len(FEATURES)).std(0) + 1e-6
    Xs = (X - mu) / sd
    json.dump(dict(features=FEATURES, mean=mu.tolist(), std=sd.tolist(), seq_len=SEQ_LEN), open("scaler.json", "w"), indent=2)
    np.savez_compressed("kerala_lstm_dataset.npz", features=np.array(FEATURES),
                        **{f"X_{s}": Xs[sp == s] for s in ("train", "val", "test")},
                        **{f"y_{s}": y[sp == s] for s in ("train", "val", "test")})
    meta.to_csv("kerala_samples.csv", index=False); seq.to_csv("kerala_sequences.csv", index=False)

    print("\nX shape:", X.shape, "(samples, days, features)")
    print(meta.groupby(["split", "label"]).size().unstack(fill_value=0))
    if (meta.groupby("split").label.sum() < 5).any():
        print("WARNING: fewer than 5 landslides in a split. Add more events or change TRAIN_END / VAL_END.")

if __name__ == "__main__":
    main()
