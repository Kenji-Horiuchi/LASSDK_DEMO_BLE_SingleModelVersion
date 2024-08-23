//
// Copyright 2023 Sony Semiconductor Solutions Corporation.
//

#include <cstdio>
#include <algorithm>
#include <vector>
#include <string>

#define NUMBER_OF_ACTION 4

namespace apps {
  namespace sensor {

    // ***** 宣言部分はヘッダーに配置して、定義部分はこのファイルに配置する *****
    class ConvertDetectionResultToUint8 {
        private:
            std::string Name;
            std::vector<uint8_t> Data;
        public:
            ConvertDetectionResultToUint8(std::string InputName, std::vector<uint8_t> InputData);
            std::string GetName();
            std::vector<uint8_t> GetData();
            ~ConvertDetectionResultToUint8();
    };
    class GATT_DATA{
        private:
            std::vector<uint8_t> send_data;
            uint8_t connection_id;
            uint16_t size;
        public:
            GATT_DATA(std::vector<uint8_t> data, uint8_t id, uint16_t data_size);
            void SendData_StrToUint8(std::string str, uint8_t id, uint16_t data_size); // ,     ConvertDetectionResultToUint8* convert_table);
            uint8_t* GetData();
            uint8_t  GetID();
            uint16_t GetSIZE();
            std::vector<uint8_t>* GetData_vector();
            ~GATT_DATA();
    };

  }  // namespace sensor
}  // namespace apps