import pandas as pd
import matplotlib.pyplot as plt


df = pd.read_csv('build-debug-gnu/geometry.csv')

df.columns = df.columns.str.strip()

fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2)

ax1.plot(df['fa'], df['volume'])
ax1.set_xlabel('Filling angle, deg')
ax1.set_ylabel('Volume')

ax2.plot(df['fa'], df['area'])
ax2.set_xlabel('Filling angle, deg')
ax2.set_ylabel('Area')

ax3.plot(df['fa'], df['kappa'])
ax3.set_xlabel('Filling angle, deg')
ax3.set_ylabel('Curvature')

ax4.axis('off')

plt.tight_layout()
plt.show()
