import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('chain.csv')
df2 = pd.read_csv('sketch7.csv')
df3 = pd.read_csv('sketch10.csv')
df4 = pd.read_csv('sketch20.csv')

df1['time_s'] = pd.to_numeric(df1['time_s'], errors='coerce')
df2['time_s'] = pd.to_numeric(df2['time_s'], errors='coerce')
df3['time_s'] = pd.to_numeric(df3['time_s'], errors='coerce')
df4['time_s'] = pd.to_numeric(df4['time_s'], errors='coerce')

df1['time_s'] = df1['time_s'] / 1000000000
df2['time_s'] = df2['time_s'] / 1000000000
df3['time_s'] = df3['time_s'] / 1000000000
df4['time_s'] = df4['time_s'] / 1000000000

plt.figure(figsize=(10, 6))

plt.plot(df1['N'], df1['time_s'], label='Chaining with Hashing (R=22)', marker='o')

plt.plot(df2['N'], df2['time_s'], label='Sketch (R=7)', marker='s')
plt.plot(df3['N'], df3['time_s'], label='Sketch (R=10)', marker='^')
plt.plot(df4['N'], df4['time_s'], label='Sketch (R=20)', marker='d')


plt.xlabel('N')
min_time = min(df1['time_s'].min(), df2['time_s'].min(), df3['time_s'].min(), df4['time_s'].min())
max_time = max(df1['time_s'].max(), df2['time_s'].max(), df3['time_s'].max(), df4['time_s'].max())
plt.ylim(min_time * 0.9, max_time * 1.1)

plt.ylabel('Time (seconds)')
plt.title('N vs Average Runtime per Update')
plt.legend()
plt.grid(True)

plt.savefig('performance.jpg', format='jpeg', dpi=300, bbox_inches='tight')

