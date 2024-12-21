#!/usr/bin/python3
import os, sys
import pandas as pd
import seaborn as sns
import numpy as np
import matplotlib
matplotlib.use("Agg")
from matplotlib import pyplot as plt
import matplotlib.ticker as ticker
from matplotlib.ticker import ScalarFormatter

# Set depths to plot
depths = range(3,7)

# Set file paths and read data
base_dir = os.path.dirname(os.path.abspath(sys.argv[0]))  # Directory of this script (src/scripts/)

def read_timing_results(filename):
    df = pd.read_csv(filename, sep=",")
    seq_time_shared = (df.time_shared[0] + df.time_shared[1] + df.time_shared[2]) / 3
    seq_nodes_shared = df.num_nodes_shared[0]
    seq_time_local = (df.time_local[0] + df.time_local[1] + df.time_local[2]) / 3
    seq_nodes_local = df.num_nodes_local[0]
    seq_time_blended = (df.time_blended[0] + df.time_blended[1] + df.time_blended[2]) / 3
    seq_nodes_blended = df.num_nodes_blended[0]
    df = df.assign(speedup_per_node_shared=(seq_time_shared / seq_nodes_shared) / (df.time_shared / df.num_nodes_shared)) \
        .assign(node_factor_shared=df.num_nodes_shared / seq_nodes_shared) \
        .assign(total_speedup_shared=seq_time_shared / df.time_shared) \
        .assign(speedup_per_node_local=(seq_time_local / seq_nodes_local) / (df.time_local / df.num_nodes_local)) \
        .assign(node_factor_local=df.num_nodes_local / seq_nodes_local) \
        .assign(total_speedup_local=seq_time_local / df.time_local) \
        .assign(speedup_per_node_blended=(seq_time_blended / seq_nodes_blended) / (df.time_blended / df.num_nodes_blended)) \
        .assign(node_factor_blended=df.num_nodes_blended / seq_nodes_blended) \
        .assign(total_speedup_blended=seq_time_blended / df.time_blended) \
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

titleSize = 36
subtitleSize = 32
labelSize = 32
tickSize = 22

formatter = ScalarFormatter(useMathText=True)
formatter.set_scientific(True)
formatter.set_powerlimits((-3, 3))
labels = ["Shared Cutoff Values", "Local Cutoff Values"]
colors = sns.color_palette()
sns.set_style("darkgrid")

def plot_std_shaded(ax, index, df_list):
    lines = []
    idx = 0
    for df in df_list:
        line, = ax.plot(index, df["mean"], marker=".", markersize=12, linewidth=3, color=colors[idx], label=labels[idx])
        lines.append(line)
        ax.fill_between(index, df["mean"] - df["std"], df["mean"] + df["std"], alpha=0.3, color=colors[idx])
        idx += 1
    ax.axhline(y=1, color="grey", linestyle="--", linewidth=3)
    return lines

for plot_type, ylabel in [
    ("speedup_per_node", "Speedup Factor Per Node"),
    ("node_factor", "Factor of Additional Nodes"),
    ("total_speedup", "Total Speedup Factor")
]:
    fig, axes = plt.subplots(len(depths), 2, figsize=(24, 20), sharex=True, sharey=False)
    for i, depth in enumerate(depths):
        csv_filename_0 = os.path.join(base_dir, f"../data/timing_results_0_{depth}.csv")
        csv_filename_1 = os.path.join(base_dir, f"../data/timing_results_1_{depth}.csv")
        df_0 = read_timing_results(csv_filename_0)
        df_1 = read_timing_results(csv_filename_1)
        for j, (ax, df) in enumerate(zip(axes[i], [df_0, df_1])):
            ax.yaxis.set_major_formatter(formatter)
            lines = plot_std_shaded(ax, df.index, [
                getattr(df, f"{plot_type}_shared"),
                getattr(df, f"{plot_type}_local"),
            ])
            ax.grid(True)
            ax.tick_params(axis='x', labelsize=tickSize)
            ax.tick_params(axis='y', labelsize=tickSize)
            ax.yaxis.set_major_formatter(ticker.FormatStrFormatter('%.1f'))
        fig.text(0.02, 0.81 - 0.19 * i,
            f"Depth {depth}",
            ha="left", va="center", fontsize=labelSize, fontweight="bold", rotation="horizontal"
        )
    axes[0][0].set_title("Early Game", fontsize=subtitleSize)
    axes[0][1].set_title("End Game", fontsize=subtitleSize)
    plt.suptitle(f"{ylabel} vs. Number of Theads", fontsize=titleSize, fontweight="bold", x=0.5625)
    fig.text(0.5625, 0.1, "Number of Threads", ha="center", fontsize=labelSize, fontweight="bold")
    leg = fig.legend(lines, labels, bbox_to_anchor=(0.5625, 0.01), loc='lower center', ncol=2, fontsize=labelSize)
    for legobj in leg.legend_handles:
        legobj.set_linewidth(5)
        legobj.set_markersize(20)
    plt.tight_layout(h_pad=1.25, rect=[0.125, 0.125, 1, 0.975])
    plt.savefig(os.path.join(base_dir, f"../data/{plot_type}.png"))
    plt.close()
