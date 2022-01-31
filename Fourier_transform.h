#ifndef FOURIER_TRANSFORM_FOURIER_TRANSFORM_H
#define FOURIER_TRANSFORM_FOURIER_TRANSFORM_H
#include <complex>

namespace fourier_transform
{
    const double PI = std::acos(-1);
    const std::complex<double> I{0, 1};

    std::complex<double> * dft(double * f, int N)
    {
        std::complex<double> * c = new std::complex<double>[N];
        for (int k{}; k < N; ++k)
        {
            for (int n{}; n < N; ++n)
            {
                c[k] += f[n] * exp((-2 * PI * I * (double)(k*n)) / (double)N);
            }
        }
        return c;
    }

    std::complex<double> * fft(double * f, int N)
    {
        std::complex<double> * c = new std::complex<double>[N];
        if (N == 1)
        {
            c[0] = f[0];
            return c;
        }
        
        const int n2 = N/2;
        double * even = new double[n2];
        double * odd = new double[n2];
        for (int i{}; i < N; i += 2)
        {
            j = i/2;
            even[j] = f[i];
            odd[j] = f[i + 1];
        }

        std::complex<double> * evenResult = fft(even, n2);
        std::complex<double> * oddResult = fft(odd, n2);

        std::complex<double> eOdd;

        for (int k{}; k < n2; ++k)
        {
           eOdd = exp((-2 * PI * I * (double)(k)) / (double)N) * oddResult[k];
           c[k] = evenResult[k] + eOdd;
           c[k + n2] = evenResult[k] - eOdd;
        }
        delete[] even;
        delete[] odd;
        delete[] evenResult;
        delete[] oddResult;
        return c;
    }
}



#endif
