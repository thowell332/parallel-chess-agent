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
csv_filename_0 = os.path.join(base_dir, f"../data/timing_results_pos_0.csv")
csv_filename_1 = os.path.join(base_dir, f"../data/timing_results_pos_1.csv")

def read_timing_results(filename):
    df = pd.read_csv(filename, sep=",")
    avg_time_0 = (df.time[0] + df.time[1] + df.time[2]) / 3
    num_nodes_0 = df.num_nodes[0]
    df = df.assign(speedup_per_node = (avg_time_0 / num_nodes_0) / (df.time / df.num_nodes)) \
        .assign(node_factor = df.num_nodes / num_nodes_0) \
        .assign(total_speedup = avg_time_0 / df.time)     \
        .drop("trial", axis=1)                             \
        .groupby("num_threads")                            \
        .agg({
            "speedup_per_node": ["mean", "std"],
            "node_factor": ["mean", "std"],
            "total_speedup": ["mean", "std"]
        })
    return df

df_0 = read_timing_results(csv_filename_0)
df_1 = read_timing_results(csv_filename_1)

titleSize = 14
labelSize = 12
tickSize = 10

formatter = ScalarFormatter(useMathText=True)
formatter.set_scientific(True)
formatter.set_powerlimits((-3, 3))
labels = ["Early Game", "End Game"]

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
line0 = plt.errorbar(df_0.index, df_0.speedup_per_node["mean"], yerr=df_0.speedup_per_node["std"], fmt="o-", capsize=2)
line1 = plt.errorbar(df_1.index, df_1.speedup_per_node["mean"], yerr=df_1.speedup_per_node["std"], fmt="o-", capsize=2)
plt.title(f"Speedup Factor Per Node vs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Speedup Factor Per Node", fontsize=labelSize)
plt.legend([line0, line1], labels)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/speedup_per_node.png"))

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
line0 = plt.errorbar(df_0.index, df_0.node_factor["mean"], yerr=df_0.node_factor["std"], fmt="o-", capsize=2)
line1 = plt.errorbar(df_1.index, df_1.node_factor["mean"], yerr=df_1.node_factor["std"], fmt="o-", capsize=2)
plt.title(f"Factor of Additional Nodes Explored\nvs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Factor of Additional Nodes", fontsize=labelSize)
plt.legend([line0, line1], labels)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/node_factor.png"))

fig, ax = plt.subplots()
ax.yaxis.set_major_formatter(formatter)
ax.yaxis.get_offset_text().set_visible(True)
plt.axhline(y=1, color="black", linestyle="--")
line0 = plt.errorbar(df_0.index, df_0.total_speedup["mean"], yerr=df_0.total_speedup["std"], fmt="o-", capsize=2)
line1 = plt.errorbar(df_1.index, df_1.total_speedup["mean"], yerr=df_1.total_speedup["std"], fmt="o-", capsize=2)
plt.title(f"Total Speedup Factor vs. Number of Threads", fontsize=titleSize)
plt.xlabel("Number of Threads", fontsize=labelSize)
plt.ylabel("Speedup Factor", fontsize=labelSize)
plt.legend([line0, line1], labels)
plt.tight_layout()
plt.savefig(os.path.join(base_dir, f"../data/total_speedup.png"))
