#!/usr/bin/python3
import os, sys
import pandas as pd
import numpy as np
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
from matplotlib.ticker import ScalarFormatter

# Directory of this script (src/scripts/)
base_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

# Set filename ending, so that plot names match the csv name
filename_end = "depth_5"
csv_filename = os.path.join(base_dir, f"../data/timing_results_{filename_end}.csv")

df = pd.read_csv(csv_filename, sep=",")
df = df.assign(time_per_node = df.time / df.num_nodes) \
    .drop("trial", axis=1)                             \
    .groupby("num_threads")                            \
    .agg({
        "time": ["mean", "std"],
        "num_nodes": ["mean", "std"],
        "time_per_node": ["mean", "std"]
    })

titleSize = 14
labelSize = 12
tickSize = 10

formatter = ScalarFormatter(useMathText=True)
formatter.set_scientific(True)
formatter.set_powerlimits((-3, 3))

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
plt.errorbar(df.index, df.time["mean"]/1e3, yerr=df.time["std"]/1e3, fmt="o-", capsize=2)
plt.title(f"Total Execution Time vs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Execution Time (ms)", fontsize=labelSize)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/avg_time_{filename_end}.png"))

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
plt.errorbar(df.index, df.num_nodes["mean"], yerr=df.num_nodes["std"], fmt="o-", capsize=2)
plt.title(f"Total Number of Nodes Explored vs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Number of Nodes", fontsize=labelSize)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/num_nodes_{filename_end}.png"))

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
plt.errorbar(df.index, df.time_per_node["mean"], yerr=df.time_per_node["std"], fmt="o-", capsize=2)
plt.title(f"Execution Time Per Node vs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Execution Time Per Node (us)", fontsize=labelSize)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/avg_time_per_node_{filename_end}.png"))
