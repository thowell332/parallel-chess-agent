#!/usr/bin/python3
import os, sys
import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
from matplotlib.ticker import ScalarFormatter

# Set the depth to select files
depth = 5

# Set file paths and read data
base_dir = os.path.dirname(os.path.abspath(sys.argv[0])) # Directory of this script (src/scripts/)
csv_filename_0 = os.path.join(base_dir, f"../data/timing_results_0_{depth}_blended.csv")
csv_filename_1 = os.path.join(base_dir, f"../data/timing_results_1_{depth}_blended.csv")

def read_timing_results(filename):
    df = pd.read_csv(filename, sep=",")
    # Initial values for execution time and number of nodes explored
    seq_time_shared = (df.time_shared[0] + df.time_shared[1] + df.time_shared[2]) / 3
    seq_nodes_shared = df.num_nodes_shared[0]
    seq_time_local = (df.time_local[0] + df.time_local[1] + df.time_local[2]) / 3
    seq_nodes_local = df.num_nodes_local[0]
    seq_time_blended = (df.time_blended[0] + df.time_blended[1] + df.time_blended[2]) / 3
    seq_nodes_blended = df.num_nodes_blended[0]
    # Pre-process data for plotting
    df = df.assign(speedup_per_node_shared = (seq_time_shared / seq_nodes_shared) / (df.time_shared / df.num_nodes_shared)) \
        .assign(node_factor_shared = df.num_nodes_shared / seq_nodes_shared) \
        .assign(total_speedup_shared = seq_time_shared / df.time_shared) \
        .assign(speedup_per_node_local = (seq_time_local / seq_nodes_local) / (df.time_local / df.num_nodes_local)) \
        .assign(node_factor_local = df.num_nodes_local / seq_nodes_local) \
        .assign(total_speedup_local = seq_time_local / df.time_local) \
        .assign(speedup_per_node_blended = (seq_time_blended / seq_nodes_blended) / (df.time_blended / df.num_nodes_blended)) \
        .assign(node_factor_blended = df.num_nodes_blended / seq_nodes_blended) \
        .assign(total_speedup_blended = seq_time_blended / df.time_blended) \
        .drop("trial", axis=1) \
        .drop("pos_idx", axis=1) \
        .groupby("num_threads") \
        .agg({
            "speedup_per_node_shared": ["mean", "std"],
            "node_factor_shared": ["mean", "std"],
            "total_speedup_shared": ["mean", "std"],
            "speedup_per_node_local": ["mean", "std"],
            "node_factor_local": ["mean", "std"],
            "total_speedup_local": ["mean", "std"],
            "speedup_per_node_blended": ["mean", "std"],
            "node_factor_blended": ["mean", "std"],
            "total_speedup_blended": ["mean", "std"]
        })
    return df

df_0 = read_timing_results(csv_filename_0)
df_1 = read_timing_results(csv_filename_1)

titleSize = 18
subtitleSize = 16
labelSize = 16
tickSize = 12

formatter = ScalarFormatter(useMathText=True)
formatter.set_scientific(True)
formatter.set_powerlimits((-3, 3))
labels = ["Shared Cutoff Values", "Local Cutoff Values", "Blended Approach"]
colors = sns.color_palette()
sns.set_style("darkgrid")

def plot_std_shaded(ax, index, df_list):
    lines = []
    idx = 0
    for df in df_list:
        line, = ax.plot(index, df["mean"], marker=".", color=colors[idx], label=labels[idx])
        lines.append(line)
        ax.fill_between(index, df["mean"] - df["std"], df["mean"] + df["std"], alpha=0.3, color=colors[idx])
        idx += 1
    ax.axhline(y=1, color="grey", linestyle="--")
    return lines

# Plot speedup factor per node
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 6), sharey=True)
ax1.yaxis.set_major_formatter(formatter)
ax1.yaxis.get_offset_text().set_visible(True)
ax1.set_title("Early Game", fontsize=labelSize)
ax1.set_ylabel("Speedup Factor Per Node", fontsize=labelSize)
ax1.grid(True)
lines = plot_std_shaded(ax1, df_0.index, [
    df_0.speedup_per_node_shared,
    df_0.speedup_per_node_local,
    df_0.speedup_per_node_blended
])

ax2.set_title("End Game", fontsize=labelSize)
ax2.grid(True)
plot_std_shaded(ax2, df_1.index, [
    df_1.speedup_per_node_shared,
    df_1.speedup_per_node_local,
    df_1.speedup_per_node_blended
])
fig.legend(lines, labels, loc="outside lower center", ncols=2, fontsize=labelSize)
plt.suptitle("Speedup Factor Per Node vs. Number of Threads", fontsize=titleSize)
fig.text(0.5, 0.175, "Number of Threads", fontsize=labelSize, ha='center')
plt.tight_layout(rect=[0, 0.2, 1, 1])
plt.savefig(os.path.join(base_dir, "../data/speedup_per_node.png"))

# Plot factor of additional nodes
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 6), sharey=True)
ax1.yaxis.set_major_formatter(formatter)
ax1.yaxis.get_offset_text().set_visible(True)
ax1.set_title("Early Game", fontsize=labelSize)
ax1.set_ylabel("Factor of Additional Nodes", fontsize=labelSize)
ax1.grid(True)
lines = plot_std_shaded(ax1, df_0.index, [
    df_0.node_factor_shared,
    df_0.node_factor_local,
    df_0.node_factor_blended
])

ax2.set_title("End Game", fontsize=labelSize)
ax2.grid(True)
plot_std_shaded(ax2, df_1.index, [
    df_1.node_factor_shared,
    df_1.node_factor_local,
    df_1.node_factor_blended
])
fig.legend(lines, labels, loc="outside lower center", ncols=2, fontsize=labelSize)
plt.suptitle("Factor of Additional Nodes Explored vs. Number of Threads", fontsize=titleSize)
fig.text(0.5, 0.175, "Number of Threads", fontsize=labelSize, ha='center')
plt.tight_layout(rect=[0, 0.2, 1, 1])
plt.savefig(os.path.join(base_dir, f"../data/node_factor.png"))

# Plot total speedup factor
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 6), sharey=True)
ax1.yaxis.set_major_formatter(formatter)
ax1.yaxis.get_offset_text().set_visible(True)
ax1.set_title("Early Game", fontsize=labelSize)
ax1.set_ylabel("Speedup Factor", fontsize=labelSize)
ax1.grid(True)
lines = plot_std_shaded(ax1, df_0.index, [
    df_0.total_speedup_shared,
    df_0.total_speedup_local,
    df_0.total_speedup_blended
])

ax2.set_title("End Game", fontsize=labelSize)
ax2.grid(True)
plot_std_shaded(ax2, df_1.index, [
    df_1.total_speedup_shared,
    df_1.total_speedup_local,
    df_1.total_speedup_blended
])
fig.legend(lines, labels, loc="outside lower center", ncols=2, fontsize=labelSize)
plt.suptitle("Total Speedup Factor vs. Number of Threads", fontsize=titleSize)
fig.text(0.5, 0.175, "Number of Threads", fontsize=labelSize, ha='center')
plt.tight_layout(rect=[0, 0.2, 1, 1])
plt.savefig(os.path.join(base_dir, f"../data/total_speedup.png"))
