# signal_utils.py
import numpy as np

def get_dominant_frequency(signal, sample_rate):
    """
    Returns the dominant frequency in the signal.
    """

    # Compute FFT
    fft_result = np.fft.fft(signal)
    magnitudes = np.abs(fft_result)

    # Generate frequency bins
    freqs = np.fft.fftfreq(len(signal), d=1/sample_rate)

    # Find index of largest magnitude
    peak_index = np.argmax(magnitudes)

    # Return corresponding frequency
    return freqs[peak_index]


if __name__ == "__main__":
    # Create a test signal: 50 Hz sine wave
    sample_rate = 1000  # Hz
    t = np.linspace(0, 1, sample_rate, endpoint=False)
    signal = np.sin(2 * np.pi * 50 * t)

    freq = get_dominant_frequency(signal, sample_rate)
    print("Dominant frequency:", freq)
