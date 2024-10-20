#include <iostream>

/*

API:
    1) Update(int index, int value)
    2) Get(int l, int r, int k)

Update(int index, int value):
    1) arr[index] = value;
    2) update_block(blocks[index / block_len])

update_block(block* b):                 just update zero_count variable
    unsigned int zero_count = 0;
    for (int i = b->l; i <= b->r; ++i)
    {
        if (arr[i] == 0)
            ++zero_count;
    }
    b->zero_count = zero_count;

blocks[], block_len, blocks_num

Get(int l, int r, int k):
    int zero_count = 0;
    for (int i = l; i <= r; )
    {
        int cur_zero_count = zero_count + blocks[block_num]->zero_count;
        block_num = i / block_len;
        if (i % block_len == 0 && i + block_len - 1 <= r && cur_zero_count < k ) {
            zero_count = cur_zero_count;
            i += block_len;
        }
        else {
            if (arr[i] == 0)
                ++zero_count;
            if (zero_count == k)
                return i;
            ++i;
        }
    }
    return -1;  *not found*

*/

int main()
{
    std::cout << "Hello World from lab1!\n";
    return 0;
}