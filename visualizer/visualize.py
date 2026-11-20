#!/usr/bin/env python3
"""push_swap visualizer.

Runs ../push_swap on a list of numbers and animates every instruction
on stacks A and B. Numbers can be random (choose how many) or typed in.

Usage:
    python3 visualizer/visualize.py            # open the GUI
    python3 visualizer/visualize.py 3 1 2      # open with these numbers
    python3 visualizer/visualize.py -n 250     # open with 250 random numbers

Keys: Space play/pause · → step · ← step back · R new random · Enter run
"""
import colorsys
import os
import random
import subprocess
import sys
import tkinter as tk
from collections import Counter, deque
from tkinter import messagebox, ttk

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
BINARY = os.path.join(ROOT, "push_swap")
INVERSE = {"sa": "sa", "sb": "sb", "ss": "ss", "pa": "pb", "pb": "pa",
           "ra": "rra", "rb": "rrb", "rr": "rrr",
           "rra": "ra", "rrb": "rb", "rrr": "rr"}
LIMITS = {3: 3, 5: 12, 100: 700, 500: 5500}

BG = "#16181d"
PANEL = "#1f2229"
FG = "#e6e6e6"
MUTED = "#8b919e"
ACCENT = "#7aa2f7"


def apply(op, a, b):
    """Execute one push_swap instruction on deques a and b."""
    if op in ("sa", "ss") and len(a) > 1:
        a[0], a[1] = a[1], a[0]
    if op in ("sb", "ss") and len(b) > 1:
        b[0], b[1] = b[1], b[0]
    if op == "pa" and b:
        a.appendleft(b.popleft())
    elif op == "pb" and a:
        b.appendleft(a.popleft())
    if op in ("ra", "rr") and a:
        a.rotate(-1)
    if op in ("rb", "rr") and b:
        b.rotate(-1)
    if op in ("rra", "rrr") and a:
        a.rotate(1)
    if op in ("rrb", "rrr") and b:
        b.rotate(1)


def ensure_binary():
    if not os.path.exists(BINARY):
        subprocess.run(["make", "-s", "-C", ROOT], check=False)
    return os.path.exists(BINARY)


def run_push_swap(nums):
    """Return (ops, error_message)."""
    if not ensure_binary():
        return None, "push_swap binary not found and `make` failed."
    proc = subprocess.run([BINARY, *map(str, nums)], capture_output=True,
                          text=True, timeout=60)
    if proc.stderr.strip():
        return None, f"push_swap printed: {proc.stderr.strip()}"
    return proc.stdout.split(), None


def bar_color(rank, n):
    r, g, b = colorsys.hsv_to_rgb(0.75 * rank / max(n - 1, 1), 0.65, 0.95)
    return f"#{int(r * 255):02x}{int(g * 255):02x}{int(b * 255):02x}"


class Visualizer:
    def __init__(self, root, initial=None, count=100):
        self.root = root
        self.nums = []
        self.ops = []
        self.step = 0
        self.playing = False
        self.a = deque()
        self.b = deque()
        self.items = {}
        self.rank = {}
        self._build_ui(count)
        root.bind("<space>", lambda e: self.toggle())
        root.bind("<Right>", lambda e: self.seek(self.step + 1))
        root.bind("<Left>", lambda e: self.seek(self.step - 1))
        root.bind("<Return>", lambda e: self.load_custom())
        root.bind("r", lambda e: self.new_random())
        self.canvas.bind("<Configure>", lambda e: self.redraw(full=True))
        if initial:
            self.entry.insert(0, " ".join(initial))
            root.after(100, self.load_custom)
        else:
            root.after(100, self.new_random)

    # ------------------------------------------------------------------ UI
    def _build_ui(self, count):
        self.root.title("push_swap visualizer")
        self.root.configure(bg=BG)
        self.root.geometry("1100x760")
        self.root.minsize(760, 520)
        style = ttk.Style()
        style.theme_use("clam")
        style.configure(".", background=BG, foreground=FG, fieldbackground=PANEL)
        style.configure("TButton", background=PANEL, foreground=FG, padding=(10, 4))
        style.map("TButton", background=[("active", "#2c303a")])
        style.configure("Accent.TButton", background=ACCENT, foreground="#10131a")
        style.map("Accent.TButton", background=[("active", "#9bb8fa")])
        style.configure("TLabel", background=BG, foreground=FG)
        style.configure("Muted.TLabel", background=BG, foreground=MUTED)
        style.configure("TSpinbox", arrowcolor=FG, foreground=FG)
        style.configure("Horizontal.TScale", background=BG, troughcolor=PANEL)

        top = ttk.Frame(self.root, padding=(12, 10, 12, 4))
        top.pack(fill="x")
        ttk.Label(top, text="How many:").pack(side="left")
        self.count_var = tk.IntVar(value=count)
        ttk.Spinbox(top, from_=1, to=1000, width=6, textvariable=self.count_var,
                    increment=1).pack(side="left", padx=(4, 6))
        for n in (3, 5, 100, 500):
            ttk.Button(top, text=str(n), width=4,
                       command=lambda n=n: (self.count_var.set(n), self.new_random())
                       ).pack(side="left", padx=1)
        ttk.Button(top, text="🎲 Random", style="Accent.TButton",
                   command=self.new_random).pack(side="left", padx=(6, 16))
        ttk.Label(top, text="or your numbers:").pack(side="left")
        self.entry = tk.Entry(top, bg=PANEL, fg=FG, insertbackground=FG,
                              relief="flat", highlightthickness=1,
                              highlightbackground="#333844", highlightcolor=ACCENT)
        self.entry.pack(side="left", fill="x", expand=True, padx=6, ipady=3)
        ttk.Button(top, text="Run", command=self.load_custom).pack(side="left")

        ctl = ttk.Frame(self.root, padding=(12, 4))
        ctl.pack(fill="x")
        self.play_btn = ttk.Button(ctl, text="▶ Play", width=9, command=self.toggle)
        self.play_btn.pack(side="left")
        ttk.Button(ctl, text="⏮", width=3, command=lambda: self.seek(0)).pack(side="left", padx=(6, 1))
        ttk.Button(ctl, text="◀", width=3, command=lambda: self.seek(self.step - 1)).pack(side="left", padx=1)
        ttk.Button(ctl, text="▶", width=3, command=lambda: self.seek(self.step + 1)).pack(side="left", padx=1)
        ttk.Button(ctl, text="⏭", width=3, command=lambda: self.seek(len(self.ops))).pack(side="left", padx=(1, 12))
        ttk.Label(ctl, text="Speed", style="Muted.TLabel").pack(side="left")
        self.speed = tk.DoubleVar(value=55)
        ttk.Scale(ctl, from_=1, to=100, variable=self.speed, length=150).pack(side="left", padx=(4, 12))
        self.speed_lbl = ttk.Label(ctl, text="", style="Muted.TLabel", width=12)
        self.speed_lbl.pack(side="left")
        self.status = ttk.Label(ctl, text="", font=("Menlo", 13, "bold"))
        self.status.pack(side="right")

        self.timeline = tk.DoubleVar(value=0)
        self.scrub = ttk.Scale(self.root, from_=0, to=1, variable=self.timeline,
                               command=lambda v: self.seek(int(float(v))))
        self.scrub.pack(fill="x", padx=12, pady=(2, 6))

        body = tk.Frame(self.root, bg=BG)
        body.pack(fill="both", expand=True, padx=12, pady=(0, 12))
        self.canvas = tk.Canvas(body, bg=PANEL, highlightthickness=0)
        self.canvas.pack(side="left", fill="both", expand=True)
        side = tk.Frame(body, bg=BG, width=190)
        side.pack(side="right", fill="y", padx=(12, 0))
        side.pack_propagate(False)
        ttk.Label(side, text="Instructions", font=("Helvetica", 13, "bold")).pack(anchor="w")
        self.stats = tk.Label(side, bg=BG, fg=FG, font=("Menlo", 12), justify="left", anchor="nw")
        self.stats.pack(anchor="w", fill="x", pady=(4, 10))
        ttk.Label(side, text="Recent", font=("Helvetica", 13, "bold")).pack(anchor="w")
        self.log = tk.Listbox(side, bg=PANEL, fg=FG, font=("Menlo", 12), relief="flat",
                              highlightthickness=0, selectbackground=ACCENT,
                              selectforeground="#10131a", activestyle="none")
        self.log.pack(fill="both", expand=True, pady=(4, 0))
        ttk.Label(side, text="Space play · ←/→ step\nR random · Enter run",
                  style="Muted.TLabel", justify="left").pack(anchor="w", pady=(8, 0))

    # ------------------------------------------------------------------ data
    def new_random(self):
        try:
            n = max(1, min(1000, int(self.count_var.get())))
        except (tk.TclError, ValueError):
            n = 100
        self.count_var.set(n)
        nums = random.sample(range(-(n * 10), n * 10 + 1), n)
        self.entry.delete(0, "end")
        self.entry.insert(0, " ".join(map(str, nums)))
        self.load(nums, [str(x) for x in nums])

    def load_custom(self):
        raw = self.entry.get().split()
        if not raw:
            return
        try:
            nums = [int(x) for x in raw]
        except ValueError:
            nums = None
        self.load(nums, raw)

    def load(self, nums, raw):
        self.pause()
        ops, err = run_push_swap(raw)
        if err:
            messagebox.showerror("push_swap", err)
            return
        self.nums = nums
        self.ops = ops
        self.rank = {v: i for i, v in enumerate(sorted(nums))}
        self.a = deque(nums)
        self.b = deque()
        self.step = 0
        self.scrub.configure(to=max(len(ops), 1))
        self.log.delete(0, "end")
        self.redraw(full=True)
        if ops:
            self.play()

    # ------------------------------------------------------------------ playback
    def ops_per_frame(self):
        s = self.speed.get()  # 1..100, exponential: ~1 op/s .. ~2000 op/s
        return 10 ** ((s - 1) / 99 * 3.3) / 60

    def toggle(self):
        self.pause() if self.playing else self.play()

    def play(self):
        if self.step >= len(self.ops):
            self.seek(0)
        self.playing = True
        self.play_btn.configure(text="⏸ Pause")
        self._acc = 0.0
        self._tick()

    def pause(self):
        self.playing = False
        self.play_btn.configure(text="▶ Play")

    def _tick(self):
        if not self.playing:
            return
        self._acc += self.ops_per_frame()
        n = int(self._acc)
        self._acc -= n
        if n:
            self.seek(self.step + n)
        if self.step >= len(self.ops):
            self.pause()
            return
        self.root.after(16, self._tick)

    def seek(self, target):
        target = max(0, min(len(self.ops), target))
        if target == self.step:
            return
        moved = []
        while self.step < target:
            op = self.ops[self.step]
            apply(op, self.a, self.b)
            self.step += 1
            moved.append(op)
        while self.step > target:
            self.step -= 1
            apply(INVERSE[self.ops[self.step]], self.a, self.b)
        self._update_log(moved, target)
        self.redraw()

    def _update_log(self, moved, target):
        if moved and len(moved) < 40:
            for i, op in enumerate(moved):
                self.log.insert("end", f"{self.step - len(moved) + i + 1:5}  {op}")
        else:
            self.log.delete(0, "end")
            for i in range(max(0, target - 40), target):
                self.log.insert("end", f"{i + 1:5}  {self.ops[i]}")
        if self.log.size() > 200:
            self.log.delete(0, self.log.size() - 200)
        self.log.selection_clear(0, "end")
        if self.log.size():
            self.log.selection_set("end")
            self.log.see("end")

    # ------------------------------------------------------------------ drawing
    def redraw(self, full=False):
        c = self.canvas
        n = len(self.nums)
        w, h = max(c.winfo_width(), 100), max(c.winfo_height(), 100)
        if full:
            c.delete("all")
            self.items = {}
            half = w / 2
            c.create_line(half, 10, half, h - 10, fill="#333844")
            c.create_text(16, 14, text="Stack A", anchor="nw", fill=MUTED, font=("Helvetica", 12, "bold"))
            c.create_text(half + 16, 14, text="Stack B", anchor="nw", fill=MUTED, font=("Helvetica", 12, "bold"))
            self.label_a = c.create_text(half - 16, 14, anchor="ne", fill=MUTED, font=("Menlo", 11))
            self.label_b = c.create_text(w - 16, 14, anchor="ne", fill=MUTED, font=("Menlo", 11))
            self.done_text = c.create_text(w / 4, h / 2, text="", fill="#9ece6a", font=("Helvetica", 28, "bold"))
            gap = 1 if n < 150 else 0
            for v in self.nums:
                r = self.rank[v]
                self.items[v] = c.create_rectangle(0, 0, 0, 0, fill=bar_color(r, n), width=0)
            self._gap = gap
        top, bottom = 40, h - 12
        bar_h = (bottom - top) / max(n, 1)
        half = w / 2
        maxw = half - 32
        for x0, stack in ((16, self.a), (half + 16, self.b)):
            for i, v in enumerate(stack):
                y = top + i * bar_h
                bw = 4 + (maxw - 4) * (self.rank[v] + 1) / n
                c.coords(self.items[v], x0, y, x0 + bw, y + bar_h - self._gap)
        if n:
            c.itemconfigure(self.label_a, text=f"{len(self.a)} items")
            c.itemconfigure(self.label_b, text=f"{len(self.b)} items")
            done = self.step == len(self.ops) and not self.b and list(self.a) == sorted(self.nums)
            c.itemconfigure(self.done_text, text="✓ sorted" if done else "")
            c.tag_raise(self.done_text)
        self._update_stats()

    def _update_stats(self):
        total = len(self.ops)
        self.timeline.set(self.step)
        op = self.ops[self.step - 1] if self.step else "—"
        self.status.configure(text=f"{self.step:>5} / {total}   {op}")
        counts = Counter(self.ops[:self.step])
        lines = [f"{k:<4}{counts.get(k, 0):>6}" for k in
                 ("sa", "sb", "ss", "pa", "pb", "ra", "rb", "rr", "rra", "rrb", "rrr")]
        n = len(self.nums)
        lines.append("")
        lines.append(f"total {total:>5}")
        if n in LIMITS:
            mark = "✓" if total <= LIMITS[n] else "✗"
            lines.append(f"limit {LIMITS[n]:>5} {mark}")
        self.stats.configure(text="\n".join(lines))
        ops_s = self.ops_per_frame() * 60
        self.speed_lbl.configure(text=f"{ops_s:,.0f} ops/s" if ops_s >= 1 else f"{ops_s:.1f} ops/s")


def main():
    args = sys.argv[1:]
    count = 100
    if len(args) == 2 and args[0] == "-n":
        count, args = int(args[1]), []
    elif len(args) == 1:
        args = args[0].split()
    root = tk.Tk()
    Visualizer(root, initial=args or None, count=count)
    root.mainloop()


if __name__ == "__main__":
    main()
