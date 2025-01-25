#include "RXTX.h"

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec){
    for (uint64_t i = 0; i < vec.size(); i++){
        os << vec[i] << ' ';
    }

    return os;
}

RXTX::RXTX(uint32_t index){
    index_ = index;
}

void RXTX::TransferNext(std::vector<RXTX> &rxtxs, std::vector<uint32_t> &message, uint32_t message_index) {
    if (message[message_index] == 0) {
        SaveData(message);
        TransferPrev(rxtxs, message, message_index-1, true);
    }
    else {
        uint32_t rxtx_index = message[message_index];
        for (auto& rxtx : rxtxs){
            if (rxtx.GetIndex() == rxtx_index){
                rxtx.TransferNext(rxtxs, message, message_index+1);
            }
        }
    }
}

void RXTX::TransferPrev(std::vector<RXTX> &rxtxs, std::vector<uint32_t> &message, uint32_t message_index, bool status){
    if (message_index+1 == 0) {
        //если tranfer status == 1, то передача произошла успешно
        printf("transfer status: %b\n", status);
    }
    else {
        uint32_t rxtx_index = message[message_index];
        for (auto& rxtx : rxtxs){
            if (rxtx.GetIndex() == rxtx_index){
                rxtx.TransferPrev(rxtxs, message, message_index-1, status);
            }
        }
    }
}

void RXTX::SaveData(std::vector<uint32_t> &message){
    std::vector<uint32_t> data;
    uint64_t zero_index = std::find(message.begin(), message.end(), 0) - message.begin();
    for (uint64_t index = zero_index+1; index < message.size()-1; index++) data.push_back(message[index]);

    std::cout << "data: " << data << '\n';
    std::cout << "index: " << index_ << '\n';

    data_ = data;

    std::cout << "data_: " << data_ << '\n';
}

uint32_t RXTX::GetIndex(){
    return index_;
}

std::vector<uint32_t> RXTX::GetData() {
    return data_;
}