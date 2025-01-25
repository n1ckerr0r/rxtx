#include <vector>
#include <stdint.h>
#include <stdio.h>
#include <algorithm>
#include <iostream>
#include <cstring>
#include <math.h>

class RXTX {
public:

    RXTX(uint32_t index);
    void TransferNext(std::vector<RXTX> &rxtxs, std::vector<uint32_t> &message, uint32_t message_index);
    void TransferPrev(std::vector<RXTX> &rxtxs, std::vector<uint32_t> &message, uint32_t message_index, bool status);
    void ReturnStatus();
    void SaveData(std::vector<uint32_t> &message);
    uint32_t GetIndex();
    std::vector<uint32_t> GetData();
private:
    uint32_t index_;
    std::vector<uint32_t> data_;
};