import numpy as np
from scipy.optimize import curve_fit
import matplotlib.pyplot as plt

# Define the model function
def model_func(x, s):
    return 1 / (s + (1 - s) / x)

# Example data (replace with your actual data)
x_data = np.array([1, 20, 40, 60, 80])  # Replace with your x data
y_data = np.array([1, 12, 24, 34, 46])  # Replace with your y data

# Perform curve fitting
initial_guess = 0.5  # Initial guess for the parameter c
params, covariance = curve_fit(model_func, x_data, y_data, p0=[initial_guess])

# Extract the optimized value of c
fitted_s = params[0]
print(f"Fitted value of s: {fitted_s}")

# Plot the data and the fitted curve
plt.scatter(x_data, y_data, label='Data')
x_fit = np.linspace(min(x_data), max(x_data), 100)
y_fit = model_func(x_fit, fitted_s)
plt.plot(x_fit, y_fit, label=f'Fitted curve (s = {fitted_s:.4f})', color='red')
plt.xlabel('Process Number')
plt.ylabel('SpeedUP')
plt.grid()
plt.legend()
plt.show()
