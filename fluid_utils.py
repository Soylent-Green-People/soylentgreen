# fluid_utils.py
import numpy as np

def simulate_mixing(grid_size=50, steps=100, diffusion=0.1, dt=0.1):
    """
    Simulate mixing of two air masses using a simple advection-diffusion model.
    """

    # Velocity field (constant rightward flow)
    u = np.ones((grid_size, grid_size)) * 1.0  # x-direction
    v = np.zeros((grid_size, grid_size))       # y-direction

    # Scalar field
    field = np.zeros((grid_size, grid_size))
    field[:, :grid_size // 2] = 1.0  # Left half "warm", right half "cold"

    for _ in range(steps):
        field = step(field, u, v, diffusion, dt)

    return field


def step(field, u, v, diffusion, dt):
    """
    One time step of advection + diffusion.
    """

    # Compute gradients
    grad_x = np.roll(field, -1, axis=1) - field
    grad_y = np.roll(field, -1, axis=0) - field

    # Advection term
    advected = field - dt * (u * grad_x + v * grad_y)

    # Diffusion term (Laplacian)
    laplacian = (
        np.roll(field, 1, axis=0) +
        np.roll(field, -1, axis=0) +
        np.roll(field, 1, axis=1) +
        np.roll(field, -1, axis=1) -
        4 * field
    )

    diffused = advected + diffusion * laplacian

    return diffused


if __name__ == "__main__":
    result = simulate_mixing()

    # Quick summary instead of plotting
    print("Min:", result.min())
    print("Max:", result.max())
    print("Mean:", result.mean())
