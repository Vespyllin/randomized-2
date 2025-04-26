import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('good.csv')
df2 = pd.read_csv('bad.csv')

df1['err'] = pd.to_numeric(df1['err'], errors='coerce')
df2['err'] = pd.to_numeric(df2['err'], errors='coerce')

df1['err'] = df1['err'] / 100
df2['err'] = df2['err'] / 100

plt.figure(figsize=(10, 6))

plt.plot(df1['R'], df1['err'], label='Relative Error (4-Wise)', marker='o')
plt.plot(df2['R'], df2['err'], label='Relative Error (2-Wise)', marker='s')

plt.xlabel('R')
min_time = min(df1['err'].min(), df2['err'].min())
max_time = max(df1['err'].max(), df2['err'].max())
plt.ylim(min_time * 0.9, max_time * 1.1)

plt.ylabel('R')
plt.title('R vs Avg. Error')
plt.legend()
plt.grid(True)

plt.savefig('avg.jpg', format='jpeg', dpi=300, bbox_inches='tight')  # Avoid cropping labels

