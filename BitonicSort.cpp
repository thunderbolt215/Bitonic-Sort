#include <iostream>
#include <cmath>
#include <ctime>
using namespace std;

// 交换浮点数值
void swap(float* x, float* y);
// 分段双调排序接口函数
void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int n, int m);


int main() 
{
    // 计时器
    clock_t start, end;
    start = clock();

    //测试数据1：样例输入
	float data[5] = { 0.8, 0.2, 0.4, 0.6, 0.5 };
	int seg_id[5] = { 0,   0,   1,   1,   1 };
	int seg_start[3] = { 0,2,5 };
	int n = 5;
	int m = 2;

    // 测试数据2：加入sqrt(-1.f)测试鲁棒性
//    float data[12] = { 0.1, sqrt(-1.f), 0.5 ,0.7,0.9,0.8,sqrt(-1.f),0.2,0.3,0.6,sqrt(-1.f),0.0 };
//    int seg_id[12] = { 0,0,0,0,1,1,1,1,1,2,2,2 };
//    int seg_start[4] = { 0,4,9,12 };
//    int n = 12;
//    int m = 3;

    // 测试数据3：大部分为NAN
    //float data[8] = { sqrt(-1.f),sqrt(-1.f),sqrt(-1.f),sqrt(-1.f),0.1,sqrt(-1.f),sqrt(-1.f),sqrt(-1.f) };
    //int seg_id[8] = { 0,0,0,0,1,1,1,1 };
    //int seg_start[3] = { 0,4,8 };
    //int n = 8;
    //int m = 2;

    // 调用分段双调排序函数
    segmentedBitonicSort(data, seg_id, seg_start, n, m);
    
    // 打印data数组，以验证正确性
    for (int i = 0; i < n; i++)
        cout << data[i] << " ";
    cout << endl;

    // 打印所用时间
    end = clock();
    cout << "time = " << double(end - start) / CLOCKS_PER_SEC << "s" << endl;

    return 0;
}

void swap(float* x, float* y)
{
    float temp;
    temp = *x;
    *x = *y;
    *y = temp;
}

void segmentedBitonicSort(float* data, int* seg_id, int* seg_start, int n, int m) 
{
    // 合法性检查
    if (n <= 0 || m <= 0 || m > n ||
        seg_start[m] != n || seg_id[n - 1] != (m - 1) || seg_id[0] != 0)   
        return;

    // 设置填充后的数组长度上限
    const int maxlen = 131072;
    float tmp_seg[maxlen] = { 0.0f };

    for (int i = 0; i < m; i++) 
    { 
        // 获取当前段的长度、首地址、最大值
        int len = seg_start[i + 1] - seg_start[i];
        float* seg = data + seg_start[i]; 
        float max_of_seg = seg[0];
        for (int k = 1; k < len; k++)
            // 此处为鲁棒性加入了NAN的判断，后面代码中同理
            if (isnan(max_of_seg) || max_of_seg < seg[k])
                max_of_seg = seg[k];
        max_of_seg += 1.0f;
            
        // 用最大值填充当前段，使长度为2的幂次方
        int len_filled = 1;
        while (len_filled < len)
            len_filled = len_filled << 1; 
        for (int j = 0; j < len; j++) 
            tmp_seg[j] = seg[j];
        for (int j = len; j < len_filled; j++)
            tmp_seg[j] = max_of_seg;
        
        // 双调排序
        for (int k = 2; k <= len_filled; k = k << 1)
        {
            for (int j = k >> 1; j > 0; j = j >> 1) 
            {
                for (int l = 0; l < len_filled; l++)
                {
                    int lxorj = l ^ j;
                    if (lxorj > l)
                    {
                        if (((l & k) == 0) && 
                            (tmp_seg[lxorj] != tmp_seg[lxorj] || tmp_seg[l] > tmp_seg[lxorj]))
                            swap(&tmp_seg[lxorj], &tmp_seg[l]);
                        if (((l & k) != 0) && 
                            (tmp_seg[l] != tmp_seg[l] || tmp_seg[l] < tmp_seg[lxorj]))
                            swap(&tmp_seg[lxorj], &tmp_seg[l]);
                    }
                }
            }
        }

        // 排序结果写回data
        for (int j = 0; j < len; j++) 
            seg[j] = tmp_seg[j];
    }
}

