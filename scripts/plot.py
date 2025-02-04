import pandas as pd
import matplotlib.pyplot as plt

# Load the CSV file
file_path = 'nogap-denot-recursive-memoization-1-15.csv'

# Read the CSV into a DataFrame
df = pd.read_csv(file_path)

# Strip any extra spaces from the column names
df.columns = df.columns.str.strip()

# Check if the necessary columns are in the dataframe
if 'k' not in df.columns or '%Dx' not in df.columns or '%Dt' not in df.columns or '%Abs' not in df.columns or '%Abe' not in df.columns:
    raise ValueError("The required columns 'k', '%Dx', '%Dt', '%Abs', and '%Abe' are not present in the CSV.")

# Create a figure with 4 subplots (2 rows and 2 columns)
fig, axs = plt.subplots(2, 2, figsize=(16, 12), dpi=300)

# Plotting the data (k vs %Dx)
axs[0, 0].plot(df['k'], df['%Dx'], marker='o', color='dodgerblue', label='%Denot execution time (s)', markersize=6,
               linewidth=1.5, markeredgewidth=1, alpha=0.7)
axs[0, 0].set_title('NOGAP DenotRecursiveMemoization', fontsize=18, fontweight='bold')
axs[0, 0].set_xlabel('k', fontsize=14)
axs[0, 0].set_ylabel('Execution time (seconds)', fontsize=14)
axs[0, 0].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[0, 0].legend(fontsize=12)
axs[0, 0].tick_params(axis='both', labelsize=12)

# Plotting the data (k vs %Dt)
axs[0, 1].plot(df['k'], df['%Dt'], marker='s', color='darkorange', label='%Denot total iterations', markersize=6,
               linewidth=1.5, markeredgewidth=1, alpha=0.7)
axs[0, 1].set_title('NOGAP DenotRecursiveMemoization', fontsize=18, fontweight='bold')
axs[0, 1].set_xlabel('k', fontsize=14)
axs[0, 1].set_ylabel('Total iterations', fontsize=14)
axs[0, 1].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[0, 1].legend(fontsize=12)
axs[0, 1].tick_params(axis='both', labelsize=12)

# Plotting the data (k vs %Abs)
axs[1, 0].plot(df['k'], df['%Abs'], marker='^', color='forestgreen', label='%BackwardNFA total states', markersize=6,
               linewidth=1.5, markeredgewidth=1, alpha=0.7)
axs[1, 0].set_title('NOGAP BackwardNFA total states', fontsize=18, fontweight='bold')
axs[1, 0].set_xlabel('k', fontsize=14)
axs[1, 0].set_ylabel('Total states', fontsize=14)
axs[1, 0].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[1, 0].legend(fontsize=12)
axs[1, 0].tick_params(axis='both', labelsize=12)

# Plotting the data (k vs %Abe)
axs[1, 1].plot(df['k'], df['%Abe'], marker='d', color='firebrick', label='%BackwardNFA total edges', markersize=6,
               linewidth=1.5, markeredgewidth=1, alpha=0.7)
axs[1, 1].set_title('NOGAP BackwardNFA total edges', fontsize=18, fontweight='bold')
axs[1, 1].set_xlabel('k', fontsize=14)
axs[1, 1].set_ylabel('Total edges', fontsize=14)
axs[1, 1].grid(True, which='both', linestyle='--', linewidth=0.5)
axs[1, 1].legend(fontsize=12)
axs[1, 1].tick_params(axis='both', labelsize=12)

# Adjust layout for better spacing between subplots
plt.tight_layout()

# Save the combined plot as an image
output_path = 'nogap-denot-recursive-memoization-1-15.png'
plt.savefig(output_path, dpi=300)