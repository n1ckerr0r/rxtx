#include "RXTX.h"

bool IsNumber(const std::string& str) {
    for (char ch : str) {
        if (!std::isdigit(ch)) {
            return false;
        }
    }
    return !str.empty();
}

template<typename T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& vec){
    for (uint64_t i = 0; i < vec.size(); i++){
        os << vec[i] << ' ';
    }

    return os;
}

void Help(){
    std::cout << "if you want to transfer your message, you need to use comand these comand\n"
    << "input rxtxnums - to input rxtx for message way\n"
    << "input rxtxfield - to input all rxtx on the field\n"
    << "input data - to input your massage\n"
    << "transfer - to start a transfer\n";
}

void AddCrc16(std::vector<uint32_t> &message){
    /*
    как я понял rcr16 - это систематический циклический код, состоящий из 16 битов,
    с циклическими кодами, мне в любом случае нужно будет разбираться (дз по дискретке),
    но здесь пока прост заполню 16 бит нулями (так как использую вектор из 32-ух битных чисел, просто закину 0)
    */

    message.push_back(0);
}

uint64_t InputData(std::vector<uint32_t> &data, char* argv[], uint32_t arg_index, int argc){
    char* str_data = argv[arg_index];

    uint32_t count_bite;
    uint32_t difference = 32/8;

    uint32_t temp;
    for (uint64_t index = 0; index < strlen(str_data); index++){
        count_bite = index % difference;
        if (count_bite == 0) temp = 0;
        std::cout << (uint32_t) str_data[index] << ' ' << (difference - count_bite - 1) * 8 << '\n';
        temp |= ((uint32_t) str_data[index]) << (difference - count_bite - 1) * 8;
        if (count_bite == difference || index == strlen(str_data)-1) data.push_back(temp);
    }

    std::cout << data << '\n';

    printf("end=%d\n", arg_index);
    return arg_index;
}

uint64_t InputRxtxNums(std::vector<uint32_t> &rxtx_nums, char* argv[], uint32_t arg_index, int argc){
    while (arg_index < argc && IsNumber(argv[arg_index])){
        rxtx_nums.push_back(atoi(argv[arg_index]));
        arg_index++;
    }

    std::cout << rxtx_nums << '\n';

    printf("end=%d\n", arg_index-1);
    return arg_index-1;
}

uint64_t InputRxtxField(std::vector<uint32_t> &rxtx_field, char* argv[], uint32_t arg_index, int argc){
    while (arg_index < argc && IsNumber(argv[arg_index])){
        rxtx_field.push_back(atoi(argv[arg_index]));
        arg_index++;
    }

    std::cout << rxtx_field << '\n';

    printf("end=%d\n", arg_index-1);
    return arg_index-1;
}

bool CanTransfer(std::vector<uint32_t> &message, std::vector<uint32_t> &rxtx_nums, std::vector<uint32_t> &rxtx_field){
    if (message.size() == 0) {
        std::cout << "Empty message\n";
        return false;
    }
    else if (rxtx_nums.size() == 0) {
        std::cout << "Empty rxtx part in message\n";
        return false;
    }
    else if (rxtx_field.size() == 0) {
        std::cout << "Empty rxtx field\n";
        return false;
    }
    else {
        uint64_t index = 0;
        while (message[index] != 0){
            if (std::find(rxtx_nums.begin(), rxtx_nums.end(), message[index]) == rxtx_nums.end()){
                std::cout << "RXTX on the field not enogh\n";
                return false;
            }
            index++;
        }
    }
    return true;
}

void UpdateMessage(std::vector<uint32_t> &message, std::vector<uint32_t> &rxtx_nums, std::vector<uint32_t> &data){
    for (uint32_t index = 0; index < rxtx_nums.size(); index++) message.push_back(rxtx_nums[index]);
    message.push_back(0);
    for (uint32_t index = 0; index < data.size(); index++) message.push_back(data[index]);
    AddCrc16(message);

    std::cout << message << '\n';
}

void UpdateRxtx(std::vector<uint32_t> &rxtx_nums, std::vector<RXTX> &rxtxs){
    for (auto rxtx_num : rxtx_nums){
        RXTX new_rxtx(rxtx_num);
        bool already_exist = false;
        for (auto rxtx : rxtxs){
            if (rxtx.GetIndex() == new_rxtx.GetIndex()) already_exist = true;
        }
        if (!already_exist) rxtxs.push_back(new_rxtx);
    }

    printf("rxtx:\n");
    for (auto i : rxtxs) printf("%d\n", i.GetIndex());
}

void Transfer(std::vector<uint32_t> &rxtxs_numbers, std::vector<RXTX> &rxtxs, std::vector<uint32_t> &message){
    uint64_t message_index = 0;
    uint32_t rxtx_index = message[message_index];

    for (auto rxtx : rxtxs){
        if (rxtx.GetIndex() == rxtx_index){
            rxtx.TransferNext(rxtxs, message, message_index+1);
        }
    }
}

int main(int argc, char* argv[]){
    //message делится на три части: rxtx, data, rcr16, rxtx и data разделяются нулем (так как вышки нумеруются с 1)
    std::vector<uint32_t> message;
    std::vector<uint32_t> rxtx_nums;
    std::vector<uint32_t> data;

    std::vector<uint32_t> rxtx_field;
    std::vector<RXTX> rxtxs;

    for (int arg_index = 1; arg_index < argc; arg_index++){
        if (strcmp(argv[arg_index], "help") == 0){
            Help();
        }
        else if (strcmp(argv[arg_index], "input") == 0){
            if (arg_index + 1 < argc && strcmp(argv[arg_index + 1], "rxtxnums") == 0){
                arg_index = InputRxtxNums(rxtx_nums, argv, arg_index+2, argc);
            }
            else if (arg_index + 1 < argc && strcmp(argv[arg_index + 1], "rxtxfield") == 0){
                arg_index = InputRxtxField(rxtx_field, argv, arg_index+2, argc);
            }
            else if (arg_index + 1 < argc && strcmp(argv[arg_index + 1], "data") == 0){
                arg_index = InputData(data, argv, arg_index+2, argc);
            }
        }
        else if (strcmp(argv[arg_index], "transfer") == 0){
            UpdateMessage(message, rxtx_nums, data);
            if (CanTransfer(message, rxtx_nums, rxtx_field)){
                UpdateRxtx(rxtx_nums, rxtxs);
                Transfer(rxtx_nums, rxtxs, message);
                for (auto i : rxtxs) std::cout << i.GetIndex() << ' ' << i.GetData() << '\n';
                std::cout << "okay" << '\n';
            }
        }
        else {
            std::cout << "Incorrect arguments, use help\n";
            return 1;
        }
    }

    return 0;
}