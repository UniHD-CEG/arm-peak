#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include <ctime>
#include <omp.h>
#include <fstream>

using namespace std;

extern "C" void mac_simd_f32(size_t count);
extern "C" void init_registers();

int main () {

	// define equivalent matrix sizes
	vector<size_t> matrices{ 32, 64, 128, 256, 512, 1024 };
	const size_t threads = 4;

	// log execution time and performance to file
	ofstream f;
	f.open("cpu_peak_performance.log");
	if( f.is_open() )
	{
		// save lof file header
		f << "N;time[ms];performance[GFLOP/s]" << endl; 

		// for different simulated matrix sizes
		for (auto n: matrices)
		{
			cout << "MACs calculated for matrix size " << n << endl;

			// a squared matrix N needs N^3 MACs
			const size_t macs = n * n * n;
			const size_t macs_per_thread = macs / threads;
			// one NEON VMLA launches four float32 MACs 
			const size_t simd_macs_per_thread = macs_per_thread / 4;
			// one mac_simd_f32 iteration does KERNEL_SIZE * 8 VMLAs (see assembly file)
			const size_t kernel_iterations = simd_macs_per_thread / (KERNEL_SIZE * 8);

			// repeat this measurement multiple times, smaller workloads more often
			const size_t norm_max = 16*1024;
			const size_t repeats = (norm_max * norm_max)/(n*n); 

			double total_time_ms = 0.0;
			double normalized_time_ms = 0.0;
			double total_gflops = 0.0;

			auto tStart = chrono::high_resolution_clock::now();
			for (size_t i = 0; i < repeats; i++)
			{
				#pragma omp parallel num_threads(threads)
				{
					// launch vmla instructions
					mac_simd_f32 (kernel_iterations);
				}
			}
			auto tEnd = chrono::high_resolution_clock::now();

			// calculate duration in nanoseconds and convert to milliseconds.
			total_time_ms = chrono::duration_cast<chrono::nanoseconds>(tEnd-tStart).count() * 1e-6; 
			normalized_time_ms = total_time_ms / repeats;

			// caculate GFLOPs
			//   one MAC corresponds to two arithmetic operations
			//   FLOPs = operations / second
			total_gflops = 2.0 * macs / normalized_time_ms * 1e-6; // gflops

			// print and save to log file
			cout << "N=" << n <<  " Total: " << total_gflops << " GFLOP/s" << endl;
			f << n << ";" << normalized_time_ms << ";" << total_gflops << endl;
		}
	} 
	return 0;
}
