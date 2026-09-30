import csv, math
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib import font_manager as fm

for f in fm.findSystemFonts():
    if "NotoSansCJK" in f and "Regular" in f:
        fm.fontManager.addfont(f)
plt.rcParams["font.family"] = ["Noto Sans CJK JP", "DejaVu Sans"]
plt.rcParams["axes.unicode_minus"] = False

rows = list(csv.DictReader(open("report/results.csv")))
ALGOS = ["quickSort", "mergeSort", "combSort"]
COL = {"quickSort": "#2b6cb0", "mergeSort": "#2f855a", "combSort": "#c05621"}

def get(shape, algo, key, n=None):
    return [float(r[key]) for r in rows if r["shape"] == shape and r["algo"] == algo and (n is None or int(r["n"]) == n)]

# 1) 입력 모양별 (n=20000)
shapes = ["random", "sorted", "reversed", "dups"]
label = {"random": "무작위", "sorted": "정렬됨", "reversed": "역순", "dups": "중복많음"}
for key, title, fn in [("ms", "걸린 시간 (ms)", "shapes-time"),
                       ("compares", "비교 횟수", "shapes-compares"),
                       ("moves", "이동 횟수", "shapes-moves")]:
    fig, ax = plt.subplots(figsize=(6.2, 3.4))
    w = 0.26
    for i, a in enumerate(ALGOS):
        ax.bar([x + (i - 1) * w for x in range(4)], [get(s, a, key)[0] for s in shapes], w, label=a, color=COL[a])
    ax.set_xticks(range(4)); ax.set_xticklabels([label[s] for s in shapes])
    ax.set_title(f"입력 모양별 {title} (n = 20,000)"); ax.legend(); ax.grid(axis="y", alpha=.3)
    fig.tight_layout(); fig.savefig(f"report/{fn}.png", dpi=170); plt.close(fig)

# 2) n 증가
ns = sorted({int(r["n"]) for r in rows if r["shape"] == "grow"})
slopes = {}
for key, title, fn in [("compares", "비교 횟수", "growth-compares"), ("ms", "걸린 시간 (ms)", "growth-time")]:
    fig, ax = plt.subplots(figsize=(6.2, 3.6))
    for a in ALGOS:
        ys = [get("grow", a, key, n)[0] for n in ns]
        ax.plot(ns, ys, "o-", label=a, color=COL[a])
        slopes[(key, a)] = (math.log(ys[-1]) - math.log(ys[0])) / (math.log(ns[-1]) - math.log(ns[0]))
    ax.set_xscale("log"); ax.set_yscale("log")
    ax.set_xlabel("n"); ax.set_title(f"n이 커질 때 {title} (무작위, 로그-로그)")
    ax.legend(); ax.grid(alpha=.3, which="both")
    fig.tight_layout(); fig.savefig(f"report/{fn}.png", dpi=170); plt.close(fig)

for k, v in slopes.items():
    print(k, round(v, 3))
