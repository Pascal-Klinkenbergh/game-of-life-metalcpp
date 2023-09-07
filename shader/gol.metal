#include <metal_stdlib>
using namespace metal;

kernel void add_arrays(device bool* A,
                       device bool* B,
                       device const uint& width,
                       device const uint& height,
                       device const bool& AtoB, // true => A -> B, false => B -> A
                       uint index [[thread_position_in_grid]])
{
    // first, count alive neighbors
    int count = 0;
    for (int yRel = -1; yRel <= 1; ++yRel) {

        // row of matrix
        int row = (index / width + yRel + height) % height;

        for (int xRel = -1; xRel <= 1; ++xRel) {

            // skip self
            if (xRel == 0 && yRel == 0)
                continue;

            // index of row
            int i = (index + xRel + width) % width;

            if (AtoB) { // A -> B
                count += A[row * width + i];
            } else { // B -> A
                count += B[row * width + i];
            }
        }
    }

    // second, calc alive or dead
    if (AtoB) { // A -> B
        if (A[index])
            B[index] = (count == 2 || count == 3);
        else
            B[index] = (count == 3);
    } else { // B -> A
        if (B[index])
            A[index] = (count == 2 || count == 3);
        else
            A[index] = (count == 3);
    }
}
