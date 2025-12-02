#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <tuple>
#include <cstring>

// Use int64_t for safety with larger numbers
using i64 = int64_t;

const i64 POW10[] = {
    1, 10, 100, 1000, 10000, 100000, 1000000, 
    10000000, 100000000, 1000000000, 10000000000, 
    100000000000, 1000000000000, 10000000000000, 
    100000000000000, 1000000000000000, 10000000000000000, 
    100000000000000000, 1000000000000000000
};

int main() {
    // --- 1. Fast Parsing & Range Bounds ---
    FILE* f = fopen("../input.txt", "rb");
    if (!f) { std::cerr << "Error opening file\n"; return 1; }
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    std::vector<char> buffer(fsize + 1);
    fread(buffer.data(), 1, fsize, f);
    fclose(f);
    buffer[fsize] = 0;

    std::vector<std::pair<i64, i64>> ranges;
    ranges.reserve(10000); 

    i64 global_max = 0;
    char* ptr = buffer.data();
    
    while (*ptr) {
        // Skip non-digits
        while (*ptr && (*ptr < '0' || *ptr > '9') && *ptr != '-') ptr++;
        if (!*ptr) break;

        i64 start = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            start = (start * 10) + (*ptr - '0');
            ptr++;
        }
        
        if (*ptr == '-') ptr++; 

        i64 end = 0;
        while (*ptr >= '0' && *ptr <= '9') {
            end = (end * 10) + (*ptr - '0');
            ptr++;
        }
        
        if (end > global_max) global_max = end;
        ranges.emplace_back(start, end);
    }

    // --- 2. Adaptive Generator ---
    // Only generate patterns that fit within the user's max range.
    std::vector<i64> bad_ids;
    bad_ids.reserve(20000); // Conservative reservation

    // Max digits needed based on input
    int max_digits = 0;
    i64 temp = global_max;
    while(temp > 0) { temp /= 10; max_digits++; }
    if (max_digits == 0) max_digits = 1;

    // Pattern length 'len'
    // A number with pattern length 'len' repeated at least twice has length >= 2*len.
    // So we only need to check pattern lengths up to max_digits / 2.
    for (int len = 1; len <= max_digits / 2 + 1; ++len) {
        i64 start = POW10[len - 1];
        i64 end = POW10[len];

        for (i64 base = start; base < end; ++base) {
            i64 current = base;
            // Repeat the pattern
            while (true) {
                // Check overflow against global_max before math
                // We need current * 10^len + base <= global_max
                
                // Fast check: if current already has too many digits compared to global_max
                if (current > global_max) break;

                // Optimization: avoid __builtin_mul_overflow check if clearly safe
                // but for strict correctness close to limits:
                if (__builtin_mul_overflow_p(current, POW10[len], (i64)0)) break;
                
                i64 next_val = current * POW10[len] + base;
                
                if (next_val > global_max) break; // Exceeded user's range
                
                current = next_val;
                bad_ids.push_back(current);
            }
        }
    }

    // Deduplicate (e.g., 1111 comes from '1' and '11')
    std::sort(bad_ids.begin(), bad_ids.end());
    bad_ids.erase(std::unique(bad_ids.begin(), bad_ids.end()), bad_ids.end());

    // --- 3. Prefix Sums ---
    std::vector<i64> prefix_sum(bad_ids.size() + 1, 0);
    for (size_t i = 0; i < bad_ids.size(); ++i) {
        prefix_sum[i+1] = prefix_sum[i] + bad_ids[i];
    }

    // --- 4. Sum Ranges ---
    i64 total = 0;
    for (const auto& r : ranges) {
        // Binary search for range bounds in bad_ids
        auto it_start = std::lower_bound(bad_ids.begin(), bad_ids.end(), r.first);
        auto it_end = std::upper_bound(bad_ids.begin(), bad_ids.end(), r.second);

        size_t idx_start = std::distance(bad_ids.begin(), it_start);
        size_t idx_end = std::distance(bad_ids.begin(), it_end);

        if (idx_end > idx_start) {
            total += (prefix_sum[idx_end] - prefix_sum[idx_start]);
        }
    }

    std::cout << "Total: " << total << std::endl;
    return 0;
}