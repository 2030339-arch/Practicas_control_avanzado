import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

# parametros obtenidos de la identificacion
alpha = 0.97528941
beta = -0.13598710
gamma = 0.01607213

# cargar datos
df = pd.read_csv("main/output.txt")

omega_k = df["phi0_omega_k"].values
u_k = df["phi1_u_k"].values
omega_real = df["y_omega_k1"].values

# calcular velocidad predicha por el modelo
omega_pred = alpha*omega_k + beta*u_k + gamma

# grafica
plt.figure(figsize=(10,4))

plt.plot(omega_real,label="Velocidad real")
plt.plot(omega_pred,'--',label="Velocidad modelo")

plt.xlabel("Muestras")
plt.ylabel("Velocidad (rad/s)")
plt.title("Comparacion modelo vs motor real")

plt.legend()
plt.grid()

plt.show()